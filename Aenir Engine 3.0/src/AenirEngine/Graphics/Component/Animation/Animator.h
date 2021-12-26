#pragma once
#include"../../Graphics.h"
#include"Animation.h"
#include "../Drawable/Drawable.h"
#include <chrono>

namespace Aen {

	class AEN_DECLSPEC Animator : public Drawable{
	private:

		double m_sStart;
		double m_sEnd;
		double m_frameRate;

		float m_scale;
		bool m_pause;
		bool m_loop;

		bool m_hasRunLayer = false;
		bool m_hasActionLayer = false;

		UINT animationIndex;
		UINT m_runIndex;
		UINT m_actionIndex;
		float m_time;
		float m_baseTime;


		std::vector<std::pair<std::string, Animation*>> m_animationList;
		void Update();
		void GetAnimation(std::vector<Mat4f>& mat, const float& deltaTime);
		void BindBuffer();
		bool HasAnimation(const std::string& anim);

		float m_runFactor = 0.f;
		float m_actionFactor = 0.f;
	public:
		Animator(const size_t& id);
		void AddAnimation(Animation& anim, const std::string& name);
		void AddAnimation(const std::string& animName, const std::string& name);
		void SetAnimation(const std::string& animName);
		void SetActionLayer(const std::string& animName);
		void SetRunLayer(const std::string& animName);
		void SetFrameRate(const int& frameRate);
		void SetAnimationScale(const float& newScale);
		void RemoveAnimation(const std::string& animName);

		void SetPaused(const bool& pause);
		void SetLoopAnim(const bool& loop);
		void Reset();

		void SetActionFactor(float f);
		void SetRunFactor(float f);

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