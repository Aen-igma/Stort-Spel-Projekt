#include "PCH.h"
#include "AssimpImporter.h"

#undef min


void Aen::AssimpImport::LoadFbx(std::vector<DirectX::XMFLOAT3>& vPos, VBuffer<Vertex>& vBuffer, const std::string path, std::vector<PartitionData>& partitions, std::unordered_map<std::string, uint32_t>& meshMaterial)
{
	std::vector<Vertex> mesh;
	Assimp::Importer importer;

	const aiScene* pScene = importer.ReadFile(path, aiProcess_CalcTangentSpace | aiProcess_MakeLeftHanded);
	

	if (pScene == NULL) {
		throw;
		printf("Assimp failed or path to model does not exist");
	}

	AssimpImport::ProcessNode(pScene->mRootNode, pScene, vBuffer, mesh, partitions, meshMaterial);

	UINT meshSize = mesh.size();

	vPos.resize(meshSize);
	for (int i = 0; i < meshSize; i++)
	{
		vPos[i] = mesh[i].pos.smVec;

		//pPosV[i] = mesh[i].pos.smVec;
	}

	if (!vBuffer.Create(mesh.data(), (UINT)mesh.size())) {
		throw;
		printf("Failed to create vbuffer");
	}
}

void Aen::AssimpImport::ProcessMesh(UINT& offset, aiMesh* mesh, const aiScene* scene, std::vector<Aen::Vertex>& verts, std::vector<Aen::PartitionData>& partsData, std::unordered_map<std::string, uint32_t>& meshMaterial)
{
	UINT numVerts = mesh->mNumVertices;

	

	aiMaterial* material;
	material = scene->mMaterials[mesh->mMaterialIndex];
	meshMaterial.emplace(material->GetName().C_Str(), mesh->mMaterialIndex);
	
	Aen::PartitionData data;
	data.materialIndex = mesh->mMaterialIndex;
	data.size = numVerts;
	data.offset = offset;
	partsData.emplace_back(data);

	for (UINT m = 0; m < 1; m++)
	{
		for (UINT i = 0; i < numVerts; i++)
		{
			Vertex vertex;
			vertex.pos.x = mesh[m].mVertices[i].x;
			vertex.pos.y = mesh[m].mVertices[i].y;
			vertex.pos.z = mesh[m].mVertices[i].z;

			if (mesh->mTextureCoords[0])
			{
				vertex.uv.x = (float)mesh[m].mTextureCoords[0][i].x;
				vertex.uv.y = (float)mesh[m].mTextureCoords[0][i].y;
			}
			if (mesh->HasNormals())
			{
				vertex.norm.x = (float)mesh[m].mNormals[i].x;
				vertex.norm.y = (float)mesh[m].mNormals[i].y;
				vertex.norm.z = (float)mesh[m].mNormals[i].z;
			}
			if (mesh->HasTangentsAndBitangents())
			{
				vertex.tan.x = (float)mesh[m].mTangents[i].x;
				vertex.tan.y = (float)mesh[m].mTangents[i].y;
				vertex.tan.z = (float)mesh[m].mTangents[i].z;

				vertex.bi.x = (float)mesh[m].mBitangents[i].x;
				vertex.bi.y = (float)mesh[m].mBitangents[i].y;
				vertex.bi.z = (float)mesh[m].mBitangents[i].z;
			}
			verts.emplace_back(vertex);
		}
		offset = numVerts;
		printf("\n");
	}
	
}

void Aen::AssimpImport::ProcessNode(aiNode* node, const aiScene* scene, Aen::VBuffer<Aen::Vertex>& vBuffer,
	std::vector<Aen::Vertex>& verts, std::vector<Aen::PartitionData>& partsData, std::unordered_map<std::string, uint32_t>& meshMaterial)
{
	UINT offset = 0;
	UINT numMeshes = node->mNumMeshes;
	for (UINT i = 0; i < numMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		AssimpImport::ProcessMesh(offset, mesh, scene, verts, partsData, meshMaterial);
	}

	UINT numNodes = node->mNumChildren;
	for (UINT i = 0; i < numNodes; i++)
		AssimpImport::ProcessNode(node->mChildren[i], scene, vBuffer, verts, partsData, meshMaterial);
}
