#include "PCH.h"
#include "Animation.h"

#undef min
#include"../AenirEngine/ThirdParty/assimp/include/assimp/Importer.hpp"
#include"../AenirEngine/ThirdParty/assimp/include/assimp/scene.h"
#include"../AenirEngine/ThirdParty/assimp/include/assimp/postprocess.h"
#include"../AenirEngine/ThirdParty/assimp/include/assimp/matrix4x4.h"
#include"../AenirEngine/ThirdParty/assimp/include/assimp/cimport.h"

namespace Aen {

	/*Assimp::Importer importer;
	const aiScene* animation = importer.ReadFile("../Resource/SkelAniSkel.fbx", aiProcess_Triangulate);*/

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
		//Assimp::Importer importer;
		////importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false);
		//const aiScene* animTest = importer.ReadFile("../Resource/SkelAniSkel.fbx", aiProcess_Triangulate);
		//assert(animTest && animTest->mRootNode);
		//m_Duration = animTest->mAnimations[0]->mDuration;
		//m_TicksPerSecond = animTest->mAnimations[0]->mTicksPerSecond;
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
			if (node->mNumChildren > 0) {
				RecursiveNodeProcess(node->mChildren[i]);
			}
		}
	}

	const Mat4f RecursiveWorldMatrix(const Bones& bone, std::vector<Bones>& boneArray) {
		Mat4f parentMat;
		if (bone.parentID != -1) {
			parentMat = RecursiveWorldMatrix(boneArray[bone.parentID], boneArray);
		}
		return parentMat * bone.offsetMatrix.Inverse();
	}

	void AssignBones(const aiScene* scene, std::vector<aiNode*>& nodeArray, std::vector<Bones>& boneArray) {
		for (int i = 0; i < nodeArray.size(); i++) {
			for (int j = 0; j < scene->mMeshes[0]->mNumBones; j++) {
				if (nodeArray[i]->mName == scene->mMeshes[0]->mBones[j]->mName) {
					//boneArray.emplace_back(scene->mMeshes[0]->mBones[j]);
					ai_node_bones.emplace_back(nodeArray[i]);
				}
			}
		}
	}

	/*int FindParentID(const aiNode* parentNode, std::vector<aiNode*>& boneNodeArray) {
		for (int i = 0; i < boneNodeArray.size(); i++) {
			if (parentNode->mName == boneNodeArray[i]->mName) {
				return i;
			}
		}
	}*/

	void MeshBoneData(const aiMesh* meshNode, std::vector<aiBone*>& boneArray) {
		for (int i = 0; i < meshNode->mNumBones; i++) {
			boneArray.emplace_back(meshNode->mBones[i]);
		}
	}

	/*void TransferBones(std::vector<aiBone*>& boneArray, std::vector<Bones>& boneArrayFinal) {
		Bones bone;
		for (int i = 0; i < boneArray.size(); i++) {
			bone.boneID = i;
			bone.boneName = boneArray[i]->mName.data;
		}
	}*/

	void TransferNodeBoneData(std::vector<aiNode*>& nodeBoneArray, std::vector<Bones>& boneArray, aiMesh* mesh) {
		Bones bone;
		for (size_t i = 0; i < nodeBoneArray.size(); i++) {
			bone.boneName = nodeBoneArray[i]->mName.data;
			bone.boneID = i;
			for (size_t j = 0; j < nodeBoneArray.size(); j++) {
				if (bone.boneID == 0) {
					bone.parentID = -1;
					break;
				}
				/*if (nodeBoneArray[i]->FindNode(nodeBoneArray[j]->mName.data)) {
					OutputDebugString(std::to_string(j).c_str());
					OutputDebugString("\n");
				}*/
				if (nodeBoneArray[i]->mParent[0].mName.data == nodeBoneArray[j]->mName.data) {
					bone.parentID = j;
				}
			}
			//bone.localMatrix = nodeBoneArray[i]->mTransformation;
			// 

			// --------- LOCAL MATRIX ----------- //
			bone.localMatrix.a11 = nodeBoneArray[i]->mTransformation.a1; bone.localMatrix.a12 = nodeBoneArray[i]->mTransformation.a2;
			bone.localMatrix.a13 = nodeBoneArray[i]->mTransformation.a3; bone.localMatrix.a14 = nodeBoneArray[i]->mTransformation.a4;

			bone.localMatrix.a21 = nodeBoneArray[i]->mTransformation.b1; bone.localMatrix.a22 = nodeBoneArray[i]->mTransformation.b2;
			bone.localMatrix.a23 = nodeBoneArray[i]->mTransformation.b3; bone.localMatrix.a24 = nodeBoneArray[i]->mTransformation.b4;

			bone.localMatrix.a31 = nodeBoneArray[i]->mTransformation.c1; bone.localMatrix.a32 = nodeBoneArray[i]->mTransformation.c2;
			bone.localMatrix.a33 = nodeBoneArray[i]->mTransformation.c3; bone.localMatrix.a34 = nodeBoneArray[i]->mTransformation.c4;

			bone.localMatrix.a41 = nodeBoneArray[i]->mTransformation.d1; bone.localMatrix.a42 = nodeBoneArray[i]->mTransformation.d2;
			bone.localMatrix.a43 = nodeBoneArray[i]->mTransformation.d3; bone.localMatrix.a44 = nodeBoneArray[i]->mTransformation.d4;
			

			// --------- OFFSET MATRIX ----------- //
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
	}

	void AnimProcess(std::unordered_map<std::string, std::vector<KeyFrameData>>& keyFrameData, const aiScene* scene, float& duration, std::vector<float>& timeStamp) {
		if (scene->mNumAnimations == 0) {
			OutputDebugString("No Animations!");
			return;
		}
		//OutputDebugString(std::to_string(scene->mAnimations[0]->mChannels[i]->mPositionKeys[j].mTime).c_str());
		//OutputDebugString("\n");

		duration = (scene->mAnimations[0]->mDuration + 1.f) / scene->mAnimations[0]->mTicksPerSecond;

		for (int i = 0; i < scene->mAnimations[0]->mNumChannels; i+=3) {
			OutputDebugString((scene->mAnimations[0]->mChannels[i]->mNodeName).C_Str());
			OutputDebugString("\n");

			int i1 = i;
			int i2 = i + 1;
			int i3 = i + 2;

			std::string nameOfChannel;
			std::string temp((scene->mAnimations[0]->mChannels[i]->mNodeName).C_Str());
			int len2;
			len2 = temp.find_first_of("$");
			for (int l = 0; l < len2 - 1; l++) {
				nameOfChannel += temp[l];
			}

			UINT posCount = scene->mAnimations[0]->mChannels[i1]->mNumPositionKeys;
			UINT rotCount = scene->mAnimations[0]->mChannels[i2]->mNumRotationKeys;
			UINT scaleCount = scene->mAnimations[0]->mChannels[i3]->mNumScalingKeys;

			UINT keys = posCount;
			keys = (keys < rotCount) ? rotCount : keys;
			keys = (keys < scaleCount) ? scaleCount : keys;

			std::vector<KeyFrameData> data(keys);
			timeStamp.resize(keys);
			float keysPerSec = duration / keys;

			// Position key data
			for (int j = 0; j < posCount; j++) {
				int index = scene->mAnimations[0]->mChannels[i1]->mPositionKeys[j].mTime;
				if (index >= 0) {
					Vec3f posValues;
					posValues.x = scene->mAnimations[0]->mChannels[i1]->mPositionKeys[j].mValue.x;
					posValues.y = scene->mAnimations[0]->mChannels[i1]->mPositionKeys[j].mValue.y;
					posValues.z = scene->mAnimations[0]->mChannels[i1]->mPositionKeys[j].mValue.z;
					data[index].position = MatTranslate(posValues);
					timeStamp[index] = keysPerSec * (float)index;
				}
			}

			 //Rotation key data
			for (int r = 0; r < rotCount; r++) {
				int index = scene->mAnimations[0]->mChannels[i2]->mRotationKeys[r].mTime;
				if (index >= 0) {
					aiQuaternion orient;
					orient = scene->mAnimations[0]->mChannels[i2]->mRotationKeys[r].mValue;
					data[index].rotation = MatQuaternion(orient.x, orient.y, orient.z, orient.w);
				}
			}	
			
			//// Scale key data
			for (int s = 0; s < scaleCount; s++) {
				int index = scene->mAnimations[0]->mChannels[i3]->mScalingKeys[s].mTime;
				if (index >= 0) {
					Vec3f scaleValues;
					scaleValues.x = scene->mAnimations[0]->mChannels[i3]->mScalingKeys[s].mValue.x;
					scaleValues.y = scene->mAnimations[0]->mChannels[i3]->mScalingKeys[s].mValue.y;
					scaleValues.z = scene->mAnimations[0]->mChannels[i3]->mScalingKeys[s].mValue.z;
					data[index].scale = MatScale(scaleValues);
				}
			}

			keyFrameData.emplace(nameOfChannel, data);
		}
	}

	Vec2f Animation::GetTimeFraction(std::vector<float>& times, float& dt)
	{
		UINT segment = 0;
		float start, end, fraction;
		while (dt > times[segment])
			segment++;
		start = times[segment - 1];
		end = times[segment];
		fraction = (dt - start) / (end - start);

		return Vec2f(segment, fraction);
	}

	void Animation::LoadAnimation(const std::string& animationPath)
	{
		Assimp::Importer importer;
		importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false);
		const aiScene* animation = importer.ReadFile("../Resource/" + animationPath, aiProcess_Triangulate | aiProcess_MakeLeftHanded);
		//----------------------BONE DATA--------------------------//
		Bones rootBone;
		/*rootBone.boneID = 0;
		rootBone.parentID = -1;
		rootBone.boneName = "GROOT";*/
		
		assert(animation && animation->mRootNode);
		auto rootBoneNode = animation->mMeshes[0]->mBones[0];
		//m_boneArray.emplace_back(rootBone);
		FindRootBone(rootBone, animation, m_boneArray);
		
		/*globalInverseTransformMatrix.a11 = animation->mRootNode->mTransformation.a1; globalInverseTransformMatrix.a12 = animation->mRootNode->mTransformation.a2;
		globalInverseTransformMatrix.a13 = animation->mRootNode->mTransformation.a3; globalInverseTransformMatrix.a14 = animation->mRootNode->mTransformation.a4;

		globalInverseTransformMatrix.a21 = animation->mRootNode->mTransformation.b1; globalInverseTransformMatrix.a22 = animation->mRootNode->mTransformation.b2;
		globalInverseTransformMatrix.a23 = animation->mRootNode->mTransformation.b3; globalInverseTransformMatrix.a24 = animation->mRootNode->mTransformation.b4;

		globalInverseTransformMatrix.a31 = animation->mRootNode->mTransformation.c1; globalInverseTransformMatrix.a32 = animation->mRootNode->mTransformation.c2;
		globalInverseTransformMatrix.a33 = animation->mRootNode->mTransformation.c3; globalInverseTransformMatrix.a34 = animation->mRootNode->mTransformation.c4;

		globalInverseTransformMatrix.a41 = animation->mRootNode->mTransformation.d1; globalInverseTransformMatrix.a42 = animation->mRootNode->mTransformation.d2;
		globalInverseTransformMatrix.a43 = animation->mRootNode->mTransformation.d3; globalInverseTransformMatrix.a44 = animation->mRootNode->mTransformation.d4;

		globalInverseTransformMatrix.Inverse();*/
		boneCount = animation->mMeshes[0]->mNumBones;

		RecursiveNodeProcess(animation->mRootNode);
		AssignBones(animation, ai_nodes, m_boneArray);
		TransferNodeBoneData(ai_node_bones, m_boneArray, animation->mMeshes[0]);
		AnimProcess(m_keyFrames, animation, m_Duration, m_timeStamp);
		MeshBoneData(animation->mMeshes[0], ai_bone_data);
		//TransferBones(ai_bone_data, m_boneArray);
		
		std::vector<DWORD> indices;
		std::vector<VertexAnimation> verts;
		
		for (int i = 0; i < m_boneArray.size(); i++) {
			if (m_boneArray[i].parentID != -1) {
				indices.emplace_back(m_boneArray[i].parentID);
				indices.emplace_back(m_boneArray[i].boneID);
			}
			VertexAnimation currentV;
			currentV.pos = m_boneArray[i].offsetMatrix.Transposed().Inverse().a;
			currentV.boneId.x = m_boneArray[i].boneID;
			currentV.boneWeights.x = 1.f;
			verts.emplace_back(currentV);
			
		}

			// creating index, vertex and structured buffers
		m_indexBuffer.Create(indices.data(), indices.size());
		if(!vBuff.Create(verts.data(), verts.size()))
			throw;

		m_finalMatrix.Create(m_boneArray.size());
		

		//----------------------ANIMATION DATA--------------------------//

	}

	//void Animation::GetPose(Animation& anim, Bones& skele, float dt, std::vector<Mat4f>& output, Mat4f& parentTrans, Mat4f& globalInverseTrans)
	//{
	//	KeyFrameData& keyFrameData = anim.m_keyFrames[skele.boneName];
	//	dt = fmod(dt, anim.m_Duration);

	//	std::pair<UINT, float> fp;

	//	// Position
	//	fp.first = GetTimeFraction(keyFrameData.timeStampPos, dt).x;
	//	fp.second = GetTimeFraction(keyFrameData.timeStampPos, dt).y;

	//	Vec3f position1 = keyFrameData.position[fp.first - 1];
	//	Vec3f position2 = keyFrameData.position[fp.first];
	//	Vec3f position = position1 * (1.0 - fp.second) + position2 * fp.second;

	//	 //Rotation
	//	fp.first = GetTimeFraction(keyFrameData.timeStampRot, dt).x;
	//	fp.second = GetTimeFraction(keyFrameData.timeStampRot, dt).y;

	//	Mat4f rotation1 = keyFrameData.rotation[fp.first - 1];
	//	Mat4f rotation2 = keyFrameData.rotation[fp.first];
	//	Mat4f rotation = rotation1 * (1.0 - fp.second) + rotation2 * fp.second;

	//	Mat4f positionMatrix = positionMatrix.identity;
	//	
	//	positionMatrix.a14 = position.x;
	//	positionMatrix.a24 = position.y;
	//	positionMatrix.a34 = position.z;
	//	positionMatrix.a44 = 1;

	//	Mat4f localTransform = positionMatrix * rotation;
	//	Mat4f globalTransform = parentTrans * localTransform;

	//	output[skele.boneID] = globalInverseTrans * globalTransform * skele.offsetMatrix;

	//	for (Bones& child : m_boneArray) {
	//		GetPose(anim, child, dt, output, globalTransform, globalInverseTrans);
	//	}

	//}

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