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
					
					RenderSystem::SetInputLayout(renderer.m_opaqueLayout);

					RenderSystem::BindShader<VShader>(renderer.m_opaqueVS);
					RenderSystem::BindShader<PShader>(pMaterial->m_pShaderModel->m_PShader);

					uint32_t* slots = pMaterial->m_pShaderModel->m_slots;

					for(UINT k = 0; k < 4; k++)
						if(pMaterial->m_textures[k] && slots[k] != UINT_MAX) {
							RenderSystem::UnBindShaderResources<PShader>(slots[k], 1u);
							RenderSystem::BindShaderResourceView<PShader>(slots[k], pMaterial->m_textures[k]->m_shaderResource);
							renderer.m_cbUseTexture.GetData()[k] = (int)true;
						} else
							renderer.m_cbUseTexture.GetData()[k] = (int)false;

					pMaterial->m_dBuffer.UpdateBuffer();
					renderer.m_cbUseTexture.UpdateBuffer();

					if(slots[4] != UINT_MAX)	renderer.m_cbTransform.BindBuffer<PShader>(			slots[4]	);
					if(slots[5] != UINT_MAX)	renderer.m_cbLightCount.BindBuffer<PShader>(		slots[5]	);
					if(slots[6] != UINT_MAX)	renderer.m_cbCamera.BindBuffer<PShader>(			slots[6]	);
					if(slots[7] != UINT_MAX)	renderer.m_cbUseTexture.BindBuffer<PShader>(		slots[7]	);
					if(slots[8] != UINT_MAX)	renderer.m_sbLight.BindSRV<PShader>(				slots[8]	);
					if(slots[9] != UINT_MAX)	RenderSystem::BindShaderResourceView<PShader>(		slots[9],	renderer.m_lGrid);
					if(slots[10] != UINT_MAX)	RenderSystem::BindShaderResourceView<PShader>(		slots[10],	renderer.m_lIndex);
					if(slots[11] != UINT_MAX)	pMaterial->m_dBuffer.BindBuffer<PShader>(			slots[11]	);

					RenderSystem::BindSamplers<PShader>(pMaterial->m_pShaderModel->m_samplerData.first, pMaterial->m_pShaderModel->m_samplerData.second);

					RenderSystem::ClearRenderTargetView(pMaterial->m_pShaderModel->m_gBuffer, Color(0.f, 0.f, 0.f, 0.f));
					RenderSystem::BindRenderTargetView(pMaterial->m_pShaderModel->m_gBuffer, renderer.m_depthMap);

					RenderSystem::ClearDepthStencilView(renderer.m_depthMap, false, true);
					RenderSystem::SetDepthStencilState(renderer.m_writeStencil, 0xFF);
					m_pMesh->m_vertices.Draw(m_pMesh->m_partitions[i].size, m_pMesh->m_partitions[i].offset);


					// Per Object Post Process Pass

					RenderSystem::UnBindShaderResources<PShader>(slots[9], 1u);
					RenderSystem::UnBindShaderResources<PShader>(slots[10], 1u);
					RenderSystem::UnBindRenderTargets(pMaterial->m_pShaderModel->m_gBuffer.GetCount());
					if(slots[12] != UINT_MAX)	renderer.m_cbTransform.BindBuffer<CShader>(			slots[12]	);
					if(slots[13] != UINT_MAX)	renderer.m_cbLightCount.BindBuffer<CShader>(		slots[13]	);
					if(slots[14] != UINT_MAX)	renderer.m_cbCamera.BindBuffer<CShader>(			slots[14]	);
					if(slots[15] != UINT_MAX)	renderer.m_cbUseTexture.BindBuffer<CShader>(		slots[15]	);
					if(slots[16] != UINT_MAX)	renderer.m_sbLight.BindSRV<CShader>(				slots[16]	);
					if(slots[17] != UINT_MAX)	pMaterial->m_dBuffer.BindBuffer<CShader>(			slots[17]	);

					RenderSystem::BindShaderResourceView<CShader>(0u, pMaterial->m_pShaderModel->m_gBuffer);
					RenderSystem::BindUnOrderedAccessView(0u, renderer.m_UAVBackBuffer);
					RenderSystem::BindShader(pMaterial->m_pShaderModel->m_CShader);

					RenderSystem::Dispatch(renderer.m_dispatchGroups, 1u);

					RenderSystem::UnBindShader<CShader>();
					RenderSystem::UnBindUnOrderedAccessViews(0u, 1u);
					RenderSystem::UnBindShaderResources<CShader>(0u, pMaterial->m_pShaderModel->m_gBuffer.GetCount());
				}
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
				RenderSystem::SetInputLayout(renderer.m_opaqueLayout);
				RenderSystem::BindShader<VShader>(renderer.m_opaqueVS);
				RenderSystem::UnBindShader<PShader>();

				renderer.m_cbTransform.BindBuffer<VShader>(0);
			}

			m_pMesh->m_vertices.BindBuffer();
			m_pMesh->m_vertices.Draw();
		}
	}
}