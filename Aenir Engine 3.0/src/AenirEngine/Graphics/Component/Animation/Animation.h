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
		Mat4f localMatrix, offsetMatrix;
		std::vector<Bones*> pChildren;
	};

	enum class BlendMode
	{
		BASE_TIME, LAYER_TIME, SCALE
	};
	enum class Action
	{
		Attack, Dash
	};

	class AEN_DECLSPEC Animation {
	private:
		float m_duration;
		int m_blendIndex = 0;
		
		std::vector<Bones> m_boneArray;
		std::unordered_map<std::string, std::vector<KeyFrameData>> m_keyFrames;
		std::vector<float> m_timeStamp;

		VBuffer<VertexAnimation> vBuff;
		IBuffer m_indexBuffer;
		SBuffer<Mat4f> m_finalMatrix;

		std::vector<bool> m_doBlendBone;
		void WhatToBlend(const int& boneIndex);
	public:
		Animation();
		~Animation();
		const float GetDuration() const;
		void LoadAnimation(const std::string& animationPath);
		void SetDoBlendBone(const std::string& root, const bool& reverse = false);
	private:

		friend class Resource;
		friend class Renderer;
		friend class Animator;
	};

}
