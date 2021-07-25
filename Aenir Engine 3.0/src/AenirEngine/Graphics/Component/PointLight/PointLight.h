#pragma once
#include"../../Graphics.h"

namespace Aen {
	
	struct SB_PointLight {
		Color color;
		Vec4f lightDist;
		Vec3f pos;
		float strength;

		SB_PointLight()
			:color(Color::White), lightDist(0.1f, 0.1f, 2.f, 1.3f), pos(Vec3f::zero), strength(1.f) {}
	};

	class AEN_DECLSPEC PointLight {
		public:
		PointLight() = default;

		void SetColor(const Color& color);
		void SetColor(const float& r, const float& g, const float& b, const float& a);
		void SetLightDist(const Vec4f& dist);
		void SetLightDist(const float& attA, const float& attB, const float& attC, const float& dist);
		void SetStrength(const float& strength);

		private:
		~PointLight() = default;

		void SetPos(const Vec3f& pos);

		SB_PointLight m_pointLight;

		friend class ComponentHandler;
		friend class Renderer;
	};

}