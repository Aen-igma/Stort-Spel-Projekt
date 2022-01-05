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
	//void Animation::WhatToBlend(const int& boneIndex, Animation& layer)
	//{
	//	//uint16_t bonearrSize = m_boneArray.size();
	//	layer.m_doBlendBone[boneIndex] = !layer.m_doBlendBone[boneIndex];
	//	uint16_t nrof = m_boneArray[boneIndex].pChildren.size();
	//	for (int i = 0; i < nrof; i++)
	//	{
	//		int childIndex = m_boneArray[boneIndex].pChildren[i]->boneID;
	//		//mp_actionLayer->m_doBlendBone[childIndex] = !layer.m_doBlendBone[childIndex];
	//		WhatToBlend(childIndex, layer);
	//	}

	//}
	//void Animation::ReversePartialBlend(Animation* layer)
	//{
	//	int nrof = m_doBlendBone.size();
	//	for (int i = 0; i < nrof; i++)
	//	{
	//		layer->m_doBlendBone[i] = !layer->m_doBlendBone[i];
	//	}
	//}
	Animation::Animation() {
	}

	//Animation::Animation(const Animation& s)
	//	:m_duration(s.m_duration), m_boneArray(s.m_boneArray), m_timeStamp(s.m_timeStamp)
	//{
	//}

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

	//void Animation::AddRunLayer(Animation& pLayer)
	//{
	//	mp_runLayer = &pLayer;
	//	m_hasRunLayer = true;
	//}
	//void Animation::AddActionLayer(Animation& pLayer)
	//{
	//	pLayer.m_doBlendBone.resize(m_boneArray.size(), true);
	//	mp_actionLayer.emplace_back(&pLayer);
	//	//mp_actionLayer = &pLayer;
	//	m_hasActionLayer = true;
	//}
	//void Animation::AddPartialActionLayer(Animation& layer, const std::string& root, const bool& reverse)
	//{
	//	//mp_actionLayer = &layer;
	//	m_hasActionLayer = true;
	//	mp_actionLayer.emplace_back(&layer);
	//	int boneArrSize = layer.m_boneArray.size();
	//	layer.m_doBlendBone.resize(boneArrSize, reverse);
	//	int rootIndex = -1;
	//	
	//	for (int i = 0; i < boneArrSize && rootIndex == -1; i++)
	//	{
	//		if (m_boneArray[i].boneName == root)
	//		{
	//			rootIndex = i;
	//		}
	//	}
	//	assert(rootIndex != -1); // Joint does not exist

	//	WhatToBlend(rootIndex, layer);
	//	layer.m_doBlendBone[rootIndex] = true;

	//	//if (reverse)
	//		//ReversePartialBlend(&layer);
	//}
	//void Animation::SetRunFactor(const float& blendFactor)
	//{
	//	m_runFactor = blendFactor;
	//}
	//void Animation::SetActionFactor(const float& blendFactor)
	//{
	//	m_actionFactor = blendFactor;
	//}
	//const bool Animation::IsBlendAnimation() const
	//{
	//	return m_hasRunLayer;
	//}
	//const float Animation::GetRunningFactor() const
	//{
	//	if (m_hasRunLayer)
	//		return m_runFactor;
	//	else return 0.f;
	//}
	//const float Animation::GetActionFactor() const
	//{
	//	return m_actionFactor;
	//}
	//const BlendMode Animation::GetBlendMode() const
	//{
	//	return m_bm;
	//}
	//void Animation::SetBlendMode(const BlendMode& bm)
	//{
	//	m_bm = bm;
	//}
	//const int Animation::GetBlendIndex() const
	//{
	//	return m_blendIndex;
	//}
	//void Animation::SetBlendAnimation(const Action& ac)
	//{
	//	m_blendIndex = (int)ac;
	//}
	Animation::~Animation() {
		//m_boneArray.clear();
	}
}