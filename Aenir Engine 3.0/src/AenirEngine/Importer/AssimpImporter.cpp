#include "PCH.h"
#include "AssimpImporter.h"

#undef min
//#include"../AenirEngine/ThirdParty/assimp/include/assimp/Importer.hpp"
//#include"../AenirEngine/ThirdParty/assimp/include/assimp/scene.h"
//#include"../AenirEngine/ThirdParty/assimp/include/assimp/postprocess.h"
//#include"../AenirEngine/ThirdParty/assimp/include/assimp/matrix4x4.h"
//#include"../AenirEngine/ThirdParty/assimp/include/assimp/cimport.h"

void Aen::AssimpImport::LoadFbx(VBuffer<Vertex>& vBuffer, const std::string path, std::vector<PartitionData>& partitions, std::unordered_map<std::string, uint32_t>& meshMaterial)
{
	std::vector<Vertex> mesh;
	Assimp::Importer importer;

	const aiScene* pScene = importer.ReadFile(path, aiProcess_CalcTangentSpace);

	

	if (pScene == NULL) {
		throw;
		printf("Assimp failed or path to model does not exist");
	}

	AssimpImport::ProcessNode(pScene->mRootNode, pScene, vBuffer, mesh, partitions, meshMaterial);

	if (!vBuffer.Create(mesh.data(), (UINT)mesh.size())) {
		throw;
		printf("Failed to create vbuffer");
	}
}

void Aen::AssimpImport::ProcessMesh(aiMesh* mesh, const aiScene* scene, std::vector<Aen::Vertex>& verts, std::vector<Aen::PartitionData>& partsData, std::unordered_map<std::string, uint32_t>& meshMaterial)
{
	UINT numVerts = mesh->mNumVertices;
	UINT numMats = scene->mNumMaterials;
	aiMaterial* material;
	for (UINT i = 0; i < numMats; i++)
	{
		material = scene->mMaterials[mesh->mMaterialIndex];
		//meshMaterial.emplace(material->GetName().C_Str(), mesh->mMaterialIndex);
	}
	verts.resize(numVerts);
	Aen::PartitionData data;
	data.materialIndex = mesh->mMaterialIndex;
	data.size = numVerts;
	data.offset = numVerts;
	partsData.emplace_back(data);
	for (UINT i = 0; i < numVerts; i++)
	{
		Aen::Vertex vertex;
		vertex.pos.x = mesh->mVertices[i].x;
		vertex.pos.y = mesh->mVertices[i].y;
		vertex.pos.z = mesh->mVertices[i].z;

		if (mesh->mTextureCoords[0])
		{
			vertex.uv.x = (float)mesh->mTextureCoords[0][i].x;
			vertex.uv.y = (float)mesh->mTextureCoords[0][i].y;
		}
		if (mesh->HasNormals())
		{
			vertex.norm.x = (float)mesh->mNormals[i].x;
			vertex.norm.y = (float)mesh->mNormals[i].y;
			vertex.norm.z = (float)mesh->mNormals[i].z;
		}
		if (mesh->HasTangentsAndBitangents())
		{
			vertex.tan.x = (float)mesh->mTangents[i].x;
			vertex.tan.y = (float)mesh->mTangents[i].y;
			vertex.tan.z = (float)mesh->mTangents[i].z;

			vertex.bi.x = (float)mesh->mBitangents[i].x;
			vertex.bi.y = (float)mesh->mBitangents[i].y;
			vertex.bi.z = (float)mesh->mBitangents[i].z;
		}

		verts.emplace_back(vertex);
	}
	printf("\n");
}

void Aen::AssimpImport::ProcessNode(aiNode* node, const aiScene* scene, Aen::VBuffer<Aen::Vertex>& vBuffer,
	std::vector<Aen::Vertex>& verts, std::vector<Aen::PartitionData>& partsData, std::unordered_map<std::string, uint32_t>& meshMaterial)
{
	UINT numMeshes = node->mNumMeshes;
	for (UINT i = 0; i < numMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		AssimpImport::ProcessMesh(mesh, scene, verts, partsData, meshMaterial);
	}

	UINT numNodes = node->mNumChildren;
	for (UINT i = 0; i < numNodes; i++)
		AssimpImport::ProcessNode(node->mChildren[i], scene, vBuffer, verts, partsData, meshMaterial);
}
