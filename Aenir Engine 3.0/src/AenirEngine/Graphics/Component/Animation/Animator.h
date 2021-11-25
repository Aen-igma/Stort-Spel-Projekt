#pragma once
#include"../../Graphics.h"
#include"Animation.h"
#include "../Drawable/Drawable.h"
#include <chrono>

namespace Aen {

	class AEN_DECLSPEC Animator : public Drawable{
	private:
		
		using ResClock = std::chrono::high_resolution_clock;
		using TimePoint = std::chrono::high_resolution_clock::time_point;
		using DurationLD = std::chrono::duration<long double>;

		TimePoint m_start;
		TimePoint m_end;
		TimePoint m_currentTime;
		TimePoint m_frameTime;

		Animation* animation;
		DurationLD frameRate;
		int m_currentFrame;
		float m_scale;
		void Update();
		void GetAnimation(std::vector<Mat4f>& mat);
		void BindBuffer();

	public:
		Animator(const size_t& id);
		void SetAnimation(Animation& anim);
		void SetFrameRate(const int& frameRate);
		void SetAnimationScale(const float& newScale);


		friend class ComponentHandler;
		friend class Renderer;
		friend class Entity;
		friend class MeshInstance;

		// Inherited via Drawable
		virtual void Draw(Renderer& renderer, const uint32_t& layer) override;
		virtual void DepthDraw(Renderer& renderer, const uint32_t& layer) override;
	};
}