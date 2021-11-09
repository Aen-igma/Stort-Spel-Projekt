#pragma once
#include"Graphics/Component/Drawable/Mesh/Mesh.h"
#undef min
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "../AenirEngine/Graphics/Component/Animation/Animation.h"

//#include"../AenirEngine/ThirdParty/assimp/include/assimp/Importer.hpp"
//#include"../AenirEngine/ThirdParty/assimp/include/assimp/scene.h"
//#include"../AenirEngine/ThirdParty/assimp/include/assimp/postprocess.h"
//#include"../AenirEngine/ThirdParty/assimp/include/assimp/matrix4x4.h"
//#include"../AenirEngine/ThirdParty/assimp/include/assimp/cimport.h"

namespace Aen
{
	class AssimpImport
	{
	public:
		static void LoadFbx(VBuffer<Vertex>& vBuffer, const std::string path, std::vector<PartitionData>& partitions, std::unordered_map<std::string, uint32_t>& meshMaterial);
	private:
		static void ProcessMesh(aiMesh* mesh, const aiScene* scene, std::vector<Aen::Vertex>& verts, std::vector<Aen::PartitionData>& partsData, std::unordered_map<std::string, uint32_t>& meshMaterial);
		static void ProcessNode(aiNode* node, const aiScene* scene, Aen::VBuffer<Aen::Vertex>& vBuffer,
			std::vector<Aen::Vertex>& verts, std::vector<Aen::PartitionData>& partsData, std::unordered_map<std::string, uint32_t>& meshMaterial);
	};

	
	
}