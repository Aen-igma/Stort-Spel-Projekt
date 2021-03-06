#include"PCH.h"
#include"Renderer.h"
#include"GlobalSettings.h"

namespace Aen {

	Renderer::Renderer(Window& window)
		:m_window(window), m_screenQuad(), m_cbBGColor(), m_cbTransform(), m_cbLightCount(), m_cbCamera(), m_sbLight(1024), 
		m_backBuffer(), m_viewPort(), m_depthMap(window), m_shadowMap(2048.f, 2048.f), m_writeStencil(true, StencilType::Write), 
		m_maskStencil(false, StencilType::Mask), m_offStencil(true, StencilType::Off),
		m_rasterizerState(FillMode::Solid, CullMode::Front), m_wireFrameState(FillMode::Wireframe, CullMode::None), 
		m_dispatchInfo(), m_lightCullCS(), m_lIndex(), m_lGrid(), m_avarageLights(200u), m_wrapSampler(SamplerType::WRAP), m_borderSampler(SamplerType::BORDER), 
		m_toggleView(false), m_particleOut(window,2) {}

	void Renderer::Initialize() {

		ZeroMemory(&m_viewPort, sizeof(D3D11_VIEWPORT));

		m_viewPort.TopLeftX = 0.f;
		m_viewPort.TopLeftY = 0.f;
		m_viewPort.Width = static_cast<float>(m_window.GetSize().x);
		m_viewPort.Height = static_cast<float>(m_window.GetSize().y);
		m_viewPort.MinDepth = 0.f;
		m_viewPort.MaxDepth = 1.f;

		ZeroMemory(&m_viewPort2, sizeof(D3D11_VIEWPORT));

		m_viewPort2.TopLeftX = 0.f;
		m_viewPort2.TopLeftY = 0.f;
		m_viewPort2.Width = 2048.f;
		m_viewPort2.Height = 2048.f;
		m_viewPort2.MinDepth = 0.f;
		m_viewPort2.MaxDepth = 1.f;

		if(!m_opaqueVS.Create(AEN_OUTPUT_DIR_WSTR(L"OpaqueVS.cso")))
			if(!m_opaqueVS.Create(L"OpaqueVS.cso"))
				throw;

		if(!m_lightCullCS.Create(AEN_OUTPUT_DIR_WSTR(L"LightCullCS.cso")))
			if(!m_lightCullCS.Create(L"LightCullCS.cso"))
				throw;
#ifdef _DEBUG
		if (!m_collisionPS.Create(AEN_OUTPUT_DIR_WSTR(L"CollisionPS.cso")))
			if (!m_collisionPS.Create(L"CollisionPS.cso"))
				throw;
#endif

		if(!m_postProcessCS.Create(AEN_OUTPUT_DIR_WSTR(L"PostProcessCS.cso")))
			if(!m_postProcessCS.Create(L"PostProcessCS.cso"))
				throw;

		//-------------- Particle System ---------------//
		if (!m_PSVShader.Create(AEN_OUTPUT_DIR_WSTR(L"VertexParticleShader.cso")))
			if (!m_PSVShader.Create(L"VertexParticleShader.cso"))
				throw;
		if (!m_PTransparencyPS.Create(AEN_OUTPUT_DIR_WSTR(L"PTransparencyPS.cso")))
			if (!m_PTransparencyPS.Create(L"PTransparencyPS.cso"))
				throw;
		if (!m_PSGShader.Create(AEN_OUTPUT_DIR_WSTR(L"GeometryParticleShader.cso")))
			if (!m_PSGShader.Create(L"GeometryParticleShader.cso"))
				throw;
		if (!m_PSCShader.Create(AEN_OUTPUT_DIR_WSTR(L"ComputeParticleShader.cso")))
			if (!m_PSCShader.Create(L"ComputeParticleShader.cso"))
				throw;
		if (!m_PSPShader.Create(AEN_OUTPUT_DIR_WSTR(L"PixelParticleShader.cso")))
			if (!m_PSPShader.Create(L"PixelParticleShader.cso"))
				throw;
		if (!m_PostPatricleCS.Create(AEN_OUTPUT_DIR_WSTR(L"PostParticleCS.cso")))
			if (!m_PostPatricleCS.Create(L"PostParticleCS.cso"))
				throw;
		m_PSLayout.Create(m_PSVShader);

		if(!m_transparencyPS.Create(AEN_OUTPUT_DIR_WSTR(L"TransparencyPS.cso")))
			if(!m_transparencyPS.Create(L"TransparencyPS.cso"))
				throw;

		if(!m_frustumGridCS.Create(AEN_OUTPUT_DIR_WSTR(L"FrustumGridCS.cso")))
			if(!m_frustumGridCS.Create(L"FrustumGridCS.cso"))
				throw;

		m_UAVFinal.Create(m_window.GetSize(), DXGI_FORMAT_R32G32B32A32_FLOAT);
		m_opaqueLayout.Create(m_opaqueVS);
		m_UAVBackBuffer.Create(m_backBuffer);

		m_dispatchInfo.GetData().windowSize.x = m_window.GetSize().x;
		m_dispatchInfo.GetData().windowSize.y = m_window.GetSize().y;
		m_dispatchInfo.GetData().numThreads.x = (int)std::ceil((float)m_window.GetSize().x / 16.f);
		m_dispatchInfo.GetData().numThreads.y = (int)std::ceil((float)m_window.GetSize().y / 16.f);
		m_dispatchInfo.GetData().threadGroups.x = (int)std::ceil((float)m_window.GetSize().x / 16.f);
		m_dispatchInfo.GetData().threadGroups.y = (int)std::ceil((float)m_window.GetSize().y / 16.f);
		m_dispatchInfo.GetData().avarageLights = m_avarageLights;
		m_dispatchInfo.UpdateBuffer();

		m_dispatchGroups.x = (int)std::ceil((float)m_window.GetSize().x / 32.f);
		m_dispatchGroups.y = (int)std::ceil((float)m_window.GetSize().y / 32.f);

		uint32_t size = m_dispatchInfo.GetData().numThreads.x * m_dispatchInfo.GetData().numThreads.y;
		m_lIndex.Create(sizeof(uint32_t), m_avarageLights * size);
		m_lGrid.Create(m_dispatchInfo.GetData().numThreads, DXGI_FORMAT_R32G32_UINT);
		m_frustumGrid.Create(128u, size);
	}

	void Renderer::Culling()
	{
		//--- Quadtree Culling ---//
		if (GlobalSettings::m_pQuadtree)
		{
			GlobalSettings::m_pQuadtree->Update(*this, m_drawTable);
		}
		else
		{
			for (uint32_t i = 0u; i < 7u; i++)
			{
				for (auto& k : ComponentHandler::m_meshLayer[i])
				{
					if (k.second->FrustumCull(*this))
					{
						m_drawTable[i].emplace_back(k.second);
					}
				}
			}
		}
		
	}

	void Renderer::Render() {
		
		RenderSystem::SetRasteriserState(m_rasterizerState);
		RenderSystem::ClearRenderTargetView(m_backBuffer, Color(0.f, 0.f, 0.f, 0.f));

		// BackGround

		m_cbBGColor.GetData() = GlobalSettings::GetBGColor();
		m_cbBGColor.UpdateBuffer();

		// Light

		uint32_t lightIndex = 0u;
		for(auto& i : ComponentHandler::m_lights) {
			i.second->m_light.m_pos = (ComponentHandler::TranslationExist(i.first)) ? ComponentHandler::GetTranslation(i.first).GetPos() : Vec3f::zero;
			i.second->m_light.m_dir = (ComponentHandler::RotationExist(i.first)) ? Transform(ComponentHandler::GetRotation(i.first).GetTranform(), Vec3f(0.f, 1.f, 0.f)) : Vec3f(0.f, 1.f, 0.f);
			m_sbLight.GetData(lightIndex) = i.second->m_light;
			lightIndex++;
		}

		m_sbLight.UpdateBuffer();
		m_cbLightCount.GetData() = (uint32_t)ComponentHandler::m_lights.size();
		m_cbLightCount.UpdateBuffer();

		// Layered Rendering

		for(uint32_t i = 0u; i < 7u; i++)
			if (m_drawTable[i].size() > 0) {
				// Light Camera

				UpdateLCamBuffer();

				// Shadow Pass

				RenderSystem::SetViewPort(m_viewPort2);
				RenderSystem::UnBindRenderTargets(1u);
				RenderSystem::BindRenderTargetView(m_shadowMap);
				RenderSystem::SetDepthStencilState(m_offStencil, 0xFF);

				for (auto& k : m_drawTable[i]) {
					k->DepthDraw(*this);
				}

				// Camera

				UpdateCamBuffer();

#ifdef _DEBUG
				if (GlobalSettings::GetUseDebugCam())
					UpdateDebugCamBuffer();
#endif

				// Pre Depth Pass
				RenderSystem::SetViewPort(m_viewPort);
				RenderSystem::UnBindRenderTargets(1u);
				RenderSystem::BindRenderTargetView(m_depthMap);
				
				for (auto& k : m_drawTable[i]) {
					k->DepthDraw(*this);
				}

				// Light Cull Pass

				RenderSystem::UnBindRenderTargets(1u);
				
				RenderSystem::BindShaderResourceView<CShader>(0u, m_frustumGrid);
				m_sbLight.BindSRV<CShader>(1u);
				RenderSystem::BindShaderResourceView<CShader>(2u, m_depthMap);
				RenderSystem::BindUnOrderedAccessView(0u, m_lIndex);
				RenderSystem::BindUnOrderedAccessView(1u, m_lGrid);
				RenderSystem::BindShader(m_lightCullCS);
				m_dispatchInfo.BindBuffer<CShader>(0u);
				m_cbLightCount.BindBuffer<CShader>(1u);
				m_cbTransform.BindBuffer<CShader>(2u);
				
				RenderSystem::Dispatch(m_dispatchInfo.GetData().threadGroups, 1u);
				
				RenderSystem::UnBindShader<CShader>();
				RenderSystem::UnBindUnOrderedAccessViews(0u, 3u);
				RenderSystem::UnBindShaderResources<CShader>(0u, 4u);

				// Draw pass

				for (auto& k : m_drawTable[i]) {
					k->Draw(*this);
				}

				m_drawTable[i].clear();
				RenderSystem::ClearDepthStencilView(m_depthMap, true, false);
				RenderSystem::ClearDepthStencilView(m_shadowMap, true, false);
			}


		// Present
		RenderSystem::Present((UINT)Aen::GlobalSettings::GetVSync());
		RenderSystem::ClearState();
	}

	void Renderer::UpdateCamBuffer() {
		if(GlobalSettings::m_pMainCamera) {
			Entity* pCam = GlobalSettings::m_pMainCamera;

			Vec3f pos = pCam->GetPos();
			Vec3f rot = pCam->GetRot();

			pCam->GetComponent<Camera>().UpdateView(pos, rot);

			m_cbCamera.GetData().pos = pos;
			m_cbCamera.GetData().fDir = pCam->GetComponent<Camera>().GetForward();
			m_cbCamera.GetData().uDir = pCam->GetComponent<Camera>().GetUp();
			m_cbCamera.UpdateBuffer();

			#ifdef _DEBUG
			if (Aen::Input::KeyDown(Aen::Key::M))
				m_toggleView = !m_toggleView;

			if (m_toggleView)
			{
				m_cbTransform.GetData().m_vMat = MatViewLH(Aen::Vec3f(0.f, 0.f, 0.f), Aen::Vec3f(0, 1.f, -1.f), Aen::Vec3f(0.f, 1.f, 0.f)).Transposed();
				m_cbTransform.GetData().m_pMat = MatPerspective<float>(90.f, m_window.GetAspectRatio(), 0.01f, 200.f).Transposed();
				m_cbTransform.GetData().m_ivMat = m_cbTransform.GetData().m_vMat.Inverse();
				m_cbTransform.GetData().m_ipMat = m_cbTransform.GetData().m_pMat.Inverse();
			}
			else {
				m_cbTransform.GetData().m_vMat = pCam->GetComponent<Camera>().GetView().Transposed();
				m_cbTransform.GetData().m_pMat = pCam->GetComponent<Camera>().GetProjecton().Transposed();
				m_cbTransform.GetData().m_ivMat = m_cbTransform.GetData().m_vMat.Inverse();
				m_cbTransform.GetData().m_ipMat = m_cbTransform.GetData().m_pMat.Inverse();
			}
			#else
			m_cbTransform.GetData().m_vMat = pCam->GetComponent<Camera>().GetView().Transposed();
			m_cbTransform.GetData().m_pMat = pCam->GetComponent<Camera>().GetProjecton().Transposed();
			m_cbTransform.GetData().m_ivMat = m_cbTransform.GetData().m_vMat.Inverse();
			m_cbTransform.GetData().m_ipMat = m_cbTransform.GetData().m_pMat.Inverse();
			#endif

		} else {
			m_cbTransform.GetData().m_vMat = Mat4f::identity;
			m_cbTransform.GetData().m_pMat = Mat4f::identity;
			m_cbTransform.GetData().m_ivMat = Mat4f::identity;
			m_cbTransform.GetData().m_ipMat = Mat4f::identity;
		}
	}

	void Renderer::UpdateLCamBuffer() {
		if(GlobalSettings::m_pLightCamera) {
			Entity* pCam = GlobalSettings::m_pLightCamera;

			Vec3f pos = pCam->GetPos();
			Vec3f rot = pCam->GetRot();

			pCam->GetComponent<Camera>().UpdateView(pos, rot);

			m_cbCamera.GetData().pos = pos;
			m_cbCamera.GetData().fDir = pCam->GetComponent<Camera>().GetForward();
			m_cbCamera.GetData().uDir = pCam->GetComponent<Camera>().GetUp();
			m_cbCamera.UpdateBuffer();

			m_cbTransform.GetData().m_vMat = pCam->GetComponent<Camera>().GetView().Transposed();
			m_cbTransform.GetData().m_pMat = pCam->GetComponent<Camera>().GetProjecton().Transposed();
			m_cbTransform.GetData().m_ivMat = m_cbTransform.GetData().m_vMat.Inverse();
			m_cbTransform.GetData().m_ipMat = m_cbTransform.GetData().m_pMat.Inverse();
			m_cbTransform.GetData().m_lvpMat = pCam->GetComponent<Camera>().GetVPMatrix().Transposed();
		} else {
			m_cbTransform.GetData().m_vMat = Mat4f::identity;
			m_cbTransform.GetData().m_pMat = Mat4f::identity;
			m_cbTransform.GetData().m_ivMat = Mat4f::identity;
			m_cbTransform.GetData().m_ipMat = Mat4f::identity;
			m_cbTransform.GetData().m_lvpMat = Mat4f::identity;
		}
	}
#ifdef _DEBUG
	void Renderer::UpdateDebugCamBuffer()
	{
		if (GlobalSettings::m_pDebugCamera) {
			Entity* pCam = GlobalSettings::m_pDebugCamera;

			Vec3f pos = pCam->GetPos();
			Vec3f rot = pCam->GetRot();

			pCam->GetComponent<Camera>().UpdateView(pos, rot);

			m_cbCamera.GetData().pos = pos;
			m_cbCamera.GetData().fDir = pCam->GetComponent<Camera>().GetForward();
			m_cbCamera.GetData().uDir = pCam->GetComponent<Camera>().GetUp();
			m_cbCamera.UpdateBuffer();

			m_cbTransform.GetData().m_vMat = pCam->GetComponent<Camera>().GetView().Transposed();
			m_cbTransform.GetData().m_pMat = pCam->GetComponent<Camera>().GetProjecton().Transposed();
			m_cbTransform.GetData().m_ivMat = m_cbTransform.GetData().m_vMat.Inverse();
			m_cbTransform.GetData().m_ipMat = m_cbTransform.GetData().m_pMat.Inverse();
			m_cbTransform.GetData().m_lvpMat = pCam->GetComponent<Camera>().GetVPMatrix().Transposed();
		}
		else {
			m_cbTransform.GetData().m_vMat = Mat4f::identity;
			m_cbTransform.GetData().m_pMat = Mat4f::identity;
			m_cbTransform.GetData().m_ivMat = Mat4f::identity;
			m_cbTransform.GetData().m_ipMat = Mat4f::identity;
			m_cbTransform.GetData().m_lvpMat = Mat4f::identity;
		}
	}
#endif
}
