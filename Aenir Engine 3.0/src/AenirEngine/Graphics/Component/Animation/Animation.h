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
		Mat4f position;
		Mat4f rotation;
		Mat4f scale;
	};

	struct Bones {
		int boneID, parentID;
		std::string boneName;
		Mat4f localMatrix;
		Mat4f offsetMatrix;			// Inverse bind pose
	};

	

	class AEN_DECLSPEC Animation {
	private:
		float m_Duration;
		int m_TicksPerSecond;
		
		AssimpData m_RootNode;
		//std::map<std::string, BoneInfo> m_BoneInfoMap;
		std::vector<Bones> m_boneArray;
		std::unordered_map<std::string, std::vector<KeyFrameData>> m_keyFrames;
		std::vector<float> m_timeStamp;

		Mat4f globalInverseTransformMatrix;
		UINT boneCount;
		VBuffer<VertexAnimation> vBuff;
		IBuffer m_indexBuffer;
		SBuffer<Mat4f> m_finalMatrix;

	public:
		Animation();
		Animation(const std::string& animationPath);

		inline float GetTicksPerSecond() { return m_TicksPerSecond; }
		inline float GetDuration() { return m_Duration; }
		inline const AssimpData& GetRootNode() { return m_RootNode; }
		Vec2f GetTimeFraction(std::vector<float>& times, float& dt);
		void LoadAnimation(const std::string& animationPath);

	private:
		~Animation();

		friend class Resource;
		friend class Renderer;
		friend class Animator;
	};

}
