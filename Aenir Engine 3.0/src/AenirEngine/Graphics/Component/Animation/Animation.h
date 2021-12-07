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
	};

	struct Bones {
		int boneID, parentID;
		std::string boneName;
		Mat4f localMatrix;
		Mat4f offsetMatrix;
	};

	

	class AEN_DECLSPEC Animation {
	private:
		float m_duration;
		
		AssimpData m_RootNode;
		std::vector<Bones> m_boneArray;
		std::unordered_map<std::string, std::vector<KeyFrameData>> m_keyFrames;
		std::vector<float> m_timeStamp;

		VBuffer<VertexAnimation> vBuff;
		IBuffer m_indexBuffer;
		SBuffer<Mat4f> m_finalMatrix;

	public:
		Animation();
		virtual~Animation();
		const float GetDuration() const;
		void LoadAnimation(const std::string& animationPath);
		void CalculateBlendAnimation(Animation* pBase, Animation* pLayer,
			const float& currentTimeBase, const float& currentTimeLayered,
			const sm::Matrix& parentTransform, const float& blendFactor);

	private:

		friend class Resource;
		friend class Renderer;
		friend class Animator;
	};

}
