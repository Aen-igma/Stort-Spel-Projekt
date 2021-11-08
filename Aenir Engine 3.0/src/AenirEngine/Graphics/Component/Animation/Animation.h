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

namespace Aen {

	struct AssimpData {
		Mat4f transform;
		std::string name;
		int childCount = 0;
		std::vector<AssimpData> children;
	};

	struct Bones {
		int boneID, parentID;
		std::string boneName;
		Mat4f offsetMatrix;			// Inverse bind pose
		//Mat4f transformRelParent;
	};

	struct KeyFrameData {
		std::vector<float> timeStampPos;
		std::vector<float> timeStampRot;

		std::vector<Vec3f> position;
		std::vector<Mat4f> rotation;
	};

	class AEN_DECLSPEC Animation {
	private:
		float m_Duration;
		int m_TicksPerSecond;
		AssimpData m_RootNode;
		//std::map<std::string, BoneInfo> m_BoneInfoMap;
		std::vector<Bones> m_boneArray;
		std::unordered_map<std::string, KeyFrameData> m_keyFrames;
		Mat4f globalInverseTransformMatrix;
		UINT boneCount;
	public:
		Animation();
		Animation(const std::string& animationPath);

		void printTest();
		inline float GetTicksPerSecond() { return m_TicksPerSecond; }
		inline float GetDuration() { return m_Duration; }
		inline const AssimpData& GetRootNode() { return m_RootNode; }
		//inline const std::map<std::string, BondInfo>& GetBoneIDMap() { return m_BoneInfoMap; }
		Vec2f GetTimeFraction(std::vector<float>& times, float& dt);
		void LoadAnimation(const std::string& animationPath);
		void GetPose(Animation& anim, Bones& skele, float dt, std::vector<Mat4f>& output, Mat4f& parentTrans, Mat4f& globalInverseTrans);

	private:
		~Animation();

		//void ReadHeirarchyData(AssimpData& destination, const aiNode* source);

		friend class Resource;
		friend class Renderer;
	};

}
