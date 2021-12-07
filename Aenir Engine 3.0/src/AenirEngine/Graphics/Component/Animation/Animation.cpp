#include "PCH.h"
#include "Animation.h"

#undef min
#include"../AenirEngine/ThirdParty/assimp/include/assimp/Importer.hpp"
#include"../AenirEngine/ThirdParty/assimp/include/assimp/scene.h"
#include"../AenirEngine/ThirdParty/assimp/include/assimp/postprocess.h"
#include"../AenirEngine/ThirdParty/assimp/include/assimp/matrix4x4.h"
#include"../AenirEngine/ThirdParty/assimp/include/assimp/cimport.h"
#include"Importer/AssimpImporter.h"

namespace Aen {

	Animation::Animation() {
	}

	const float Animation::GetDuration() const {
		return m_duration;
	}

	void Animation::LoadAnimation(const std::string& animationPath) {
		AssimpImport::LoadFbxAnimation(animationPath, m_boneArray, m_keyFrames, m_timeStamp, m_duration);

		std::vector<DWORD> indices;
		std::vector<VertexAnimation> verts;
		
		for (int i = 0; i < m_boneArray.size(); i++) {
			if (m_boneArray[i].parentID != -1) {
				indices.emplace_back(m_boneArray[i].parentID);
				indices.emplace_back(m_boneArray[i].boneID);
			}

			VertexAnimation currentV;
			currentV.pos = m_boneArray[i].offsetMatrix.Transposed().Inverse().w;
			currentV.boneId.x = m_boneArray[i].boneID;
			currentV.boneWeights.x = 1.f;
			verts.emplace_back(currentV);
		}

		m_indexBuffer.Create(indices.data(), indices.size());
		if(!vBuff.Create(verts.data(), verts.size()))
			throw;

		m_finalMatrix.Create(m_boneArray.size());
		m_finalMatrix.UpdateBuffer();
	}

	void Animation::CalculateBlendAnimation(Animation* pBase, Animation* pLayer, const float& currentTimeBase, const float& currentTimeLayered, const sm::Matrix& parentTransform, const float& blendFactor)
	{
		// Blanda 2 matriser
		UINT boneArraySize = m_boneArray.size();
		for (int i = 0; i < boneArraySize; i++)
		{
			const sm::Quaternion rot0 = DirectX::XMQuaternionRotationMatrix(pBase->m_boneArray[i].localMatrix.smMat);
			const sm::Quaternion rot1 = DirectX::XMQuaternionRotationMatrix(pLayer->m_boneArray[i].localMatrix.smMat);
			const sm::Quaternion finalRot = DirectX::XMQuaternionSlerp(rot0, rot1, blendFactor);

			const sm::Matrix blendMatrix = DirectX::XMMatrixRotationQuaternion(finalRot);
			Mat4f ncm;
			ncm.smMat = blendMatrix;
			Mat4f blendM(ncm);
			blendM[3].smVec = (1.f - blendFactor) * pBase->m_RootNode.transform[3].smVec + pLayer->m_RootNode.transform[3].smVec * blendFactor;

			const sm::Matrix globalTransformation = parentTransform * blendMatrix;

			//const auto& boneInfoMap = pBase->m_boneArray;
			//if (boneInfoMap[i].boneID != boneInfoMap.back().boneID)
			//{
			//	const sm::Matrix& offset = boneInfoMap[i].offsetMatrix.smMat;
			//	const sm::Matrix& offsetLayer = pLayer->m_boneArray[i].offsetMatrix.smMat;

			//	// Blend again
			//	const sm::Quaternion rot0 = DirectX::XMQuaternionRotationMatrix(offset);
			//	const sm::Quaternion rot1 = DirectX::XMQuaternionRotationMatrix(offsetLayer);
			//	const sm::Quaternion finalRot = DirectX::XMQuaternionSlerp(rot0, rot1, blendFactor);

			//	const sm::Matrix blendMatrix = DirectX::XMMatrixRotationQuaternion(finalRot);
			//	Mat4f blendM(ncm);
			//	blendM[3].smVec = (1.f - blendFactor) * pBase->m_RootNode.transform[3].smVec + pLayer->m_RootNode.transform[3].smVec * blendFactor;

			//	m_finalMatrix.GetData(boneInfoMap[i].boneID).smMat = globalTransformation * blendM.smMat;
			//}
		}
		
	}

	Animation::~Animation() {
	}
}