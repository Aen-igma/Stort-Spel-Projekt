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
	const aiScene* animation = importer.ReadFile("../Resource/AnimTimDab.fbx", aiProcess_Triangulate);

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
		const aiScene* animTest = importer.ReadFile("../Resource/AnimTimDab.fbx", aiProcess_Triangulate);
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
					/*OutputDebugString((nodeArray[i]->mName).C_Str());
					OutputDebugString("\n");*/
					/*OutputDebugString((ai_node_bones[i]->mName).C_Str());
					OutputDebugString("\n");
					OutputDebugString((ai_node_bones[i]->mParent->mName).C_Str());
					OutputDebugString("\n");*/
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
		}
	}

	void TransferNodeBoneData(std::vector<aiNode*>& nodeBoneArray, std::vector<Bones>& boneArray, aiMesh* mesh) {
		Bones bone;
		for (size_t i = 0; i < nodeBoneArray.size(); i++) {
			bone.boneName = nodeBoneArray[i]->mName.data;
			bone.boneID = i;
			for (size_t j = 0; j < nodeBoneArray.size(); j++) {
				if (nodeBoneArray[i]->mParent[0].mParent[0].mParent[0].mParent[0].mParent[0].mName.data == nodeBoneArray[j]->mName.data)
					bone.parentID = j;
			}
			//OutputDebugString((nodeBoneArray[i]->mParent[0].mParent[0].mParent[0].mParent[0].mParent[0].mName).C_Str());
			/*if (bone.boneID == 0)
				bone.parentID = -1;*/

			bone.offsetMatrix.a11 = mesh->mBones[i]->mOffsetMatrix.a1; bone.offsetMatrix.a12 = mesh->mBones[i]->mOffsetMatrix.a2; 
			bone.offsetMatrix.a13 = mesh->mBones[i]->mOffsetMatrix.a3; bone.offsetMatrix.a14 = mesh->mBones[i]->mOffsetMatrix.a4;

			bone.offsetMatrix.a21 = mesh->mBones[i]->mOffsetMatrix.b1; bone.offsetMatrix.a22 = mesh->mBones[i]->mOffsetMatrix.b2;
			bone.offsetMatrix.a23 = mesh->mBones[i]->mOffsetMatrix.b3; bone.offsetMatrix.a24 = mesh->mBones[i]->mOffsetMatrix.b4;

			bone.offsetMatrix.a31 = mesh->mBones[i]->mOffsetMatrix.c1; bone.offsetMatrix.a32 = mesh->mBones[i]->mOffsetMatrix.c2;
			bone.offsetMatrix.a33 = mesh->mBones[i]->mOffsetMatrix.c3; bone.offsetMatrix.a34 = mesh->mBones[i]->mOffsetMatrix.c4;

			bone.offsetMatrix.a41 = mesh->mBones[i]->mOffsetMatrix.d1; bone.offsetMatrix.a42 = mesh->mBones[i]->mOffsetMatrix.d2;
			bone.offsetMatrix.a43 = mesh->mBones[i]->mOffsetMatrix.d3; bone.offsetMatrix.a44 = mesh->mBones[i]->mOffsetMatrix.d4;

			boneArray.emplace_back(bone);
		}
		for (int i = 0; i < boneArray.size(); i++) {
			OutputDebugString(" BONE NAME ");
			OutputDebugString((boneArray[i].boneName).c_str());
			OutputDebugString(" BONE ID ");
			OutputDebugString(std::to_string(boneArray[i].boneID).c_str());
			OutputDebugString(" PARENT ID ");
			OutputDebugString(std::to_string(boneArray[i].parentID).c_str());
			OutputDebugString("\n");
		}
	}

	void AnimProcess() {
		if (animation->mNumAnimations == 0) {
			OutputDebugString("No Animations!");
			return;
		}
		for (int i = 0; i < animation->mAnimations[0]->mNumChannels; i++) {
			ai_nodes_anim.emplace_back(animation->mAnimations[0]->mChannels[i]);
			Vec4f test;
			aiQuaternion orient;															// Try to find out if this is the correct way of getting rotaions from the animation keyframes
																							// Also, try to find out how to use quaternions or conversion
			test.x = animation->mAnimations[0]->mChannels[i]->mPositionKeys[11].mValue.x;
			test.y = animation->mAnimations[0]->mChannels[i]->mPositionKeys[11].mValue.y;
			test.z = animation->mAnimations[0]->mChannels[i]->mPositionKeys[11].mValue.z;
			//orient.w = animation->mAnimations[0]->mChannels[i]->mPositionKeys[20].mValue.w;
			OutputDebugString(std::to_string(test.x).c_str());
			OutputDebugString(" : ");		
			OutputDebugString(std::to_string(test.y).c_str());
			OutputDebugString(" : ");		
			OutputDebugString(std::to_string(test.z).c_str());
			/*OutputDebugString(" : ");		
			OutputDebugString(std::to_string(orient.w).c_str());*/
			OutputDebugString("\n");
		}
	}

	//void GetBoneOffsetMatrix(const std::vector<aiBone*>& boneArray)

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
		AssignBones(animation, ai_nodes, m_boneArray);
		TransferNodeBoneData(ai_node_bones, m_boneArray, animation->mMeshes[0]);
		MeshBoneData(animation->mMeshes[0], ai_bone_data);
		//TransferBones(ai_bone_data, m_boneArray);
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