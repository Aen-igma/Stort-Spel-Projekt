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
		float m_blendFactor = 0.f;
		bool m_isBlendAnimation = false;
		
		//AssimpData m_RootNode;
		std::vector<Bones> m_boneArray;
		std::unordered_map<std::string, std::vector<KeyFrameData>> m_keyFrames;
		std::vector<float> m_timeStamp;

		VBuffer<VertexAnimation> vBuff;
		IBuffer m_indexBuffer;
		SBuffer<Mat4f> m_finalMatrix;

		Animation* mp_layer = nullptr;
		BlendMode m_bm = BlendMode(0);
		std::vector<bool> m_doBlendBone;
		void WhatToBlend(const int& boneIndex);
	public:
		Animation();
		~Animation();
		const float GetDuration() const;
		void LoadAnimation(const std::string& animationPath);
		void AddAnimationLayer(Animation* pLayer);
		void AddPartialAnimationLayer(Animation* pLayer, const std::string& root);
		/// <summary>
		/// 0.0f - 1.0f
		/// </summary>
		void SetBlendFactor(const float& blendFactor);
		const bool IsBlendAnimation() const;
		const float GetBlendFactor() const;
		const BlendMode GetBlendMode() const;
		void SetBlendMode(const BlendMode& bm);
	private:

		friend class Resource;
		friend class Renderer;
		friend class Animator;
	};

}
