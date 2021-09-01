#include"PCH.h"
#include"Mesh.h"
#include"Core/GlobalSettings.h"
#include"Importer/ObjImporter.h"
#include<thread>
#include<thread>

namespace Aen {
	
	Mesh::Mesh()
		:m_pMaterials(1u, &Resource::GetMaterial("DefaultMaterial")) {}
	
	Mesh::Mesh(const std::string& dir) {
		ImportObj(m_vertices, dir, m_partitions, m_meshMaterialName);
		if(m_meshMaterialName.size() > 0) {
			m_pMaterials.reserve(m_meshMaterialName.size());
			m_pMaterials.resize(m_meshMaterialName.size(), &Resource::GetMaterial("DefaultMaterial"));
		}
	}
	
	void Mesh::PrintMaterialSlots() {
		for(auto& i : m_meshMaterialName) {
			AEN_PRINT(i.first); 
			AEN_ENDL;
		}
	}

	void Mesh::Load(const std::string& dir) {
		std::thread worker(ImportObj, std::ref(m_vertices), dir, std::ref(m_partitions), std::ref(m_meshMaterialName));
		worker.join();
		if(m_meshMaterialName.size() > 0) {
			m_pMaterials.reserve(m_meshMaterialName.size());
			m_pMaterials.resize(m_meshMaterialName.size(), &Resource::GetMaterial("DefaultMaterial"));
		}
	}

	void Mesh::SetMaterial(Material& material) {
		m_pMaterials[0] = &material;
	}
	
	void Mesh::SetMaterial(const std::string& materialSlotName, Material& material) {
		if(m_meshMaterialName.count(materialSlotName) == 0) throw;
		m_pMaterials[m_meshMaterialName.at(materialSlotName)] = &material;
	}

	void Mesh::SetMaterial(const std::string& materialSlotName, const std::string& materialName) {
		if(m_meshMaterialName.count(materialSlotName) == 0) throw;
		if(!Resource::MaterialExist(materialName)) throw;
		m_pMaterials[m_meshMaterialName.at(materialSlotName)] = &Resource::GetMaterial(materialName);
	}

	Mesh::~Mesh() {
		for(auto& i : m_pMaterials)
			i = nullptr;
	}

	MeshInstance::~MeshInstance() {
		m_pMesh = nullptr;
	}

	MeshInstance::MeshInstance()
		:m_pMesh(nullptr) {}

	MeshInstance::MeshInstance(Mesh& mesh)
		:m_pMesh(&mesh) {}

	void MeshInstance::SetMesh(Mesh& mesh) {
		m_pMesh = &mesh;
	}
}