#pragma once
#include"../../Graphics.h"

namespace Aen {

	struct SB_DirectionalLight {
		Color color;
		Vec3f dir;
		float strength;

		SB_DirectionalLight()
			:color(Color::White), dir(0.f, 1.f, 0.f), strength(1.f) {}
	};

	class AEN_DECLSPEC DirectionalLight {
		public:
		DirectionalLight() = default;

		void SetColor(const Color& color);
		void SetColor(const float& r, const float& g, const float& b, const float& a);
		void SetStrength(const float& strength);

		private:
		~DirectionalLight() = default;

		void SetRot(const Mat4f& rot);

		SB_DirectionalLight m_directionalLight;

		friend class ComponentHandler;
		friend class Renderer;
	};

}