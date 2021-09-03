#include"PCH.h"
#include"Renderer.h"
#include"GlobalSettings.h"

namespace Aen {

	Renderer::Renderer(Window& window)
		:m_window(window), m_screenQuad(), m_cbBGColor(), m_cbTransform(), m_cbLightCount(), m_cbCamera(), m_sbLight(900), m_postProcessBuffer(window), m_layerBuffer(window, 7u), 
		m_backBuffer(), m_viewPort(), m_clampSampler(SamplerType::CLAMP), m_depth{}, m_writeStencil{}, m_maskStencil{}, m_offStencil{},
		m_rasterizerState(FillMode::Solid, CullMode::Front) {}

	void Renderer::Initialize() {

		ZeroMemory(&m_viewPort, sizeof(D3D11_VIEWPORT));

		m_viewPort.TopLeftX = 0.f;
		m_viewPort.TopLeftY = 0.f;
		m_viewPort.Width = static_cast<float>(m_window.GetSize().x);
		m_viewPort.Height = static_cast<float>(m_window.GetSize().y);
		m_viewPort.MinDepth = 0.f;
		m_viewPort.MaxDepth = 1.f;

		for(uint32_t i = 0; i < 7; i++) {
			m_depth[i].Create(m_window);
			m_writeStencil[i].SetStencil(true, StencilType::Write);
			m_maskStencil[i].SetStencil(false, StencilType::Mask);
			m_offStencil[i].SetStencil(true, StencilType::Off);
		}

		if(!m_postProcessVS.Create(AEN_OUTPUT_DIR_WSTR(L"PostProcessVS.cso")))
			if(!m_postProcessVS.Create(L"PostProcessVS.cso"))
				throw;

		if(!m_postProcessPS.Create(AEN_OUTPUT_DIR_WSTR(L"PostProcessPS.cso")))
			if(!m_postProcessPS.Create(L"PostProcessPS.cso"))
				throw;

		if(!m_combineLayersPS.Create(AEN_OUTPUT_DIR_WSTR(L"CombineLayersPS.cso")))
			if(!m_combineLayersPS.Create(L"CombineLayersPS.cso"))
				throw;

		m_postLayout.Create(m_postProcessVS);

		RenderSystem::SetViewPort(m_viewPort);
		RenderSystem::SetPrimitiveTopology(Topology::TRIANGLELIST);
		RenderSystem::SetRasteriserState(m_rasterizerState);
	}

	void Renderer::Render() {
		
		for(uint32_t i = 0; i < 7; i++)
			RenderSystem::ClearDepthStencilView(m_depth[i], true, false);

		RenderSystem::ClearRenderTargetView(m_backBuffer, Color(0.f, 0.f, 0.f, 0.f));
		RenderSystem::ClearRenderTargetView(m_layerBuffer, Color(0.f, 0.f, 0.f, 0.f));
		RenderSystem::ClearRenderTargetView(m_postProcessBuffer, Color(0.f, 0.f, 0.f, 0.f));

		// Camera

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

		// Light

		uint32_t lightIndex = 0;
		for(auto& i : ComponentHandler::m_lights) {
			i.second->m_light.m_pos = (ComponentHandler::TranslationExist(i.first)) ? ComponentHandler::GetTranslation(i.first).GetPos() : Vec3f::zero;
			i.second->m_light.m_dir = (ComponentHandler::RotationExist(i.first)) ? Transform(ComponentHandler::GetRotation(i.first).GetTranform(), Vec3f(0.f, 1.f, 0.f)) : Vec3f(0.f, 1.f, 0.f);
			m_sbLight.GetData(lightIndex) = i.second->m_light;
			lightIndex++;
		}

		m_sbLight.UpdateBuffer();
		m_cbLightCount.GetData() = ComponentHandler::m_lights.size();
		m_cbLightCount.UpdateBuffer();

		// Layered Rendering

		for(uint32_t i = 0; i < 7; i++)
			if(ComponentHandler::m_meshLayer[i].size() > 0)
				Draw(ComponentHandler::m_meshLayer[i], i);

		// Combine Layers Pass
		
		RenderSystem::SetInputLayout(m_postLayout);
		RenderSystem::UnBindRenderTargets(1u);

		m_cbBGColor.GetData() = GlobalSettings::GetBGColor();
		m_cbBGColor.UpdateBuffer();
		m_cbBGColor.BindBuffer<PShader>(0u);
		RenderSystem::BindRenderTargetView(m_postProcessBuffer);
		RenderSystem::BindShader<VShader>(m_postProcessVS);
		RenderSystem::BindShader<PShader>(m_combineLayersPS);
		RenderSystem::BindSamplers<PShader>(0u, m_clampSampler);
		RenderSystem::BindShaderResourceView<PShader>(0u, m_layerBuffer);

		m_screenQuad.Draw();
		// Post Process pass

		RenderSystem::UnBindShaderResources<PShader>(0u, m_layerBuffer.GetCount());
		RenderSystem::UnBindRenderTargets(m_postProcessBuffer.GetCount());

		RenderSystem::BindRenderTargetView(m_backBuffer);
		RenderSystem::BindShader<VShader>(m_postProcessVS);
		RenderSystem::BindShader<PShader>(m_postProcessPS);
		RenderSystem::BindShaderResourceView<PShader>(0u, m_postProcessBuffer);

		m_screenQuad.Draw();

		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		ImGui::Begin("Debug");
		ImGui::Text("deez");
		// code here

		ImGui::End();
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		// Present

		RenderSystem::Present();
	}

	void Renderer::Draw(std::unordered_map<uint32_t, MeshInstance*>& meshLayer, const uint32_t& layer) {

		// Pre Depth Pass

		RenderSystem::UnBindRenderTargets(1u);
		RenderSystem::BindRenderTargetView(m_depth[layer]);
		RenderSystem::SetDepthStencilState(m_offStencil[layer], 0xFF);

		for(auto& i : meshLayer) {
			Mesh* pMesh = i.second->m_pMesh;

			if(pMesh) {

				m_cbTransform.GetData().m_mdlMat = EntityHandler::GetEntity(i.first).GetTransformation().Transposed();
				m_cbTransform.UpdateBuffer();

				Material* pMaterial = (pMesh && pMesh->m_pMaterials[0]) ? pMesh->m_pMaterials[0] : nullptr;
				if(pMaterial) {
					RenderSystem::SetInputLayout(pMaterial->m_pShaderModel->m_iLayoutPass1);
					RenderSystem::BindShader<VShader>(pMaterial->m_pShaderModel->m_VShaderPass1);
					RenderSystem::UnBindShader<PShader>();

					uint32_t* slots = pMaterial->m_pShaderModel->m_slotsPass1;
					if(slots[0] != UINT_MAX) m_cbTransform.BindBuffer<VShader>(slots[0]);
					if(slots[1] != UINT_MAX) m_cbLightCount.BindBuffer<VShader>(slots[1]);
					if(slots[2] != UINT_MAX) m_cbCamera.BindBuffer<VShader>(slots[2]);
					if(slots[3] != UINT_MAX) m_cbUseTexture.BindBuffer<VShader>(slots[3]);
					if(slots[4] != UINT_MAX) m_sbLight.BindSRV<VShader>(slots[4]);
				}

				pMesh->m_vertices.BindBuffer();
				pMesh->m_vertices.Draw();
			}
		}

		for(auto& i : meshLayer) { 

			Mesh* pMesh = i.second->m_pMesh;

			if(pMesh) {

				// Transform

				m_cbTransform.GetData().m_mdlMat = EntityHandler::GetEntity(i.first).GetTransformation().Transposed();
				m_cbTransform.UpdateBuffer();

				// Mesh and Material

				for(uint32_t j = 0; j < pMesh->m_partitions.size(); j++) {

					pMesh->m_vertices.BindBuffer();

					// Opaque pass

					uint32_t materialIndex = pMesh->m_partitions[j].materialIndex;
					Material* pMaterial = (pMesh->m_pMaterials[materialIndex]) ? pMesh->m_pMaterials[materialIndex] : nullptr;
					if(pMaterial) {

						RenderSystem::UnBindShaderResources<PShader>(0u, pMaterial->m_pShaderModel->m_gBuffer.GetCount());
						RenderSystem::SetInputLayout(pMaterial->m_pShaderModel->m_iLayoutPass1);

						RenderSystem::BindShader<VShader>(pMaterial->m_pShaderModel->m_VShaderPass1);
						RenderSystem::BindShader<PShader>(pMaterial->m_pShaderModel->m_PShaderPass1);

						uint32_t* slots = pMaterial->m_pShaderModel->m_slotsPass1;

						if(slots[0] != UINT_MAX) m_cbTransform.BindBuffer<VShader>(slots[0]);
						if(slots[1] != UINT_MAX) m_cbLightCount.BindBuffer<VShader>(slots[1]);
						if(slots[2] != UINT_MAX) m_cbCamera.BindBuffer<VShader>(slots[2]);
						if(slots[3] != UINT_MAX) m_cbUseTexture.BindBuffer<VShader>(slots[3]);
						if(slots[4] != UINT_MAX) m_sbLight.BindSRV<VShader>(slots[4]);

						if(slots[5] != UINT_MAX) m_cbTransform.BindBuffer<PShader>(slots[5]);
						if(slots[6] != UINT_MAX) m_cbLightCount.BindBuffer<PShader>(slots[6]);
						if(slots[7] != UINT_MAX) m_cbCamera.BindBuffer<PShader>(slots[7]);
						if(slots[8] != UINT_MAX) m_cbUseTexture.BindBuffer<PShader>(slots[8]);
						if(slots[9] != UINT_MAX) m_sbLight.BindSRV<PShader>(slots[9]);

						for(UINT k = 0; k < 4; k++)
							if(pMaterial->m_textures[k] && slots[10 + k] != UINT_MAX) {
								RenderSystem::BindShaderResourceView<PShader>(slots[10 + k], pMaterial->m_textures[k]->m_shaderResource);
								m_cbUseTexture.GetData()[k] = (int)true;
							} else
								m_cbUseTexture.GetData()[k] = (int)false;

						pMaterial->m_dBuffer.UpdateBuffer();
						if(slots[14] != UINT_MAX) pMaterial->m_dBuffer.BindBuffer<PShader>(slots[14]);
						if(slots[15] != UINT_MAX) pMaterial->m_dBuffer.BindBuffer<PShader>(slots[15]);

						m_cbUseTexture.UpdateBuffer();

						RenderSystem::BindSamplers<PShader>(pMaterial->m_pShaderModel->m_samplerDataPass1.first, pMaterial->m_pShaderModel->m_samplerDataPass1.second);

						RenderSystem::UnBindRenderTargets(1u);
						RenderSystem::ClearRenderTargetView(pMaterial->m_pShaderModel->m_gBuffer, Color(0.f, 0.f, 0.f, 0.f));
						RenderSystem::BindRenderTargetView(pMaterial->m_pShaderModel->m_gBuffer, m_depth[layer]);
					}

					RenderSystem::ClearDepthStencilView(m_depth[layer], false, true);
					RenderSystem::SetDepthStencilState(m_writeStencil[layer], 0xFF);
					pMesh->m_vertices.Draw(pMesh->m_partitions[j].size, pMesh->m_partitions[j].offset);

					// Per Object Post Process Pass

					if(pMaterial) {

						RenderSystem::BindShader<VShader>(pMaterial->m_pShaderModel->m_VShaderPass2);
						RenderSystem::BindShader<PShader>(pMaterial->m_pShaderModel->m_PShaderPass2);

						std::optional<D3D11_SHADER_INPUT_BIND_DESC> bDescShaderModelVS = pMaterial->m_pShaderModel->m_VSReflectPass2.GetBindDescByName(pMaterial->m_pShaderModel->m_bufferName);
						std::optional<D3D11_SHADER_INPUT_BIND_DESC> bDescShaderModelPS = pMaterial->m_pShaderModel->m_PSReflectPass2.GetBindDescByName(pMaterial->m_pShaderModel->m_bufferName);

						if(bDescShaderModelVS) pMaterial->m_dBuffer.BindBuffer<PShader>(bDescShaderModelVS.value().BindPoint);
						if(bDescShaderModelPS) pMaterial->m_dBuffer.BindBuffer<PShader>(bDescShaderModelPS.value().BindPoint);

						uint32_t* slots = pMaterial->m_pShaderModel->m_slotsPass2;

						if(slots[0] != UINT_MAX) m_cbTransform.BindBuffer<VShader>(slots[0]);
						if(slots[1] != UINT_MAX) m_cbLightCount.BindBuffer<VShader>(slots[1]);
						if(slots[2] != UINT_MAX) m_cbCamera.BindBuffer<VShader>(slots[2]);
						if(slots[3] != UINT_MAX) m_cbUseTexture.BindBuffer<VShader>(slots[3]);
						if(slots[4] != UINT_MAX) m_sbLight.BindSRV<VShader>(slots[4]);

						if(slots[5] != UINT_MAX) m_cbTransform.BindBuffer<PShader>(slots[5]);
						if(slots[6] != UINT_MAX) m_cbLightCount.BindBuffer<PShader>(slots[6]);
						if(slots[7] != UINT_MAX) m_cbCamera.BindBuffer<PShader>(slots[7]);
						if(slots[8] != UINT_MAX) m_cbUseTexture.BindBuffer<PShader>(slots[8]);
						if(slots[9] != UINT_MAX) m_sbLight.BindSRV<PShader>(slots[9]);

						if(slots[14] != UINT_MAX) pMaterial->m_dBuffer.BindBuffer<PShader>(slots[14]);
						if(slots[15] != UINT_MAX) pMaterial->m_dBuffer.BindBuffer<PShader>(slots[15]);

						RenderSystem::BindSamplers<PShader>(pMaterial->m_pShaderModel->m_samplerDataPass2.first, pMaterial->m_pShaderModel->m_samplerDataPass2.second);
						RenderSystem::SetInputLayout(pMaterial->m_pShaderModel->m_iLayoutPass2);
						RenderSystem::UnBindRenderTargets(pMaterial->m_pShaderModel->m_gBuffer.GetCount());
						RenderSystem::BindShaderResourceView<PShader>(0, pMaterial->m_pShaderModel->m_gBuffer);
					}

					RenderSystem::BindRenderTargetView(m_layerBuffer.GetRtv(layer), m_depth[layer]);
					RenderSystem::SetDepthStencilState(m_maskStencil[layer], 0xFF);

					m_screenQuad.Draw();
				}
			}
		}
	}
}