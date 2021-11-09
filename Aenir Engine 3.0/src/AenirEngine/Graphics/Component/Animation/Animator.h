#pragma once
#include"../../Graphics.h"
#include"Animation.h"
#include "../Drawable/Drawable.h"

namespace Aen {

	class AEN_DECLSPEC Animator : public Drawable{
	private:
		/*std::vector<Mat4f> m_FinalBoneMatrices;
		Animation* m_CurrentAnimation;
		float m_CurrentTime;
		float m_DeltaTime;*/
	public:
		/*Animator(Animation& animation);
		void UpdateAnimation(float deltaTime);
		void PlayAnimation(Animation* animation);
		void CalculateBoneTransform(const AssimpData* node, Mat4f parentTransform);
		inline std::vector<Mat4f> GetFinalBoneMatrices() { return m_FinalBoneMatrices; }*/

		friend class ComponentHandler;
		friend class Renderer;

		// Inherited via Drawable
		virtual void Draw(Renderer& renderer, const uint32_t& id, const uint32_t& layer) override;
		virtual void DepthDraw(Renderer& renderer, const uint32_t& id, const uint32_t& layer) override;
	};
}