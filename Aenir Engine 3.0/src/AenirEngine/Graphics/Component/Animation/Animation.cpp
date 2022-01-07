#include <PCH.h>
#include "Animation.h"

#undef min
#include"../AenirEngine/ThirdParty/assimp/include/assimp/Importer.hpp"
#include"../AenirEngine/ThirdParty/assimp/include/assimp/scene.h"
#include"../AenirEngine/ThirdParty/assimp/include/assimp/postprocess.h"
#include"../AenirEngine/ThirdParty/assimp/include/assimp/matrix4x4.h"
#include"../AenirEngine/ThirdParty/assimp/include/assimp/cimport.h"
#include"Importer/AssimpImporter.h"

namespace Aen {
	void Animation::WhatToBlend(const int& boneIndex)
	{
		if (boneIndex == -1) return;

		m_doBlendBone[boneIndex] = !m_doBlendBone[boneIndex];
		uint16_t nrof = m_boneArray[boneIndex].pChildren.size();
		for (int i = 0; i < nrof; i++)
		{
			int childIndex = m_boneArray[boneIndex].pChildren[i]->boneID;
			//mp_actionLayer->m_doBlendBone[childIndex] = !layer.m_doBlendBone[childIndex];
			WhatToBlend(childIndex);
		}

	}
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
		if (!vBuff.Create(verts.data(), verts.size()))
			throw;

		m_finalMatrix.Create(m_boneArray.size());
		m_finalMatrix.UpdateBuffer();
	}

	void Animation::SetDoBlendBone(const std::string& root, const bool& reverse)
	{
		int index = -1;
		int nrof = m_boneArray.size();
		for (int i = 0; i < nrof; i++)
		{
			if (m_boneArray[i].boneName == root)
				index = i;
		}
		WhatToBlend(index);

		if (reverse)
		{
			for (int i = 0; i < nrof; i++)
			{
				m_doBlendBone[i] = !m_doBlendBone[i];
			}
		}
	}

	Animation::~Animation() {
		//m_boneArray.clear();
	}


}