#pragma once
#include"../../Graphics.h"

namespace Aen {

	struct VertexAnimation {
		Vec3f pos;
		Vec3f norm;
		Vec3f tan;
		Vec3f bi;
		Vec2f uv;
		Vec4i boneId;
		Vec4f boneWeights;
	};

	struct AssimpData {
		Mat4f transform;
		std::string name;
		int childCount = 0;
		std::vector<AssimpData> children;
	};

	struct KeyFrameData {
		Mat4f rotation;
		Vec4f quatOrientation;
		Vec3f translation;
	};

	struct Bones {
		int boneID, parentID;
		std::string boneName;
		Mat4f localMatrix;
		Mat4f offsetMatrix;
		std::vector<Bones*> pChildren;
	};

	enum class BlendMode
	{
		BASE_TIME, LAYER_TIME, SCALE
	};

	class AEN_DECLSPEC Animation {
	private:
		float m_duration;
		float m_runFactor = 0.f;
		float m_actionFactor = 0.f;
		bool m_hasRunLayer = false;
		bool m_hasActionLayer = false;
		
		std::vector<Bones> m_boneArray;
		std::unordered_map<std::string, std::vector<KeyFrameData>> m_keyFrames;
		std::vector<float> m_timeStamp;

		VBuffer<VertexAnimation> vBuff;
		IBuffer m_indexBuffer;
		SBuffer<Mat4f> m_finalMatrix;

		Animation* mp_runLayer = nullptr;
		Animation* mp_actionLayer = nullptr;
		BlendMode m_bm = BlendMode(0);
		std::vector<bool> m_doBlendBone;
		void WhatToBlend(const int& boneIndex, Animation& pLayer);
		void ReversePartialBlend(Animation* layer);
	public:
		Animation();
		~Animation();
		const float GetDuration() const;
		void LoadAnimation(const std::string& animationPath);
		void AddRunLayer(Animation& pLayer);
		void AddActionLayer(Animation& pLayer);
		void AddPartialActionLayer(Animation& pLayer, const std::string& root, const bool& reverse = false);
		void SetRunFactor(const float& blendFactor);
		void SetActionFactor(const float& blendFactor);
		const bool IsBlendAnimation() const;
		const float GetRunningFactor() const;
		const float GetActionFactor() const;
		const BlendMode GetBlendMode() const;
		void SetBlendMode(const BlendMode& bm);
	private:

		friend class Resource;
		friend class Renderer;
		friend class Animator;
	};

}
