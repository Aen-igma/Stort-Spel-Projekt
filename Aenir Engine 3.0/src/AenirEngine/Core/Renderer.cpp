#include"PCH.h"
#include"Renderer.h"
#include"GlobalSettings.h"

namespace Aen {

	Renderer::Renderer(Window& window)
		:m_window(window), m_cbTransform(), m_backBuffer(), 
		m_viewPort(), m_depthStencil(window), m_rasterizerState(FillMode::Solid, CullMode::None) {}

	void Renderer::Initialize() {

		ZeroMemory(&m_viewPort, sizeof(D3D11_VIEWPORT));

		m_viewPort.TopLeftX = 0;
		m_viewPort.TopLeftY = 0;
		m_viewPort.Width = static_cast<float>(m_window.GetSize().x);
		m_viewPort.Height = static_cast<float>(m_window.GetSize().y);
		m_viewPort.MinDepth = 0;
		m_viewPort.MaxDepth = 1;

		m_depthStencil.Create(m_window);
		RenderSystem::SetViewPort(m_viewPort);
		RenderSystem::SetPrimitiveTopology(Topology::TRIANGLELIST);
	}

	void Renderer::Draw() {

		RenderSystem::ClearDepthStencilView(m_depthStencil);
		RenderSystem::ClearRenderTargetView(m_backBuffer, Color(0.03f, 0.03f, 0.05f, 1.f));

		RenderSystem::SetDepthStencilState(m_depthStencil);
		RenderSystem::SetRasteriserState(m_rasterizerState);

		RenderSystem::BindRenderTargetView(m_backBuffer, m_depthStencil);
		m_cbTransform.BindBuffer<VShader>(0);

		if(GlobalSettings::m_pMainCamera) {

			Entity* pCam = GlobalSettings::m_pMainCamera;

			Vec3f pos = pCam->GetPos();
			Vec3f rot = pCam->GetRot();

			pCam->GetComponent<Camera>().UpdateView(pos, rot);
			
			m_cbTransform.GetData().m_vpMat = pCam->GetComponent<Camera>().GetVPMatrix().Transposed();
		} else
			m_cbTransform.GetData().m_vpMat = Mat4f::identity;

		for(auto& i : MeshIHandler::m_mesheInstances) {
			uint32_t id = i.first;
			Mesh* pMesh = i.second->m_mesh;
			Material* pMaterial = (pMesh && MaterialIHandler::MaterialInstanceExist(id)) ? MaterialIHandler::GetMaterialInstance(id).m_pMaterial : nullptr;

			RenderSystem::UnBindShaderResources<PShader>(0, 4);

			Mat4f pos = (TransformHandler::TranslationExist(id)) ? TransformHandler::GetTranslation(id).GetTranform() : Mat4f::identity;
			Mat4f rot = (TransformHandler::RotationExist(id)) ? TransformHandler::GetRotation(id).GetTranform() : Mat4f::identity;
			Mat4f scale = (TransformHandler::ScaleExist(id)) ? TransformHandler::GetScale(id).GetTranform() : Mat4f::identity;
			m_cbTransform.GetData().m_mdlMat = (scale * rot * pos).Transposed();
			m_cbTransform.UpdateBuffer();

			if(pMaterial) {

				RenderSystem::SetInputLayout(pMaterial->m_pShader->m_iLayout);

				//for(UINT i = 0; i < 4; i++)
				//	if(pMaterial->m_textures[i]) {
				//		RenderSystem::BindShaderResourceView<PShader>(i, pMaterial->m_textures[i]->m_shaderResource); // causing memory leak
				//	}

				RenderSystem::BindShader<VShader>(pMaterial->m_pShader->m_VShader);
				RenderSystem::BindShader<PShader>(pMaterial->m_pShader->m_PShader);

				for(uint32_t i = 0; i < pMaterial->m_dBuffers.size(); i++) {
					UINT slot = pMaterial->m_pShader->m_dbLayouts[i].first;
					pMaterial->m_dBuffers[i]->BindBuffer<PShader>(slot);
					pMaterial->m_dBuffers[i]->UpdateBuffer();
				}

				for(auto& i : pMaterial->m_pShader->m_samplerDatas) {
					UINT slot = i.first;
					RenderSystem::BindSamplers<PShader>(slot, i.second);
				}
			}

			if(pMesh) {
				pMesh->m_vertices.BindVBuffer();
				pMesh->m_vertices.Draw();
			}
		}

		RenderSystem::Present();
	}
}