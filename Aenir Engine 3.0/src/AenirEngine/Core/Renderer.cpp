#include"PCH.h"
#include"Renderer.h"
#include"GlobalSettings.h"

namespace Aen {

	Renderer::Renderer(Window& window)
		:m_window(window), m_cbTransform(), m_backBuffer(), 
		m_viewPort(), m_depthStencil(window), m_rasterizerState(FillMode::Solid, CullMode::Front) {}

	void Renderer::Initialize() {

		ZeroMemory(&m_viewPort, sizeof(D3D11_VIEWPORT));

		m_viewPort.TopLeftX = 0.f;
		m_viewPort.TopLeftY = 0.f;
		m_viewPort.Width = static_cast<float>(m_window.GetSize().x);
		m_viewPort.Height = static_cast<float>(m_window.GetSize().y);
		m_viewPort.MinDepth = 0.f;
		m_viewPort.MaxDepth = 1.f;

		RenderSystem::SetViewPort(m_viewPort);
		RenderSystem::SetPrimitiveTopology(Topology::TRIANGLELIST);
		RenderSystem::SetRasteriserState(m_rasterizerState);
		RenderSystem::BindRenderTargetView(m_backBuffer, m_depthStencil);
		RenderSystem::SetDepthStencilState(m_depthStencil);
	}

	void Renderer::Draw() {

		RenderSystem::ClearDepthStencilView(m_depthStencil);
		RenderSystem::ClearRenderTargetView(m_backBuffer, Color(0.03f, 0.03f, 0.05f, 1.f));

		m_cbTransform.BindBuffer<VShader>(0);
		if(GlobalSettings::m_pMainCamera) {

			Entity* pCam = GlobalSettings::m_pMainCamera;

			Vec3f pos = pCam->GetPos();
			Vec3f rot = pCam->GetRot();

			pCam->GetComponent<Camera>().UpdateView(pos, rot);
			
			m_cbTransform.GetData().m_vpMat = pCam->GetComponent<Camera>().GetVPMatrix().Transposed();
		} else
			m_cbTransform.GetData().m_vpMat = Mat4f::identity;

		for(auto& i : ComponentHandler::m_mesheInstances) { 
			uint32_t id = i.first;
			Mesh* pMesh = i.second->m_mesh;
			Material* pMaterial = (pMesh && ComponentHandler::MaterialInstanceExist(id)) ? ComponentHandler::GetMaterialInstance(id).m_pMaterial : nullptr;

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

			if(pMaterial) {

				RenderSystem::SetInputLayout(pMaterial->m_pShader->m_iLayout);

				for(UINT i = 0; i < 4; i++)
					if(pMaterial->m_textures[i]) {
						RenderSystem::BindShaderResourceView<PShader>(i, pMaterial->m_textures[i]->m_shaderResource); // causing memory leak
					}

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