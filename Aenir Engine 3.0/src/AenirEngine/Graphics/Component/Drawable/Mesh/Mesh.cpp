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
		:m_pMaterials(1u, &Resource::GetMaterial("DefaultMaterial")) {
	
		/*const aiScene* animTest = aiImportFile("../Resource/rectAnirectAni.fbx", aiProcessPreset_TargetRealtime_MaxQuality);
		std::cout << "Number of meshes: " << animTest->mNumMeshes << std::endl;
		std::cout << "Number of verts: " << animTest->mMeshes[0]->mNumVertices << std::endl;
		std::cout << "Does mesh have Animations?: " << animTest->HasAnimations() << std::endl;
		std::cout << "Name of Animation: " << (animTest->mAnimations[0]->mName).C_Str() << std::endl;
		std::cout << "Duration of Ani: " << animTest->mAnimations[0]->mDuration << std::endl;
		std::cout << "Number of Ani: " << animTest->mNumAnimations << std::endl;
		std::cout << "mBones: " << (animTest->mMeshes[0]->mBones[0]->mName).C_Str() << std::endl;*/
	}
	
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
}