#include"PCH.h"
#include"Renderer.h"
#include"GlobalSettings.h"

namespace Aen {

	Renderer::Renderer(Window& window)
		:m_window(window), m_screenQuad(), m_cbBGColor(), m_cbTransform(), m_cbLightCount(), m_cbCamera(), m_sbLight(1024), m_postProcessBuffer(window), m_layerBuffer(window, 7u), 
		m_backBuffer(), m_viewPort(), m_clampSampler(SamplerType::CLAMP), m_depthMap(m_window), m_writeStencil(true, StencilType::Write), 
		m_maskStencil(false, StencilType::Mask), m_offStencil(true, StencilType::Off),
		m_rasterizerState(FillMode::Solid, CullMode::Front), m_dispatchInfo(), m_frustumCS(), m_lightCullCS(), m_frustums(), m_lIndexCount(), m_lIndex(), m_lGrid() {}

	void Renderer::Initialize() {

		ZeroMemory(&m_viewPort, sizeof(D3D11_VIEWPORT));

		m_viewPort.TopLeftX = 0.f;
		m_viewPort.TopLeftY = 0.f;
		m_viewPort.Width = static_cast<float>(m_window.GetSize().x);
		m_viewPort.Height = static_cast<float>(m_window.GetSize().y);
		m_viewPort.MinDepth = 0.f;
		m_viewPort.MaxDepth = 1.f;

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


		// temp --------------------------------------------------------------------

		m_dispatchInfo.GetData().threadGroups.x = (int)std::ceilf((float)m_window.GetSize().x / 256.f);
		m_dispatchInfo.GetData().threadGroups.y = (int)std::ceilf((float)m_window.GetSize().y / 256.f);

		m_dispatchInfo.GetData().totalThreads.x = m_window.GetSize().x;
		m_dispatchInfo.GetData().totalThreads.y = m_window.GetSize().y;

		uint32_t size = m_window.GetSize().x / 16 * m_window.GetSize().y / 16;
		m_frustums.Create(64, size);

		if(!m_frustumCS.Create(AEN_OUTPUT_DIR_WSTR(L"FrustomCS.cso")))
			if(!m_frustumCS.Create(L"FrustomCS.cso"))
				throw;

		m_dispatchInfo.UpdateBuffer();
		m_dispatchInfo.BindBuffer<CShader>(0u);
		RenderSystem::BindUnOrderedAccessView(0u, m_frustums);
		RenderSystem::BindShader(m_frustumCS);

		RenderSystem::Dispatch(m_dispatchInfo.GetData().threadGroups, 1u);

		RenderSystem::UnBindShader<CShader>();
		RenderSystem::UnBindUnOrderedAccessViews(0u, 1u);

		if(!m_lightCullCS.Create(AEN_OUTPUT_DIR_WSTR(L"LightCullCS.cso")))
			if(!m_lightCullCS.Create(L"LightCullCS.cso"))
				throw;

		const uint32_t avarageLights = 200u;
		m_lGrid.Create(m_window.GetSize().x / 16, m_window.GetSize().y / 16);
		m_lIndexCount.Create(sizeof(uint32_t), size * avarageLights);
		m_lIndex.Create(sizeof(uint32_t), size * avarageLights);

		//--------------------------------------------------------------------------

		RenderSystem::SetViewPort(m_viewPort);
		RenderSystem::SetPrimitiveTopology(Topology::TRIANGLELIST);
		RenderSystem::SetRasteriserState(m_rasterizerState);
	}

	void Renderer::Render() {
		
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

		uint32_t lightIndex = 0u;
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

		for(uint32_t i = 0u; i < 7u; i++)
			if(ComponentHandler::m_meshLayer[i].size() > 0) {

				RenderSystem::UnBindRenderTargets(1u);
				RenderSystem::BindRenderTargetView(m_depthMap);
				RenderSystem::SetDepthStencilState(m_offStencil, 0xFF);
				
				// Pre Depth Pass

				for(auto& k : ComponentHandler::m_meshLayer[i]) k.second->DepthDraw(*this, k.first, i);

				// Light Clip Pass

				RenderSystem::UnBindRenderTargets(1u);

				m_sbLight.BindSRV<CShader>(0u);
				RenderSystem::BindShaderResourceView<CShader>(1u, m_frustums);
				RenderSystem::BindShaderResourceView<CShader>(2u, m_depthMap);
				RenderSystem::BindUnOrderedAccessView(0u, m_lIndexCount);
				RenderSystem::BindUnOrderedAccessView(1u, m_lIndex);
				RenderSystem::BindUnOrderedAccessView(2u, m_lGrid);
				m_dispatchInfo.BindBuffer<CShader>(0u);
				RenderSystem::BindShader(m_lightCullCS);
				
				RenderSystem::Dispatch(m_dispatchInfo.GetData().threadGroups, 1u);
				
				RenderSystem::UnBindShader<CShader>();
				RenderSystem::UnBindUnOrderedAccessViews(0u, 3u);
				RenderSystem::UnBindShaderResources<CShader>(0u, 3u);

				// Draw pass

				for(auto& k : ComponentHandler::m_meshLayer[i]) k.second->Draw(*this, k.first, i);

				RenderSystem::ClearDepthStencilView(m_depthMap, true, false);
			}

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

	//void Renderer::Draw(std::unordered_map<uint32_t, MeshInstance*>& meshLayer, const uint32_t& layer) {

	//	// Pre Depth Pass

	//	RenderSystem::UnBindRenderTargets(1u);
	//	RenderSystem::BindRenderTargetView(m_depth[layer]);
	//	RenderSystem::SetDepthStencilState(m_offStencil[layer], 0xFF);

	//	for(auto& i : meshLayer) {
	//		Mesh* pMesh = i.second->m_pMesh;

	//		if(pMesh) {

	//			m_cbTransform.GetData().m_mdlMat = EntityHandler::GetEntity(i.first).GetTransformation().Transposed();
	//			m_cbTransform.UpdateBuffer();

	//			Material* pMaterial = (pMesh && pMesh->m_pMaterials[0]) ? pMesh->m_pMaterials[0] : nullptr;
	//			if(pMaterial) {
	//				RenderSystem::SetInputLayout(pMaterial->m_pShaderModel->m_iLayoutPass1);
	//				RenderSystem::BindShader<VShader>(pMaterial->m_pShaderModel->m_VShaderPass1);
	//				RenderSystem::UnBindShader<PShader>();

	//				uint32_t* slots = pMaterial->m_pShaderModel->m_slotsPass1;
	//				if(slots[0] != UINT_MAX) m_cbTransform.BindBuffer<VShader>(slots[0]);
	//				if(slots[1] != UINT_MAX) m_cbLightCount.BindBuffer<VShader>(slots[1]);
	//				if(slots[2] != UINT_MAX) m_cbCamera.BindBuffer<VShader>(slots[2]);
	//				if(slots[3] != UINT_MAX) m_cbUseTexture.BindBuffer<VShader>(slots[3]);
	//				if(slots[4] != UINT_MAX) m_sbLight.BindSRV<VShader>(slots[4]);
	//			}

	//			pMesh->m_vertices.BindBuffer();
	//			pMesh->m_vertices.Draw();
	//		}
	//	}

	//	
	//}
}