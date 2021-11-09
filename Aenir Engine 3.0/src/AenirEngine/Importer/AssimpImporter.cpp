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

	const aiScene* pScene = importer.ReadFile(path, aiProcess_CalcTangentSpace | aiProcess_Triangulate);

	

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
	Animation animation(;
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

		vertex.boneId = { 0, 0, 0, 0 };
		vertex.boneWeights = { 0.f, 0.f, 0.f, 0.f };
		
		verts.emplace_back(vertex);
	}
	printf("\n");

	std::unordered_map<std::string, std::pair<int, Mat4f>> boneInfo = {};
	std::vector<UINT> boneCount;
	boneCount.resize(verts.size(), 0);

	for (int i = 0; i < mesh->mNumBones; i++) {
		aiBone* bone = mesh->mBones[i];
		Mat4f bOffset;
		bOffset.a11 = mesh->mBones[i]->mOffsetMatrix.a1; bOffset.a12 = mesh->mBones[i]->mOffsetMatrix.a2;
		bOffset.a13 = mesh->mBones[i]->mOffsetMatrix.a3; bOffset.a14 = mesh->mBones[i]->mOffsetMatrix.a4;

		bOffset.a21 = mesh->mBones[i]->mOffsetMatrix.b1; bOffset.a22 = mesh->mBones[i]->mOffsetMatrix.b2;
		bOffset.a23 = mesh->mBones[i]->mOffsetMatrix.b3; bOffset.a24 = mesh->mBones[i]->mOffsetMatrix.b4;

		bOffset.a31 = mesh->mBones[i]->mOffsetMatrix.c1; bOffset.a32 = mesh->mBones[i]->mOffsetMatrix.c2;
		bOffset.a33 = mesh->mBones[i]->mOffsetMatrix.c3; bOffset.a34 = mesh->mBones[i]->mOffsetMatrix.c4;

		bOffset.a41 = mesh->mBones[i]->mOffsetMatrix.d1; bOffset.a42 = mesh->mBones[i]->mOffsetMatrix.d2;
		bOffset.a43 = mesh->mBones[i]->mOffsetMatrix.d3; bOffset.a44 = mesh->mBones[i]->mOffsetMatrix.d4;

		boneInfo[bone->mName.C_Str()] = { i, bOffset };

		for (int j = 0; j < bone->mNumWeights; j++) {
			UINT id = bone->mWeights[j].mVertexId;
			float weight = bone->mWeights[j].mWeight;
			boneCount[id]++;
			switch (boneCount[id]) {
			case 1:
				verts[id].boneId.x = i;
				verts[id].boneWeights.x = weight;
				break;
			case 2:
				verts[id].boneId.y = i;
				verts[id].boneWeights.y = weight;
				break;
			case 3:
				verts[id].boneId.z = i;
				verts[id].boneWeights.z = weight;
				break;
			case 4:
				verts[id].boneId.w = i;
				verts[id].boneWeights.w = weight;
				break;
			default:
				std::cout << "ERROR: UNABLE TO ALLOCATE BONE TO VERTEX" << std::endl;
				break;
			}
		}
	}

	for (int i = 0; i < verts.size(); i++) {
		Vec4f& boneWeights = verts[i].boneWeights;
		float totalWeight = boneWeights.x + boneWeights.y + boneWeights.z + boneWeights.w;
		if (totalWeight > 0.0f) {
			verts[i].boneWeights = Vec4f(boneWeights.x / totalWeight, boneWeights.y / totalWeight, boneWeights.z / totalWeight, boneWeights.w / totalWeight);
		}
	}

	
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
