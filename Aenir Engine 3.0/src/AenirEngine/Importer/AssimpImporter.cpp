#include "PCH.h"
#include "AssimpImporter.h"

#undef min
#include"../AenirEngine/ThirdParty/assimp/include/assimp/Importer.hpp"
#include"../AenirEngine/ThirdParty/assimp/include/assimp/scene.h"
#include"../AenirEngine/ThirdParty/assimp/include/assimp/postprocess.h"
#include"../AenirEngine/ThirdParty/assimp/include/assimp/matrix4x4.h"
#include"../AenirEngine/ThirdParty/assimp/include/assimp/cimport.h"

void ProcessNode(aiNode* node, const aiScene* scene)
{
	UINT numMeshes = node->mNumMeshes;
	for (UINT i = 0; i < numMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
	}
	UINT numMeshes = node->mNumChildren;
	for (UINT i = 0; i < numMeshes; i++)
		ProcessNode(node->mChildren[i], scene);
}

void ProcessMesh(aiMesh* mesh, const aiScene* scene, std::vector<Aen::Vertex> verts)
{
	UINT numVerts = mesh->mNumVertices;
	verts.resize(numVerts);
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
			vertex.norm.x = (float)mesh->mNormals->x;
			vertex.norm.y = (float)mesh->mNormals->y;
			vertex.norm.z = (float)mesh->mNormals->z;
		}
		if (mesh->mTangents[0][i])
		{
			vertex.tan.x = 
		}
		mesh->mNormals

		verts.emplace_back(vertex);
	}
}

void Aen::LoadFbx(VBuffer<Vertex> vBuffer, const std::string path, std::vector<PartitionData>& partitions, std::unordered_map<std::string, uint32_t>& meshMaterial)
{
	std::vector<Vertex> mesh;
	Assimp::Importer importer;

	const aiScene* pScene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);

	if (pScene == NULL)
		throw; printf("Assimp failed or path to model does not exist");


}
