#include "PCH.h"
#include "Animation.h"

#undef min
#include"../AenirEngine/ThirdParty/assimp/include/assimp/Importer.hpp"
#include"../AenirEngine/ThirdParty/assimp/include/assimp/scene.h"
#include"../AenirEngine/ThirdParty/assimp/include/assimp/postprocess.h"
#include"../AenirEngine/ThirdParty/assimp/include/assimp/matrix4x4.h"
#include"../AenirEngine/ThirdParty/assimp/include/assimp/cimport.h"

namespace Aen {
	Assimp::Importer importer;
	const aiScene* animation = importer.ReadFile("../Resource/ThanosLastest.fbx", aiProcess_Triangulate);

	void LoadingBones(Bones& bones, const aiNode* fromFile, std::vector<Bones>& boneArray) {
		Bones currentBone;
		currentBone.boneID = boneArray.size();
		currentBone.parentID = bones.boneID;

		for (int i = 0; i < fromFile->mNumChildren; i++) {
			boneArray.emplace_back(currentBone);
			LoadingBones(currentBone, fromFile->mChildren[i], boneArray);
		}
	}

	Animation::Animation()
	{
	}

	Animation::Animation(const std::string& animationPath)
	{
		Assimp::Importer importer;
		const aiScene* animTest = importer.ReadFile("../Resource/ThanosLastest.fbx", aiProcess_Triangulate);
		assert(animTest && animTest->mRootNode);
		m_Duration = animTest->mAnimations[0]->mDuration;
		m_TicksPerSecond = animTest->mAnimations[0]->mTicksPerSecond;
	}

	
	void Animation::printTest()
	{
		AEN_PRINT("Hello There\n");
	}

	void FindRootBone(Bones& bones, const aiScene* scene, std::vector<Bones>& boneArray) {
		aiBone* rootBone = scene->mMeshes[0]->mBones[0];
		
	}

	std::vector<aiNode*> ai_nodes;
	std::vector<aiNodeAnim*> ai_nodes_anim;
	std::vector<aiNode*> ai_node_bones;
	std::vector<aiBone*> ai_bone_data;
	void RecursiveNodeProcess(aiNode* node) {
		ai_nodes.emplace_back(node);
		for (int i = 0; i < node->mNumChildren; i++) {
			RecursiveNodeProcess(node->mChildren[i]);
		}
	}

	void AssignBones(const aiScene* scene, std::vector<aiNode*>& nodeArray, std::vector<Bones>& boneArray) {
		for (int i = 0; i < nodeArray.size(); i++) {
			for (int j = 0; j < scene->mMeshes[0]->mNumBones; j++) {
				if (nodeArray[i]->mName == scene->mMeshes[0]->mBones[j]->mName) {
					//boneArray.emplace_back(scene->mMeshes[0]->mBones[j]);
					ai_node_bones.emplace_back(nodeArray[i]);
					//OutputDebugString((nodeArray[i]->mName).C_Str());
					//OutputDebugString("\n");
					//OutputDebugString((ai_node_bones[i]->mName).C_Str());
				}
			}
		}
	}

	int FindParentID(const aiNode* parentNode, std::vector<aiNode*>& boneNodeArray) {
		for (int i = 0; i < boneNodeArray.size(); i++) {
			if (parentNode->mName == boneNodeArray[i]->mName) {
				return i;
			}
		}
	}

	void MeshBoneData(const aiMesh* meshNode, std::vector<aiBone*>& boneArray) {
		for (int i = 0; i < meshNode->mNumBones; i++) {
			boneArray.emplace_back(meshNode->mBones[i]);
		}
	}

	void TransferBones(std::vector<aiBone*>& boneArray, std::vector<Bones>& boneArrayFinal) {
		Bones bone;
		for (int i = 0; i < boneArray.size(); i++) {
			bone.boneID = i;
			bone.boneName = boneArray[i]->mName.data;
			OutputDebugString("Num Weights");
			OutputDebugString(std::to_string(boneArray[i]->mNumWeights).c_str());
			OutputDebugString("\n");
		}
	}

	void TransferNodeBoneData(std::vector<aiNode*>& nodeBoneArray, std::vector<Bones>& boneArray) {
		Bones bone;
		for (size_t i = 0; i < nodeBoneArray.size(); i++) {
			bone.boneName = nodeBoneArray[i]->mName.data;
			bone.boneID = i;
			//bone.parentID = FindParentID(nodeBoneArray[i], nodeBoneArray);
			//OutputDebugString((nodeBoneArray[i]->mParent->mName).C_Str());
			boneArray.emplace_back(bone);
		}
		/*for (int i = 0; i < boneArray.size(); i++) {
			OutputDebugString((boneArray[i].boneName).c_str());
			OutputDebugString(" : ");
			OutputDebugString(std::to_string(boneArray[i].boneID).c_str());
			OutputDebugString(" : ");
			OutputDebugString(std::to_string(boneArray[i].parentID).c_str());
			OutputDebugString("\n");
		}*/
	}

	void AnimProcess() {
		if (animation->mNumAnimations == 0)
			return;
		for (int i = 0; i < animation->mAnimations[0]->mNumChannels; i++) {
			ai_nodes_anim.emplace_back(animation->mAnimations[0]->mChannels[i]);
		}
	}

	void Animation::LoadAnimation(const std::string& animationPath)
	{
		Bones rootBone;
		rootBone.boneID = 0;
		rootBone.parentID = -1;
		rootBone.boneName = "GROOT";
		
		assert(animation && animation->mRootNode);
		auto rootBoneNode = animation->mMeshes[0]->mBones[0];
		m_boneArray.emplace_back(rootBone);
		FindRootBone(rootBone, animation, m_boneArray);


		RecursiveNodeProcess(animation->mRootNode);
		AnimProcess();
		//AssignBones(animation, ai_nodes, m_boneArray);
		//TransferNodeBoneData(ai_node_bones, m_boneArray);
		MeshBoneData(animation->mMeshes[0], ai_bone_data);
		TransferBones(ai_bone_data, m_boneArray);

		//for (int i = 1; i < ai_nodes.size(); i++) {
		//	OutputDebugString((ai_nodes[i]->mParent->mName).C_Str());
		//	OutputDebugString((LPCSTR)"  :  ");
		//	OutputDebugString((ai_nodes[i]->mName).C_Str());
		//	//OutputDebugString((LPCSTR)"  :  ");
		//	//OutputDebugString((ai_nodes[i]->mChildren[i]->mName).C_Str());
		//	OutputDebugString((LPCSTR)"\n");
		//}
		//for (int i = 0; i < ai_nodes_anim.size(); i++) {
		//	OutputDebugString((ai_nodes_anim[i]->mNodeName).C_Str());
		//	OutputDebugString((LPCSTR)"\n");
		//}
		//for (int i = 0; i < m_boneArray.size(); i++) {
		//	OutputDebugString((m_boneArray[i].boneName).c_str());
		//	OutputDebugString((LPCSTR)"\n");
		//}
		//for (int i = 0; i < ai_node_bones.size(); i++) {
		//	/*OutputDebugString((ai_nodes[i]->mName).C_Str());
		//	OutputDebugString("\n");*/
		//	OutputDebugString((ai_node_bones[i]->mName).C_Str());
		//	OutputDebugString("\n");
		//}

		//OutputDebugString("MESH NAME");
		//OutputDebugString((animation->mMeshes[0]->mBones[0]->mName).C_Str());

	/*	for (int i = 0; i < animation->mRootNode->mNumChildren; i++) {
			OutputDebugString((animation->mRootNode->mChildren[i]->mName).C_Str());
			OutputDebugString((LPCSTR)"\n");
		}
		for (int i = 0; i < animation->mRootNode->mChildren[1]->mNumChildren; i++) {
			OutputDebugString((animation->mRootNode->mChildren[1]->mChildren[i]->mName).C_Str());
			OutputDebugString((LPCSTR)"\n");
		}*/


		/*for (int i = 0; i < animation->mMeshes[0]->mNumBones; i++) {
			Bones currentBone;
			currentBone.boneID = i;
			m_boneArray.emplace_back(currentBone);

			OutputDebugString(std::to_string(m_boneArray[i].boneID).c_str());
		}*/



		/*for (int i = 0; i < m_boneArray.size(); i++) {
			OutputDebugString(std::to_string(m_boneArray[i].boneID).c_str());
			OutputDebugString((LPCSTR)"\n");
		}*/

		//LoadingBones(rootBone, animation->mRootNode->mChildren[1], m_boneArray);
		//LoadingBones(rootBone, animation->mMeshes[0]->mBones[0], m_boneArray);

		//animation->mMeshes[0]->mBones[0]->
		//for (int i = 0; i < animation->mMeshes[0]->mNumBones; i++) {
		//	//OutputDebugString(animation->mMeshes[0]->mBones[i]->mName.C_Str());
		//	//OutputDebugString((LPCSTR)"\n");
		//	OutputDebugString(animation->mMeshes[0]->mBones[2]->mName.C_Str());
		//	OutputDebugString((LPCSTR)"\n");
		//}

		/*for (size_t i = 0; i < m_boneArray.size(); i++) {
			OutputDebugString(std::to_string(m_boneArray[i].boneID).c_str());
			OutputDebugString((LPCSTR)"\n");
		}*/

		/*OutputDebugString((animation->mRootNode->mChildren[1]->mName).C_Str());
		OutputDebugString((LPCSTR)"\n");
		OutputDebugString(std::to_string(animation->mMeshes[0]->mNumBones).c_str());
		OutputDebugString((LPCSTR)"\n");


		for (size_t i = 0; i < m_boneArray.size(); i++) {
			AEN_PRINT("Bone ID: ");
			AEN_PRINT(m_boneArray[i].boneID);
			AEN_PRINT("	Parent ID: ");
			AEN_PRINT(m_boneArray[i].parentID);
			AEN_ENDL;
			OutputDebugString((LPCSTR)"Bone ID: ");
			OutputDebugString(std::to_string(m_boneArray[i].boneID).c_str());
			OutputDebugString((LPCSTR)"\n");
		}*/
	}

	Animation::~Animation()
	{
	}

	//void Animation::ReadHeirarchyData(AssimpData& destination, const aiNode* source)
	//{
	//	assert(source);

	//	destination.name = source->mName.data;
	//	// CONVERTER destination.transform = source->mTransformation;
	//	destination.childCount = source->mNumChildren;

	//	for (int i = 0; i < source->mNumChildren; i++) {
	//		AssimpData newData;
	//		ReadHeirarchyData(newData, source->mChildren[i]);
	//		destination.children.push_back(newData);
	//	}
	//}
}