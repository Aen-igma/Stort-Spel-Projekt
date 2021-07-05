#include"PCH.h"
#include"Mesh.h"
#include"Importer/ObjImporter.h"

namespace Aen {
	
	Mesh::Mesh(const std::string& name)
		:m_vertices() {
		MeshHandler::GetMeshes().emplace(name, this);
	}

	Mesh::Mesh(const std::string& dir, const std::string& name) {
		ImportObj(m_vertices, dir);
		MeshHandler::GetMeshes().emplace(name, this);
	}
	
	void Mesh::Load(const std::string& dir) {
		ImportObj(m_vertices, dir);
	}

	MeshInstance::~MeshInstance() {
		MeshIHandler::GetMesheInstances().erase(m_id);
	}

	MeshInstance::MeshInstance(const uint32_t& id, Mesh& mesh)
		:m_mesh(&mesh), m_id(id) {
		MeshIHandler::GetMesheInstances().emplace(m_id, this);
	}

	void MeshInstance::SetMesh(Mesh& mesh) {
		m_mesh = &mesh;
	}
}