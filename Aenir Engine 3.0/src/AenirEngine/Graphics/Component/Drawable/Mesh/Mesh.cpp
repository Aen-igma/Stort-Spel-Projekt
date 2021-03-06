#include"PCH.h"
#include"Mesh.h"
#include"Core/GlobalSettings.h"
#include"Importer/ObjImporter.h"
#include"Importer/AssimpImporter.h"
#include<thread>



namespace Aen {
	
	Mesh::Mesh()
		:m_vertices(), m_partitions(), m_meshMaterialName(), m_obb(), m_aabb() {}
	
	Mesh::Mesh(const std::string& dir)
		: m_obb(), m_aabb() {
		ImportObj(m_vertices, dir, m_partitions, m_meshMaterialName);
	}
	
	void Mesh::PrintMaterialSlots() {
		for(auto& i : m_meshMaterialName) {
			AEN_PRINT(i.first); 
			AEN_ENDL;
		}
	}

	void Mesh::Load(const std::string& dir) {


		size_t off = dir.find_last_of('.');
		std::string format = dir.substr(off + 1);
		if (format == "fbx") {
			AssimpImport::LoadFbx(m_vPos, m_vertices, dir, m_partitions, m_meshMaterialName, m_indices);

			size_t vStride = sizeof(DirectX::XMFLOAT3);
			m_aabb.CreateFromPoints(m_aabb, m_vPos.size(), m_vPos.data(), vStride);
			m_obb.CreateFromPoints(m_obb, m_vPos.size(), m_vPos.data(), vStride);

		} else {
			throw;
			printf("Format not supported!");
		}
		

	}

	const std::vector<DirectX::XMFLOAT3>& Mesh::GetvPos()
	{
		return this->m_vPos;
	}

	const std::vector<uint32_t>& Mesh::GetIndices()
	{
		return this->m_indices;
	}

	DirectX::BoundingBox Mesh::getAABB() const
	{
		return m_aabb;
	}

	DirectX::BoundingOrientedBox Mesh::getOBB() const
	{
		return m_obb;
	}

	std::vector<PartitionData> Mesh::getPartitions() const
	{
		return m_partitions;
	}

	Mesh::~Mesh() {

		//delete[] mp_posV;
	}
}