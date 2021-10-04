#include"PCH.h"
#include"MeshInstance.h"
#include"..\..\ComponentHandler.h"
#include"Core/Renderer.h"

namespace Aen {
	MeshInstance::~MeshInstance() {
		for(auto& i : m_pMaterials)
			i = nullptr;

		m_pMaterials.clear();
		m_pMesh = nullptr;
	}

	MeshInstance::MeshInstance()
		:m_pMesh(nullptr), m_pMaterials(1u, &Resource::GetMaterial("DefaultMaterial")) {}

	MeshInstance::MeshInstance(Mesh& mesh)
		:m_pMesh(&mesh) {}

	void MeshInstance::RemoveMesh() {
		for(auto& i : m_pMaterials)
			i = nullptr;
		m_pMaterials.clear();
		m_pMesh = nullptr;

		m_pMaterials.reserve(1u);
		m_pMaterials.resize(1u, &Resource::GetMaterial("DefaultMaterial"));
	}

	void MeshInstance::SetMesh(Mesh& mesh) {
		m_pMesh = &mesh;

		if(m_pMesh->m_meshMaterialName.size() > 0) {
			m_pMaterials.reserve(m_pMesh->m_meshMaterialName.size());
			m_pMaterials.resize(m_pMesh->m_meshMaterialName.size(), &Resource::GetMaterial("DefaultMaterial"));
		}
	}

	void MeshInstance::PrintMaterialSlots() {
		if(m_pMesh)
			m_pMesh->PrintMaterialSlots();
	}

	void MeshInstance::SetMaterial(Material& material) {
		m_pMaterials[0] = &material;
	}

	void MeshInstance::SetMaterial(const std::string& materialSlotName, Material& material) {
		if(m_pMesh->m_meshMaterialName.count(materialSlotName) == 0) throw;
		m_pMaterials[m_pMesh->m_meshMaterialName.at(materialSlotName)] = &material;
	}

	void MeshInstance::SetMaterial(const std::string& materialSlotName, const std::string& materialName) {
		if(m_pMesh->m_meshMaterialName.count(materialSlotName) == 0) throw;
		if(!Resource::MaterialExist(materialName)) throw;
		m_pMaterials[m_pMesh->m_meshMaterialName.at(materialSlotName)] = &Resource::GetMaterial(materialName);
	}

	void MeshInstance::Draw(Renderer& renderer, const uint32_t& id, const uint32_t& layer) {

		if(m_pMesh) {

			// Transform

			if(ComponentHandler::RigidExist(id))
				renderer.m_cbTransform.GetData().m_mdlMat = (EntityHandler::GetEntity(id).GetScaleMat() * ComponentHandler::GetRigid(id).GetTransform()).Transposed();
			else
				renderer.m_cbTransform.GetData().m_mdlMat = EntityHandler::GetEntity(id).GetTransformation().Transposed();
			renderer.m_cbTransform.UpdateBuffer();

			// Mesh and Material

			for(uint32_t i = 0; i < m_pMesh->m_partitions.size(); i++) {

				m_pMesh->m_vertices.BindBuffer();

				// Opaque pass

				uint32_t materialIndex = m_pMesh->m_partitions[i].materialIndex;
				Material* pMaterial = (m_pMaterials[materialIndex]) ? m_pMaterials[materialIndex] : nullptr;
				if(pMaterial) {

					RenderSystem::UnBindShaderResources<PShader>(0u, pMaterial->m_pShaderModel->m_gBuffer.GetCount());
					RenderSystem::SetInputLayout(pMaterial->m_pShaderModel->m_iLayoutPass1);

					RenderSystem::BindShader<VShader>(pMaterial->m_pShaderModel->m_VShaderPass1);
					RenderSystem::BindShader<PShader>(pMaterial->m_pShaderModel->m_PShaderPass1);

					uint32_t* slots = pMaterial->m_pShaderModel->m_slotsPass1;

					if(slots[0] != UINT_MAX) renderer.m_cbTransform.BindBuffer<VShader>(slots[0]);
					if(slots[1] != UINT_MAX) renderer.m_cbLightCount.BindBuffer<VShader>(slots[1]);
					if(slots[2] != UINT_MAX) renderer.m_cbCamera.BindBuffer<VShader>(slots[2]);
					if(slots[3] != UINT_MAX) renderer.m_cbUseTexture.BindBuffer<VShader>(slots[3]);
					if(slots[4] != UINT_MAX) renderer.m_sbLight.BindSRV<VShader>(slots[4]);

					if(slots[5] != UINT_MAX) renderer.m_cbTransform.BindBuffer<PShader>(slots[5]);
					if(slots[6] != UINT_MAX) renderer.m_cbLightCount.BindBuffer<PShader>(slots[6]);
					if(slots[7] != UINT_MAX) renderer.m_cbCamera.BindBuffer<PShader>(slots[7]);
					if(slots[8] != UINT_MAX) renderer.m_cbUseTexture.BindBuffer<PShader>(slots[8]);
					if(slots[9] != UINT_MAX) renderer.m_sbLight.BindSRV<PShader>(slots[9]);

					for(UINT k = 0; k < 4; k++)
						if(pMaterial->m_textures[k] && slots[10 + k] != UINT_MAX) {
							RenderSystem::BindShaderResourceView<PShader>(slots[10 + k], pMaterial->m_textures[k]->m_shaderResource);
							renderer.m_cbUseTexture.GetData()[k] = (int)true;
						} else
							renderer.m_cbUseTexture.GetData()[k] = (int)false;

						pMaterial->m_dBuffer.UpdateBuffer();
						if(slots[14] != UINT_MAX) pMaterial->m_dBuffer.BindBuffer<PShader>(slots[14]);
						if(slots[15] != UINT_MAX) pMaterial->m_dBuffer.BindBuffer<PShader>(slots[15]);

						renderer.m_cbUseTexture.UpdateBuffer();

						RenderSystem::BindSamplers<PShader>(pMaterial->m_pShaderModel->m_samplerDataPass1.first, pMaterial->m_pShaderModel->m_samplerDataPass1.second);

						RenderSystem::UnBindRenderTargets(1u);
						RenderSystem::ClearRenderTargetView(pMaterial->m_pShaderModel->m_gBuffer, Color(0.f, 0.f, 0.f, 0.f));
						RenderSystem::BindRenderTargetView(pMaterial->m_pShaderModel->m_gBuffer, renderer.m_depthMap);

						if(slots[16] != UINT_MAX) RenderSystem::BindShaderResourceView<PShader>(slots[16], renderer.m_lGrid);
						if(slots[17] != UINT_MAX) RenderSystem::BindShaderResourceView<PShader>(slots[17], renderer.m_lIndex);
				}

				RenderSystem::ClearDepthStencilView(renderer.m_depthMap, false, true);
				RenderSystem::SetDepthStencilState(renderer.m_writeStencil, 0xFF);
				m_pMesh->m_vertices.Draw(m_pMesh->m_partitions[i].size, m_pMesh->m_partitions[i].offset);

				// Per Object Post Process Pass

				if(pMaterial) {

					RenderSystem::BindShader<VShader>(pMaterial->m_pShaderModel->m_VShaderPass2);
					RenderSystem::BindShader<PShader>(pMaterial->m_pShaderModel->m_PShaderPass2);

					std::optional<D3D11_SHADER_INPUT_BIND_DESC> bDescShaderModelVS = pMaterial->m_pShaderModel->m_VSReflectPass2.GetBindDescByName(pMaterial->m_pShaderModel->m_bufferName);
					std::optional<D3D11_SHADER_INPUT_BIND_DESC> bDescShaderModelPS = pMaterial->m_pShaderModel->m_PSReflectPass2.GetBindDescByName(pMaterial->m_pShaderModel->m_bufferName);

					if(bDescShaderModelVS) pMaterial->m_dBuffer.BindBuffer<PShader>(bDescShaderModelVS.value().BindPoint);
					if(bDescShaderModelPS) pMaterial->m_dBuffer.BindBuffer<PShader>(bDescShaderModelPS.value().BindPoint);

					uint32_t* slots = pMaterial->m_pShaderModel->m_slotsPass2;

					if(slots[0] != UINT_MAX) renderer.m_cbTransform.BindBuffer<VShader>(slots[0]);
					if(slots[1] != UINT_MAX) renderer.m_cbLightCount.BindBuffer<VShader>(slots[1]);
					if(slots[2] != UINT_MAX) renderer.m_cbCamera.BindBuffer<VShader>(slots[2]);
					if(slots[3] != UINT_MAX) renderer.m_cbUseTexture.BindBuffer<VShader>(slots[3]);
					if(slots[4] != UINT_MAX) renderer.m_sbLight.BindSRV<VShader>(slots[4]);

					if(slots[5] != UINT_MAX) renderer.m_cbTransform.BindBuffer<PShader>(slots[5]);
					if(slots[6] != UINT_MAX) renderer.m_cbLightCount.BindBuffer<PShader>(slots[6]);
					if(slots[7] != UINT_MAX) renderer.m_cbCamera.BindBuffer<PShader>(slots[7]);
					if(slots[8] != UINT_MAX) renderer.m_cbUseTexture.BindBuffer<PShader>(slots[8]);
					if(slots[9] != UINT_MAX) renderer.m_sbLight.BindSRV<PShader>(slots[9]);

					if(slots[14] != UINT_MAX) pMaterial->m_dBuffer.BindBuffer<PShader>(slots[14]);
					if(slots[15] != UINT_MAX) pMaterial->m_dBuffer.BindBuffer<PShader>(slots[15]);

					RenderSystem::BindSamplers<PShader>(pMaterial->m_pShaderModel->m_samplerDataPass2.first, pMaterial->m_pShaderModel->m_samplerDataPass2.second);
					RenderSystem::SetInputLayout(pMaterial->m_pShaderModel->m_iLayoutPass2);
					RenderSystem::UnBindRenderTargets(pMaterial->m_pShaderModel->m_gBuffer.GetCount());
					RenderSystem::BindShaderResourceView<PShader>(0, pMaterial->m_pShaderModel->m_gBuffer);

					if(slots[16] != UINT_MAX) RenderSystem::BindShaderResourceView<PShader>(slots[16], renderer.m_lGrid);
					if(slots[17] != UINT_MAX) RenderSystem::BindShaderResourceView<PShader>(slots[17], renderer.m_lIndex);
				}

				RenderSystem::BindRenderTargetView(renderer.m_layerBuffer.GetRtv(layer), renderer.m_depthMap);
				RenderSystem::SetDepthStencilState(renderer.m_maskStencil, 0xFF);

				renderer.m_screenQuad.Draw();
			}
		}
	}

	void MeshInstance::DepthDraw(Renderer& renderer, const uint32_t& id, const uint32_t& layer) {

		if(m_pMesh) {

			if(ComponentHandler::RigidExist(id))
				renderer.m_cbTransform.GetData().m_mdlMat = (EntityHandler::GetEntity(id).GetScaleMat() * ComponentHandler::GetRigid(id).GetTransform()).Transposed();
			else
				renderer.m_cbTransform.GetData().m_mdlMat = EntityHandler::GetEntity(id).GetTransformation().Transposed();
			renderer.m_cbTransform.UpdateBuffer();

			Material* pMaterial = (m_pMesh && m_pMaterials[0]) ? m_pMaterials[0] : nullptr;
			if(pMaterial) {
				RenderSystem::SetInputLayout(pMaterial->m_pShaderModel->m_iLayoutPass1);
				RenderSystem::BindShader<VShader>(pMaterial->m_pShaderModel->m_VShaderPass1);
				RenderSystem::UnBindShader<PShader>();

				uint32_t* slots = pMaterial->m_pShaderModel->m_slotsPass1;
				if(slots[0] != UINT_MAX) renderer.m_cbTransform.BindBuffer<VShader>(slots[0]);
				if(slots[1] != UINT_MAX) renderer.m_cbLightCount.BindBuffer<VShader>(slots[1]);
				if(slots[2] != UINT_MAX) renderer.m_cbCamera.BindBuffer<VShader>(slots[2]);
				if(slots[3] != UINT_MAX) renderer.m_cbUseTexture.BindBuffer<VShader>(slots[3]);
				if(slots[4] != UINT_MAX) renderer.m_sbLight.BindSRV<VShader>(slots[4]);
			}

			m_pMesh->m_vertices.BindBuffer();
			m_pMesh->m_vertices.Draw();
		}
	}
}