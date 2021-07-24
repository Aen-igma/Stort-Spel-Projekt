#pragma once
#include"../../Graphics.h"

namespace Aen {

	struct CB_PointLight {
		Color color;
		Vec4f lightDist;
		Vec3f pos;

		CB_PointLight()
			:color(Color::White), lightDist(0.1f, 0.1f, 2.f, 1.3f), pos(Vec3f::zero) {}
	};

	class AEN_DECLSPEC PointLight {
		public:
		PointLight();

		void SetColor(const Color& color);
		void SetColor(const float& r, const float& g, const float& b, const float& a);
		void SetLightDist(const Vec4f& dist);
		void SetLightDist(const float& attA, const float& attB, const float& attC, const float& dist);

		private:
		~PointLight() = default;

		static void Initialize();
		void SetPos(const Vec3f& pos);

		uint32_t m_index;

		static uint32_t m_indexer;
		static SBuffer<CB_PointLight> m_cbPointLight;

		friend class ComponentHandler;
		friend class GameLoop;
		friend class Renderer;
	};

}