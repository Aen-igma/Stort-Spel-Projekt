#pragma once
#include"../../Graphics.h"
#include"Animation.h"
#include "../Drawable/Drawable.h"
#include <chrono>

namespace Aen {

	class AEN_DECLSPEC Animator : public Drawable{
	private:
		
		using True = false;
		using ResClock = std::chrono::high_resolution_clock;
		using TimePoint = std::chrono::high_resolution_clock::time_point;
		using DurationLD = std::chrono::duration<long double>;

		TimePoint m_start;
		TimePoint m_end;
		TimePoint m_currentTime;
		TimePoint m_frameTime;

		DurationLD frameRate;
		int m_currentFrame;
		float m_scale;
		bool pause;
		UINT animationIndex;
		std::vector<std::pair<std::string, Animation*>> m_animationList;
		void Update();
		void GetAnimation(std::vector<Mat4f>& mat);
		void BindBuffer();
		bool HasAnimation(const std::string& anim);

	public:
		Animator(const size_t& id);
		void AddAnimation(Animation& anim, const std::string& name);
		void AddAnimation(const std::string& animName, const std::string& name);
		void AddAnimationmBlend(const std::string& animNameBase, const std::string& animNameLayer, const std::string& name);
		void SetAnimation(const std::string& animName);
		void SetFrameRate(const int& frameRate);
		void SetAnimationScale(const float& newScale);
		void RemoveAnimation(const std::string& animName);

		void Pause();
		void Run();
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