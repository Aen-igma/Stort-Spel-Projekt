#include"PCH.h"
#include"Renderer.h"
#include"GlobalSettings.h"

namespace Aen {

	Renderer::Renderer(Window& window)
		:m_window(window), m_screenQuad(), m_cbBGColor(), m_cbTransform(), m_cbLightCount(), m_cbCamera(), m_sbLight(1024), m_postProcessBuffer(window), m_layerBuffer(window, 7u), 
		m_backBuffer(), m_viewPort(), m_clampSampler(SamplerType::CLAMP), m_depthMap(m_window), m_writeStencil(true, StencilType::Write), 
		m_maskStencil(false, StencilType::Mask), m_offStencil(true, StencilType::Off),
		m_rasterizerState(FillMode::Solid, CullMode::Front), m_dispatchInfo(), m_lightCullCS(), m_lIndex(), m_lGrid(), m_avarageLights(200u) {}

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

		if(!m_lightCullCS.Create(AEN_OUTPUT_DIR_WSTR(L"LightCullCS.cso")))
			if(!m_lightCullCS.Create(L"LightCullCS.cso"))
				throw;

		m_postLayout.Create(m_postProcessVS);

		m_dispatchInfo.GetData().windowSize.x = m_window.GetSize().x;
		m_dispatchInfo.GetData().windowSize.y = m_window.GetSize().y;
		m_dispatchInfo.GetData().numThreads.x = (int)std::ceil((float)m_window.GetSize().x / 16.f);
		m_dispatchInfo.GetData().numThreads.y = (int)std::ceil((float)m_window.GetSize().y / 16.f);
		m_dispatchInfo.GetData().threadGroups.x = (int)std::ceil((float)m_window.GetSize().x / 16.f);
		m_dispatchInfo.GetData().threadGroups.y = (int)std::ceil((float)m_window.GetSize().y / 16.f);
		m_dispatchInfo.GetData().avarageLights = m_avarageLights;
		m_dispatchInfo.UpdateBuffer();

		uint32_t size = m_dispatchInfo.GetData().numThreads.x * m_dispatchInfo.GetData().numThreads.y;
		m_lIndex.Create(sizeof(uint32_t), m_avarageLights * size);
		m_lGrid.Create(m_dispatchInfo.GetData().numThreads);
	}

	void Renderer::Render() {
		
		RenderSystem::SetViewPort(m_viewPort);
		RenderSystem::SetPrimitiveTopology(Topology::TRIANGLELIST);
		RenderSystem::SetRasteriserState(m_rasterizerState);
		RenderSystem::ClearRenderTargetView(m_backBuffer, Color(0.f, 0.f, 0.f, 0.f));
		RenderSystem::ClearRenderTargetView(m_layerBuffer, Color(0.f, 0.f, 0.f, 0.f));
		RenderSystem::ClearRenderTargetView(m_postProcessBuffer, Color(0.f, 0.f, 0.f, 0.f));

		// Camera

		if(GlobalSettings::m_pMainCamera) {

			//CamClass* pTempCam = GlobalSettings::m_pMainTempCam;
			Entity* pCam = GlobalSettings::m_pMainCamera;

			Vec3f pos = pCam->GetPos();
			Vec3f rot = pCam->GetRot();

			//sm::Vector3 pos = pTempCam->getPosition();
			//sm::Vector3 rot = pTempCam->getRotation();

			

			pCam->GetComponent<Camera>().UpdateView(pos, rot);
			

			//m_cbCamera.GetData().pos = pTempCam->getPosition();
			//m_cbCamera.GetData().uDir = pTempCam->getUpV();
			//m_cbCamera.GetData().fDir = pTempCam->getForwardV();
			m_cbCamera.GetData().pos = { pos.x, pos.y, pos.z };
			m_cbCamera.GetData().fDir = pCam->GetComponent<Camera>().GetForward();
			m_cbCamera.GetData().uDir = pCam->GetComponent<Camera>().GetUp();
			m_cbCamera.UpdateBuffer();

			m_cbTransform.GetData().m_vMat = pCam->GetComponent<Camera>().GetView().Transposed();
			m_cbTransform.GetData().m_pMat = pCam->GetComponent<Camera>().GetProjecton().Transposed();

			//m_cbTransform.GetData().m_vMat = pTempCam->getView().Transpose();
			//m_cbTransform.GetData().m_pMat = pTempCam->getProj().Transpose();
		} else {
			//m_cbTransform.GetData().m_pMat = sm::Matrix::Identity;
			//m_cbTransform.GetData().m_vMat = sm::Matrix::Identity;
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

				// Light Cull Pass

				RenderSystem::UnBindRenderTargets(1u);

				m_sbLight.BindSRV<CShader>(0u);
				RenderSystem::BindShaderResourceView<CShader>(1u, m_depthMap);
				RenderSystem::BindUnOrderedAccessView(0u, m_lIndex);
				RenderSystem::BindUnOrderedAccessView(1u, m_lGrid);
				RenderSystem::BindShader(m_lightCullCS);
				m_dispatchInfo.BindBuffer<CShader>(0u);
				m_cbLightCount.BindBuffer<CShader>(1u);
				m_cbTransform.BindBuffer<CShader>(2u);
				
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

		

		#ifdef _DEBUG
		RenderSystem::BindShaderResourceView<PShader>(4, m_lGrid);

		static bool toggle = false;
		toggle = (Input::KeyDown(Key::P)) ? !toggle : toggle;
		m_heatMap.GetData() = toggle;
		m_heatMap.UpdateBuffer();
		m_heatMap.BindBuffer<PShader>(0u);

		m_cbTransform.BindBuffer<CShader>(1u);
		#endif

		m_screenQuad.Draw();


		Aen::GlobalSettings::mp_guiHandler->NewFrame();
		Aen::GlobalSettings::mp_guiHandler->RenderAllWindows();
		Aen::GlobalSettings::mp_guiHandler->Render();

		// Present
		RenderSystem::Present();
		RenderSystem::ClearState();
	}
}