#include"PCH.h"
#include"Mesh.h"
#include"Core/GlobalSettings.h"
#include"Importer/ObjImporter.h"
#include<thread>

#undef min

#include"assimp/Importer.hpp"
#include"assimp/scene.h"
#include"assimp/postprocess.h"
#include"assimp/matrix4x4.h"
#include"assimp/cimport.h"

namespace Aen {
	
	Mesh::Mesh()
		:m_vertices(), m_partitions(), m_meshMaterialName() {}
	
	Mesh::Mesh(const std::string& dir) {
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
			throw;
			printf("Format not supported!");
		}
		else if (format == "obj") {
			std::thread worker(ImportObj, std::ref(m_vertices), dir, std::ref(m_partitions), std::ref(m_meshMaterialName));
			worker.join();
		}
		else {
			throw;
			printf("Format not supported!");
		}
		

	}

	Mesh::~Mesh() {}
}