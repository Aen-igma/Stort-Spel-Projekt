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

		DurationLD m_frameTime;
		DurationLD m_deltaTime;
		float m_scale;
		bool m_pause;
		bool m_loop;
		UINT animationIndex;
		float m_time;

		std::vector<std::pair<std::string, Animation*>> m_animationList;
		void Update();
		void GetAnimation(std::vector<Mat4f>& mat, const float& deltaTime);
		void BindBuffer();
		bool HasAnimation(const std::string& anim);

	public:
		Animator(const size_t& id);
		void AddAnimation(Animation& anim, const std::string& name);
		void AddAnimation(const std::string& animName, const std::string& name);
		void SetAnimation(const std::string& animName);
		void SetFrameRate(const int& frameRate);
		void SetAnimationScale(const float& newScale);
		void RemoveAnimation(const std::string& animName);

		void SetPaused(const bool& pause);
		void SetLoopAnim(const bool& loop);
		void Reset();

		const Mat4f GetBoneMat(const uint32_t& index);

		friend class ComponentHandler;
		friend class Renderer;
		friend class Entity;
		friend class MeshInstance;

		// Inherited via Drawable
		virtual void Draw(Renderer& renderer, const uint32_t& layer) override;
		virtual void DepthDraw(Renderer& renderer, const uint32_t& layer) override;

		// Inherited via Drawable
		virtual bool FrustumCull(Renderer& renderer) override;
	};
}