#pragma once
#include"../../Graphics.h"
//#include"ThirdParty/assimp/AssimpIncludes.h"
//#include"ThirdParty/assimp/AssimpIncludes.h"

//#undef min
//#include"../AenirEngine/ThirdParty/assimp/include/assimp/Importer.hpp"
//#include"../AenirEngine/ThirdParty/assimp/include/assimp/scene.h"
//#include"../AenirEngine/ThirdParty/assimp/include/assimp/postprocess.h"
//#include"../AenirEngine/ThirdParty/assimp/include/assimp/matrix4x4.h"
//#include"../AenirEngine/ThirdParty/assimp/include/assimp/cimport.h"
//#undef min
//#include <assimp/Importer.hpp>
//#include <assimp/scene.h>
//#include <assimp/postprocess.h>
//#include "Importer/AssimpImporter.h"

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
		float GetDuration();
		void LoadAnimation(const std::string& animationPath);

	private:
		~Animation();

		friend class Resource;
		friend class Renderer;
		friend class Animator;
	};

}
