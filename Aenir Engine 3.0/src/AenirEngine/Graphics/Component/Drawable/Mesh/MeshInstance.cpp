#include"PCH.h"
#include"MeshInstance.h"
#include"..\..\ComponentHandler.h"
#include"Core/Renderer.h"
#include"Core/GlobalSettings.h"

namespace Aen {
	MeshInstance::~MeshInstance() {
		for(auto& i : m_pMaterials)
			i = nullptr;

		m_pMaterials.clear();
		m_pMesh = nullptr;
	}

	MeshInstance::MeshInstance(const size_t& m_id)
		:Drawable(m_id), m_pMesh(nullptr), m_pMaterials(1u, GlobalSettings::GetDefaultMaterial()) {}

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
			m_pMaterials.resize(m_pMesh->m_meshMaterialName.size(), GlobalSettings::GetDefaultMaterial());
		}
	}

	void MeshInstance::SetMesh(const std::string& meshName) {
		if(!Resource::MeshExist(meshName)) throw;
		m_pMesh = &Resource::GetMesh(meshName);

		if(m_pMesh->m_meshMaterialName.size() > 0) {
			m_pMaterials.reserve(m_pMesh->m_meshMaterialName.size());
			m_pMaterials.resize(m_pMesh->m_meshMaterialName.size(), GlobalSettings::GetDefaultMaterial());
		}
	}

	void MeshInstance::PrintMaterialSlots() {
		if(m_pMesh)
			m_pMesh->PrintMaterialSlots();
	}

	void MeshInstance::SetMaterial(Material& material) {
		m_pMaterials[0] = &material;
	}

	void MeshInstance::SetMaterial(const std::string& materialName) {
		if(!Resource::MaterialExist(materialName)) throw;
		m_pMaterials[0] = &Resource::GetMaterial(materialName);
	}

	void MeshInstance::SetMaterial(const UINT& index, Material& material) {
		m_pMaterials[index] = &material;
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

	void MeshInstance::Draw(Renderer& renderer, const uint32_t& layer) {

		if(m_pMesh) {

			// Transform

			Mat4f m = EntityHandler::GetEntity(m_id).GetTransformation();
			renderer.m_cbTransform.GetData().m_mdlMat = m.Transposed();
			renderer.m_cbTransform.UpdateBuffer();

			//DirectX::BoundingOrientedBox box(m_pMesh->m_obb);
			DirectX::BoundingOrientedBox box(m_pMesh->m_aabb.Center, m_pMesh->m_aabb.Extents, DirectX::XMFLOAT4(0, 0, 0, 1));

			box.Transform(box, m.smMat);

			if(GlobalSettings::GetMainCamera())
				if(box.Intersects(GlobalSettings::GetMainCamera()->GetComponent<Camera>().GetFrustum())) {

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

								if(slots[4] != UINT_MAX)	renderer.m_cbTransform.BindBuffer<PShader>(slots[4]);
								if(slots[5] != UINT_MAX)	renderer.m_cbLightCount.BindBuffer<PShader>(slots[5]);
								if(slots[6] != UINT_MAX)	renderer.m_cbCamera.BindBuffer<PShader>(slots[6]);
								if(slots[7] != UINT_MAX)	renderer.m_cbUseTexture.BindBuffer<PShader>(slots[7]);
								if(slots[8] != UINT_MAX)	renderer.m_sbLight.BindSRV<PShader>(slots[8]);
								if(slots[9] != UINT_MAX)	RenderSystem::BindShaderResourceView<PShader>(slots[9], renderer.m_lGrid);
								if(slots[10] != UINT_MAX)	RenderSystem::BindShaderResourceView<PShader>(slots[10], renderer.m_lIndex);
								if(slots[11] != UINT_MAX)	pMaterial->m_dBuffer.BindBuffer<PShader>(slots[11]);

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
								if(slots[12] != UINT_MAX)	renderer.m_cbTransform.BindBuffer<CShader>(slots[12]);
								if(slots[13] != UINT_MAX)	renderer.m_cbLightCount.BindBuffer<CShader>(slots[13]);
								if(slots[14] != UINT_MAX)	renderer.m_cbCamera.BindBuffer<CShader>(slots[14]);
								if(slots[15] != UINT_MAX)	renderer.m_cbUseTexture.BindBuffer<CShader>(slots[15]);
								if(slots[16] != UINT_MAX)	renderer.m_sbLight.BindSRV<CShader>(slots[16]);
								if(slots[17] != UINT_MAX)	pMaterial->m_dBuffer.BindBuffer<CShader>(slots[17]);
								if(slots[18] != UINT_MAX)	renderer.m_cbBGColor.BindBuffer<CShader>(slots[18]);

								RenderSystem::BindShaderResourceView<CShader>(0u, pMaterial->m_pShaderModel->m_gBuffer);
								RenderSystem::BindUnOrderedAccessView(0u, renderer.m_UAVBackBuffer);
								RenderSystem::BindUnOrderedAccessView(1u, renderer.m_UAVFinal);
								RenderSystem::BindShader(pMaterial->m_pShaderModel->m_CShader);

								RenderSystem::Dispatch(renderer.m_dispatchGroups, 1u);

								RenderSystem::UnBindShader<CShader>();
								RenderSystem::UnBindUnOrderedAccessViews(0u, 2u);
								RenderSystem::UnBindShaderResources<CShader>(0u, pMaterial->m_pShaderModel->m_gBuffer.GetCount());
						}
					}
				}
		}
	}

	void MeshInstance::DepthDraw(Renderer& renderer, const uint32_t& layer) {

		if(m_pMesh) {

			Mat4f m = EntityHandler::GetEntity(m_id).GetTransformation();
			renderer.m_cbTransform.GetData().m_mdlMat = m.Transposed();
			renderer.m_cbTransform.UpdateBuffer();

			//DirectX::BoundingOrientedBox box;
			//box.Extents = m_pMesh->m_aabb.Extents;
			DirectX::BoundingOrientedBox box(m_pMesh->m_aabb.Center, m_pMesh->m_aabb.Extents, DirectX::XMFLOAT4(0, 0, 0, 1));
			box.Transform(box, m.smMat);

			if(GlobalSettings::GetMainCamera())
				if(box.Intersects(GlobalSettings::GetMainCamera()->GetComponent<Camera>().GetFrustum())) {
					Material* pMaterial = (m_pMesh && m_pMaterials[0]) ? m_pMaterials[0] : nullptr;
					if(pMaterial) {
						RenderSystem::SetInputLayout(renderer.m_opaqueLayout);
						RenderSystem::BindShader(renderer.m_opaqueVS);
						RenderSystem::BindShader(renderer.m_transparencyPS);

						renderer.m_cbTransform.BindBuffer<VShader>(0u);
					}

					m_pMesh->m_vertices.BindBuffer();
					m_pMesh->m_vertices.Draw();
				}
		}
	}
}