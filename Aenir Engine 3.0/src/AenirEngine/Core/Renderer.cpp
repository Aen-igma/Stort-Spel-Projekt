#include"PCH.h"
#include"Renderer.h"
#include"GlobalSettings.h"

namespace Aen {

	Renderer::Renderer(Window& window)
		:m_window(window), m_screenQuad(), m_cbTransform(), m_cbLightCount(), m_cbCamera(), m_sbLight(900), m_postProcessBuffer(window, 2u), m_backBuffer(), m_viewPort(),
		m_clampSampler(SamplerType::CLAMP), m_depth(window), m_writeStencil(true, StencilType::Write), m_maskStencil(false, StencilType::Mask), m_offStencil(true, StencilType::Off),
		m_rasterizerState(FillMode::Solid, CullMode::Front) {}

	void Renderer::Initialize() {

		ZeroMemory(&m_viewPort, sizeof(D3D11_VIEWPORT));

		m_viewPort.TopLeftX = 0.f;
		m_viewPort.TopLeftY = 0.f;
		m_viewPort.Width = static_cast<float>(m_window.GetSize().x);
		m_viewPort.Height = static_cast<float>(m_window.GetSize().y);
		m_viewPort.MinDepth = 0.f;
		m_viewPort.MaxDepth = 1.f;

		if(!m_postProcessVS.Create(AEN_OUTPUT_DIR_WSTR(L"PostProcessVS.cso")))
			if(m_postProcessVS.Create(L"PostProcessVS.cso"))
				throw;

		if(!m_postProcessPS.Create(AEN_OUTPUT_DIR_WSTR(L"PostProcessPS.cso")))
			if(!m_postProcessPS.Create(L"PostProcessPS.cso"))
				throw;

		m_postLayout.m_inputDesc = {
				{"POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT,    0,                            0, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"UV",        0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		};

		m_postLayout.Create(m_postProcessVS);

		RenderSystem::SetViewPort(m_viewPort);
		RenderSystem::SetPrimitiveTopology(Topology::TRIANGLELIST);
		RenderSystem::SetRasteriserState(m_rasterizerState);
	}

	void Renderer::Draw() {
		
		RenderSystem::ClearDepthStencilView(m_depth, true, false);
		RenderSystem::ClearRenderTargetView(m_backBuffer, Color(0.08f, 0.08f, 0.13f, 1.f));
		RenderSystem::ClearRenderTargetView(m_postProcessBuffer, Color(0.08f, 0.08f, 0.13f, 1.f));

		// Camera

		m_cbTransform.BindBuffer<VShader>(0);
		if(GlobalSettings::m_pMainCamera) {

			Entity* pCam = GlobalSettings::m_pMainCamera;

			Vec3f pos = pCam->GetPos();
			Vec3f rot = pCam->GetRot();

			pCam->GetComponent<Camera>().UpdateView(pos, rot);
			
			m_cbCamera.GetData().pos = pos;
			m_cbCamera.GetData().fDir = pCam->GetComponent<Camera>().GetForward();
			m_cbCamera.GetData().uDir = pCam->GetComponent<Camera>().GetUp();
			m_cbCamera.UpdateBuffer();

			m_cbTransform.GetData().m_vMat = pCam->GetComponent<Camera>().GetView().Transposed();
			m_cbTransform.GetData().m_pMat = pCam->GetComponent<Camera>().GetProjecton().Transposed();
		} else {
			m_cbTransform.GetData().m_vMat = Mat4f::identity;
			m_cbTransform.GetData().m_pMat = Mat4f::identity;
		}

		m_cbCamera.BindBuffer<PShader>(3);
		m_cbCamera.BindBuffer<VShader>(3);

		// Light

		uint32_t lightIndex = 0;
		for(auto& i : ComponentHandler::m_lights) {
			i.second->m_light.m_pos = (ComponentHandler::TranslationExist(i.first)) ? ComponentHandler::GetTranslation(i.first).GetPos() : Vec3f::zero;
			i.second->m_light.m_dir = (ComponentHandler::RotationExist(i.first)) ? Transform(ComponentHandler::GetRotation(i.first).GetTranform(), Vec3f(0.f, 1.f, 0.f)) : Vec3f(0.f, 1.f, 0.f);
			m_sbLight.GetData(lightIndex) = i.second->m_light;
			lightIndex++;
		}

		m_sbLight.UpdateBuffer();
		m_sbLight.BindSRV<PShader>(4);
		m_cbLightCount.GetData() = ComponentHandler::m_lights.size();
		m_cbLightCount.UpdateBuffer();

		// Pre Depth Pass

		for(auto& i : ComponentHandler::m_mesheInstances) {
			Mesh* pMesh = i.second->m_mesh;

			if(pMesh) {
				
				uint32_t id = i.first;
				
				Mat4f parentTranform;
				if(EntityHandler::GetEntity(id).m_hasParent) {
					uint32_t parentId = EntityHandler::GetEntity(id).m_parentId;
					Mat4f parentPos = (ComponentHandler::TranslationExist(parentId)) ? ComponentHandler::GetTranslation(parentId).GetTranform() : Mat4f::identity;
					Mat4f parentRot = (ComponentHandler::RotationExist(parentId)) ? ComponentHandler::GetRotation(parentId).GetTranform() : Mat4f::identity;
					Mat4f parentScale = (ComponentHandler::ScaleExist(parentId)) ? ComponentHandler::GetScale(parentId).GetTranform() : Mat4f::identity;
					parentTranform = parentScale * parentRot * parentPos;
				}

				Mat4f pos = (ComponentHandler::TranslationExist(id)) ? ComponentHandler::GetTranslation(id).GetTranform() : Mat4f::identity;
				Mat4f rot = (ComponentHandler::RotationExist(id)) ? ComponentHandler::GetRotation(id).GetTranform() : Mat4f::identity;
				Mat4f scale = (ComponentHandler::ScaleExist(id)) ? ComponentHandler::GetScale(id).GetTranform() : Mat4f::identity;
				m_cbTransform.GetData().m_mdlMat = (scale * rot * pos * parentTranform).Transposed();
				m_cbTransform.UpdateBuffer();
				
				Material* pMaterial = (pMesh && pMesh->m_pMaterials[0]) ? pMesh->m_pMaterials[0] : nullptr;
				if(pMaterial) {
					RenderSystem::SetInputLayout(pMaterial->m_pShaderModel->m_iLayoutPass1);
					RenderSystem::BindShader<VShader>(pMaterial->m_pShaderModel->m_VShaderPass1);
					RenderSystem::UnBindShader<PShader>();
				}

				RenderSystem::UnBindRenderTargets(1);
				RenderSystem::BindRenderTargetView(m_depth);
				RenderSystem::SetDepthStencilState(m_offStencil, 0xFF);

				pMesh->m_vertices.BindBuffer();
				pMesh->m_vertices.Draw();
			}
		}

		for(auto& i : ComponentHandler::m_mesheInstances) { 

			Mesh* pMesh = i.second->m_mesh;
			
			if(pMesh) {

				// Transform

				uint32_t id = i.first;
				Mat4f parentTranform;
				if(EntityHandler::GetEntity(id).m_hasParent) {
					uint32_t parentId = EntityHandler::GetEntity(id).m_parentId;
					Mat4f parentPos = (ComponentHandler::TranslationExist(parentId)) ? ComponentHandler::GetTranslation(parentId).GetTranform() : Mat4f::identity;
					Mat4f parentRot = (ComponentHandler::RotationExist(parentId)) ? ComponentHandler::GetRotation(parentId).GetTranform() : Mat4f::identity;
					Mat4f parentScale = (ComponentHandler::ScaleExist(parentId)) ? ComponentHandler::GetScale(parentId).GetTranform() : Mat4f::identity;
					parentTranform = parentScale * parentRot * parentPos;
				}

				Mat4f pos = (ComponentHandler::TranslationExist(id)) ? ComponentHandler::GetTranslation(id).GetTranform() : Mat4f::identity;
				Mat4f rot = (ComponentHandler::RotationExist(id)) ? ComponentHandler::GetRotation(id).GetTranform() : Mat4f::identity;
				Mat4f scale = (ComponentHandler::ScaleExist(id)) ? ComponentHandler::GetScale(id).GetTranform() : Mat4f::identity;
				m_cbTransform.GetData().m_mdlMat = (scale * rot * pos * parentTranform).Transposed();
				m_cbTransform.UpdateBuffer();

				// Mesh and Material

				for(uint32_t j = 0; j < pMesh->m_partitions.size(); j++) {

					pMesh->m_vertices.BindBuffer();
					m_cbLightCount.BindBuffer<PShader>(2);

					// Opaque pass

					RenderSystem::UnBindShaderResources<PShader>(0u, 4u);
					uint32_t materialIndex = pMesh->m_partitions[j].materialIndex;
					Material* pMaterial = (pMesh->m_pMaterials[materialIndex]) ? pMesh->m_pMaterials[materialIndex] : nullptr;
					if(pMaterial) {

						RenderSystem::SetInputLayout(pMaterial->m_pShaderModel->m_iLayoutPass1);

						for(UINT k = 0; k < 4; k++)
							if(pMaterial->m_textures[k]) {
								RenderSystem::BindShaderResourceView<PShader>(k, pMaterial->m_textures[k]->m_shaderResource);
								m_cbUseTexture.GetData()[k] = (int)true;
							} else
								m_cbUseTexture.GetData()[k] = (int)false;

						m_cbUseTexture.UpdateBuffer();
						m_cbUseTexture.BindBuffer<PShader>(4);

						RenderSystem::BindShader<VShader>(pMaterial->m_pShaderModel->m_VShaderPass1);
						RenderSystem::BindShader<PShader>(pMaterial->m_pShaderModel->m_PShaderPass1);

						pMaterial->m_dBuffer.BindBuffer<PShader>(pMaterial->m_pShaderModel->m_dbLayout.first);
						pMaterial->m_dBuffer.UpdateBuffer();

						RenderSystem::BindSamplers<PShader>(pMaterial->m_pShaderModel->m_samplerDataPass1.first, pMaterial->m_pShaderModel->m_samplerDataPass1.second);

						RenderSystem::UnBindRenderTargets(1);
						RenderSystem::ClearRenderTargetView(pMaterial->m_pShaderModel->m_gBuffer, Color(0.f, 0.f, 0.f, 0.f));
						RenderSystem::BindRenderTargetView(pMaterial->m_pShaderModel->m_gBuffer, m_depth);
					}

					RenderSystem::ClearDepthStencilView(m_depth, false, true);
					RenderSystem::SetDepthStencilState(m_writeStencil, 0xFF);
					pMesh->m_vertices.Draw(pMesh->m_partitions[j].size, pMesh->m_partitions[j].offset);

					// Per Object Post Process Pass

					if(pMaterial) {

						RenderSystem::BindShader<VShader>(pMaterial->m_pShaderModel->m_VShaderPass2);
						RenderSystem::BindShader<PShader>(pMaterial->m_pShaderModel->m_PShaderPass2);

						pMaterial->m_dBuffer.BindBuffer<PShader>(pMaterial->m_pShaderModel->m_dbLayout.first);
						pMaterial->m_dBuffer.UpdateBuffer();

						RenderSystem::BindSamplers<PShader>(pMaterial->m_pShaderModel->m_samplerDataPass2.first, pMaterial->m_pShaderModel->m_samplerDataPass2.second);

						RenderSystem::UnBindRenderTargets(pMaterial->m_pShaderModel->m_gBuffer.GetCount());
						RenderSystem::SetInputLayout(pMaterial->m_pShaderModel->m_iLayoutPass2);

						RenderSystem::BindShaderResourceView<PShader>(0, pMaterial->m_pShaderModel->m_gBuffer);
					}

					
					RenderSystem::BindRenderTargetView(m_postProcessBuffer, m_depth);
					RenderSystem::SetDepthStencilState(m_maskStencil, 0xFF);

					m_cbTransform.BindBuffer<PShader>(0);
					m_screenQuad.Draw();
				}
			}
		}

		// Post Process pass
		
		RenderSystem::SetInputLayout(m_postLayout);
		RenderSystem::UnBindRenderTargets(m_postProcessBuffer.GetCount());

		RenderSystem::BindRenderTargetView(m_backBuffer);
		RenderSystem::BindShader<VShader>(m_postProcessVS);
		RenderSystem::BindShader<PShader>(m_postProcessPS);
		RenderSystem::BindSamplers<PShader>(0u, m_clampSampler);
		RenderSystem::BindShaderResourceView<PShader>(0u, m_postProcessBuffer);

		m_screenQuad.Draw();

		// Present

		RenderSystem::Present();
	}
}