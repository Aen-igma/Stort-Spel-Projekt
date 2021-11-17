#pragma once
#include"../../Graphics.h"
#include"Animation.h"
#include "../Drawable/Drawable.h"
#include <chrono>

namespace Aen {

	class AEN_DECLSPEC Animator : public Drawable{
	private:
		/*std::vector<Mat4f> m_FinalBoneMatrices;
		Animation* m_CurrentAnimation;
		float m_CurrentTime;
		float m_DeltaTime;*/
		using ResClock = std::chrono::high_resolution_clock;
		using TimePoint = std::chrono::high_resolution_clock::time_point;
		using DurationLD = std::chrono::duration<long double>;

		TimePoint m_start;
		TimePoint m_end;

		Animation* animation;
		DurationLD frameRate;
		void Update();
	public:
		Animator(const size_t& id);
		void SetAnimation(Animation& anim);
		void SetFrameRate(const int& frameRate);

		/*Animator(Animation& animation);
		void UpdateAnimation(float deltaTime);
		void PlayAnimation(Animation* animation);
		void CalculateBoneTransform(const AssimpData* node, Mat4f parentTransform);
		inline std::vector<Mat4f> GetFinalBoneMatrices() { return m_FinalBoneMatrices; }*/

		friend class ComponentHandler;
		friend class Renderer;
		friend class Entity;

		// Inherited via Drawable
		virtual void Draw(Renderer& renderer, const uint32_t& layer) override;
		virtual void DepthDraw(Renderer& renderer, const uint32_t& layer) override;
	};
}