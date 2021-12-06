#pragma once
#include"Graphics/Component/Drawable/Mesh/Mesh.h"
#undef min
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "../AenirEngine/Graphics/Component/Animation/Animation.h"

#include<vector>
#include<unordered_map>

namespace Aen
{
	class AssimpImport
	{
	public:
		static void LoadFbx(std::vector<DirectX::XMFLOAT3>& vPos, VBuffer<Vertex>& vBuffer, const std::string path, std::vector<PartitionData>& partitions, std::unordered_map<std::string, uint32_t>& meshMaterial, std::vector<uint32_t>& invertIndices);
		static void LoadFbxAnimation(const std::string path, std::vector<Bones>& boneArray, std::unordered_map<std::string, std::vector<KeyFrameData>>& keyFrames, std::vector<float>& timeStamp, float& duration);
	private:
		static void ProcessMesh(UINT& offset, aiMesh* mesh, const aiScene* scene, std::vector<Aen::Vertex>& verts, std::vector<uint32_t>& invertIndices, std::vector<Aen::PartitionData>& partsData, std::unordered_map<std::string, uint32_t>& meshMaterial);
		static void ProcessNode(aiNode* node, const aiScene* scene, Aen::VBuffer<Aen::Vertex>& vBuffer,
			std::vector<Aen::Vertex>& verts, std::vector<uint32_t>& invertIndices,
			std::vector<Aen::PartitionData>& partsData, std::unordered_map<std::string, uint32_t>& meshMaterial);

		static void ProcNode(aiNode* node, const aiScene* pScene, std::vector<std::vector<Aen::Vertex>>& mGroup, std::vector<uint32_t>& invertIndices, const std::unordered_map<std::string, Bones>& skeleton);
		static void ProcMesh(aiMesh* mesh, const aiScene* pScene, std::vector<Aen::Vertex>& verts, std::vector<uint32_t>& invertIndices, const std::unordered_map<std::string, Bones>& skeleton);

		static void ProcNodeAnim(aiNode* node, const aiScene* scene, std::unordered_map<std::string, aiBone*>& bones);
		static void ProcBoneHeiarchy(aiNode* node, const aiScene* scene, std::unordered_map<std::string, aiBone*>& bones, std::vector<Bones>& boneArray, const int& parentId = -1);
		static void ProcAnimation(const aiScene* scene, std::unordered_map<std::string, std::vector<KeyFrameData>>& keyFrames, std::vector<float>& m_timeStamp, float& duration);
	};

	
	
}