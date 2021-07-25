#pragma once
#include"../../Graphics.h"

namespace Aen {

	struct SB_SpotLight {
		Color color;
		Vec4f lightDist;
		Vec3f pos;
		float strength;
		Vec3f dir;
		float angle;

		SB_SpotLight()
			:color(Color::White), lightDist(0.4f, 0.02f, 0.f, 100.f), pos(Vec3f::zero), strength(1.f), dir(0.f, 1.f, 0.f), angle(20.f) {}
	};

	class AEN_DECLSPEC SpotLight {
		public:
		SpotLight() = default;

		void SetColor(const Color & color);
		void SetColor(const float& r, const float& g, const float& b, const float& a);
		void SetLightDist(const Vec4f & dist);
		void SetLightDist(const float& attA, const float& attB, const float& attC, const float& dist);
		void SetStrength(const float& strength);
		void SetConeSize(const float& ang);

		private:
		~SpotLight() = default;

		void SetPos(const Vec3f & pos);
		void SetRot(const Mat4f& rot);

		SB_SpotLight m_spotLight;

		friend class ComponentHandler;
		friend class Renderer;
	};

}