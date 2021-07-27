#include"PCH.h"
#include"Renderer.h"
#include"GlobalSettings.h"

namespace Aen {

	Renderer::Renderer(Window& window)
		:m_window(window), m_gBuffer(3, window), m_postVS(), m_postPS(), m_screenQuad(), m_postLayout(), m_borderSampler(SamplerType::CLAMP),
		m_cbTransform(), m_cbLightCount(), m_cbCamera(),
		m_sbPointLight(300), m_sbDirectionalLight(300), m_sbSpotLight(300),
		m_backBuffer(), m_viewPort(), m_depth(window), m_writeStencil(true, StencilType::Write), m_maskStencil(false, StencilType::Mask),
		m_rasterizerState(FillMode::Solid, CullMode::Front) {}

	void Renderer::Initialize() {

		ZeroMemory(&m_viewPort, sizeof(D3D11_VIEWPORT));

		m_viewPort.TopLeftX = 0.f;
		m_viewPort.TopLeftY = 0.f;
		m_viewPort.Width = static_cast<float>(m_window.GetSize().x);
		m_viewPort.Height = static_cast<float>(m_window.GetSize().y);
		m_viewPort.MinDepth = 0.f;
		m_viewPort.MaxDepth = 1.f;

		if(!m_postVS.Create(AEN_OUTPUT_DIR_WSTR(L"PostVS.cso")))
			if(!m_postVS.Create(L"PostVS.cso"))
				throw;

		if(!m_postPS.Create(AEN_OUTPUT_DIR_WSTR(L"PostPS.cso")))
			if(!m_postPS.Create(L"PostPS.cso"))
				throw;

		m_postLayout.m_inputDesc = {
			{"POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT,    0,                            0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"UV",        0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		};

		m_postLayout.Create(m_postVS);

		RenderSystem::SetViewPort(m_viewPort);
		RenderSystem::SetPrimitiveTopology(Topology::TRIANGLELIST);
		RenderSystem::SetRasteriserState(m_rasterizerState);
	}

	void Renderer::Draw() {
		
		RenderSystem::ClearDepthStencilView(m_depth, true, false);
		RenderSystem::ClearRenderTargetView(m_backBuffer, Color(0.08f, 0.08f, 0.13f, 1.f));

		// Camera

		m_cbTransform.BindBuffer<VShader>(0);
		if(GlobalSettings::m_pMainCamera) {

			Entity* pCam = GlobalSettings::m_pMainCamera;

			Vec3f pos = pCam->GetPos();
			Vec3f rot = pCam->GetRot();

			pCam->GetComponent<Camera>().UpdateView(pos, rot);
			
			m_cbCamera.GetData() = pos;
			m_cbCamera.UpdateBuffer();

			m_cbTransform.GetData().m_vMat = pCam->GetComponent<Camera>().GetView().Transposed();
			m_cbTransform.GetData().m_pMat = pCam->GetComponent<Camera>().GetProjecton().Transposed();
		} else {
			m_cbTransform.GetData().m_vMat = Mat4f::identity;
			m_cbTransform.GetData().m_pMat = Mat4f::identity;
		}

		m_cbCamera.BindBuffer<PShader>(2);

		// SpotLight

		uint32_t lightIndex = 0;
		for(auto& i : ComponentHandler::m_spotLights) {
			i.second->SetPos(ComponentHandler::GetTranslation(i.first).GetPos());
			i.second->SetRot(ComponentHandler::GetRotation(i.first).GetTranform());
			m_sbSpotLight.GetData(lightIndex) = i.second->m_spotLight;
			lightIndex++;
		}

		m_sbSpotLight.BindSRV<PShader>(4);
		m_sbSpotLight.UpdateBuffer();

		// PointLight

		lightIndex = 0;
		for(auto& i : ComponentHandler::m_pointLights) {
			i.second->SetPos(ComponentHandler::GetTranslation(i.first).GetPos());
			m_sbPointLight.GetData(lightIndex) = i.second->m_pointLight;
			lightIndex++;
		}

		m_sbPointLight.BindSRV<PShader>(5); 
		m_sbPointLight.UpdateBuffer();

		// DirectionalLight

		lightIndex = 0;
		for(auto& i : ComponentHandler::m_directionalLights) {
			i.second->SetRot(ComponentHandler::GetRotation(i.first).GetTranform());
			m_sbDirectionalLight.GetData(lightIndex) = i.second->m_directionalLight;
			lightIndex++;
		}

		m_sbDirectionalLight.BindSRV<PShader>(6);
		m_sbDirectionalLight.UpdateBuffer();

		// LightCount

		m_cbLightCount.GetData().x = ComponentHandler::m_spotLights.size();
		m_cbLightCount.GetData().y = ComponentHandler::m_pointLights.size();
		m_cbLightCount.GetData().z = ComponentHandler::m_directionalLights.size();

		// Mesh

		for(auto& i : ComponentHandler::m_mesheInstances) { 

			m_cbLightCount.BindBuffer<PShader>(1);
			m_cbLightCount.UpdateBuffer();

			uint32_t id = i.first;
			Mesh* pMesh = i.second->m_mesh;
			Material* pMaterial = (pMesh && ComponentHandler::MaterialInstanceExist(id)) ? ComponentHandler::GetMaterialInstance(id).m_pMaterial : nullptr;


			// Transform

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

			// Material

			RenderSystem::UnBindShaderResources<PShader>(0u, 4u);
			if(pMaterial) {

				RenderSystem::SetInputLayout(pMaterial->m_pShader->m_iLayout);

				for(UINT i = 0; i < 4; i++)
					if(pMaterial->m_textures[i]) {
						RenderSystem::BindShaderResourceView<PShader>(i, pMaterial->m_textures[i]->m_shaderResource);
						m_cbUseTexture.GetData()[i] = (int)true;
					} else
						m_cbUseTexture.GetData()[i] = (int)false;

				RenderSystem::BindShader<VShader>(pMaterial->m_pShader->m_VShader);
				RenderSystem::BindShader<PShader>(pMaterial->m_pShader->m_PShader);

				for(uint32_t i = 0; i < pMaterial->m_dBuffers.size(); i++) {
					pMaterial->m_dBuffers[i]->BindBuffer<PShader>(pMaterial->m_pShader->m_dbLayouts[i].first);
					pMaterial->m_dBuffers[i]->UpdateBuffer();
				}

				for(auto& i : pMaterial->m_pShader->m_samplerDatas)
					RenderSystem::BindSamplers<PShader>(i.first, i.second);
			}

			m_cbUseTexture.UpdateBuffer();
			m_cbUseTexture.BindBuffer<PShader>(3);

			RenderSystem::UnBindRenderTargets(1);
			RenderSystem::ClearRenderTargetView(m_gBuffer, Color(0.f, 0.f, 0.f, 0.f));
			RenderSystem::ClearDepthStencilView(m_depth, false, true);
			RenderSystem::BindRenderTargetView(m_gBuffer, m_depth);
			RenderSystem::SetDepthStencilState(m_writeStencil, 0xFF);

			if(pMesh) {
				pMesh->m_vertices.BindBuffer();
				pMesh->m_vertices.Draw();
			}

			RenderSystem::UnBindRenderTargets(m_gBuffer.GetCount());
			RenderSystem::BindRenderTargetView(m_backBuffer, m_depth);
			RenderSystem::SetDepthStencilState(m_maskStencil, 0xFF);
			RenderSystem::SetInputLayout(m_postLayout);

			RenderSystem::BindShaderResourceView<PShader>(0, m_gBuffer);
			RenderSystem::BindSamplers<PShader>(0, m_borderSampler);
			RenderSystem::BindShader<VShader>(m_postVS);
			RenderSystem::BindShader<PShader>(m_postPS);
			m_cbTransform.BindBuffer<PShader>(1);

			if(pMaterial)
				for(uint32_t i = 0; i < pMaterial->m_dBuffers.size(); i++) {
					pMaterial->m_dBuffers[i]->BindBuffer<PShader>(pMaterial->m_pShader->m_dbLayouts[i].first);
					pMaterial->m_dBuffers[i]->UpdateBuffer();
				}

			m_screenQuad.Draw();
		}

		RenderSystem::Present();
	}
}