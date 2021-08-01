#include"PCH.h"
#include"Mesh.h"
#include"Core/GlobalSettings.h"
#include"Importer/ObjImporter.h"

namespace Aen {
	
	Mesh::Mesh()
		:m_pMaterials(1u, &ResourceHandler::GetMaterial("DefaultMaterial")) {}
	
	Mesh::Mesh(const std::string& dir) {
		ImportObj(m_vertices, dir, m_partitions, m_meshMaterialName);
		if(m_meshMaterialName.size() > 0) {
			m_pMaterials.reserve(m_meshMaterialName.size());
			m_pMaterials.resize(m_meshMaterialName.size(), &ResourceHandler::GetMaterial("DefaultMaterial"));
		}
	}
	
	void Mesh::PrintMaterialSlots() {
		for(auto& i : m_meshMaterialName) {
			AEN_PRINT(i.first); 
			AEN_ENDL;
		}
	}

	void Mesh::Load(const std::string& dir) {
		ImportObj(m_vertices, dir, m_partitions, m_meshMaterialName);
		if(m_meshMaterialName.size() > 0) {
			m_pMaterials.reserve(m_meshMaterialName.size());
			m_pMaterials.resize(m_meshMaterialName.size(), &ResourceHandler::GetMaterial("DefaultMaterial"));
		}
	}

	void Mesh::SetMaterial(Material& material) {
		m_pMaterials[0] = &material;
	}
	
	void Mesh::SetMaterial(const std::string& materialSlotName, Material& material) {
		m_pMaterials[m_meshMaterialName.at(materialSlotName)] = &material;
	}

	Mesh::~Mesh() {
		for(auto& i : m_pMaterials)
			i = nullptr;
	}

	MeshInstance::~MeshInstance() {
		m_mesh = nullptr;
	}

	MeshInstance::MeshInstance()
		:m_mesh(nullptr) {}

	MeshInstance::MeshInstance(Mesh& mesh)
		:m_mesh(&mesh) {}

	void MeshInstance::SetMesh(Mesh& mesh) {
		m_mesh = &mesh;
	}
}