#include"PCH.h"
#include"Mesh.h"
#include"Importer/ObjImporter.h"

namespace Aen {

	Mesh::Mesh(const std::string& dir) {
		ImportObj(m_vertices, dir);
	}
	
	void Mesh::Load(const std::string& dir) {
		ImportObj(m_vertices, dir);
	}

	MeshInstance::~MeshInstance() {
		m_mesh = nullptr;
	}

	MeshInstance::MeshInstance(Mesh& mesh)
		:m_mesh(&mesh) {}

	void MeshInstance::SetMesh(Mesh& mesh) {
		m_mesh = &mesh;
	}
}