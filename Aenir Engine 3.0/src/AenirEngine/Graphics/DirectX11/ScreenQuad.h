#pragma once
#include"VBuffer.h"
#include"IBuffer.h"

namespace Aen {

	struct QuadVB {
		Vec3f pos;
		Vec2f uv;
	};

	class ScreenQuad {
		public:

		~ScreenQuad() = default;
		ScreenQuad();

		void Draw();

		private:
		VBuffer<QuadVB> m_vBuffer;
		IBuffer m_iBuffer;
	};
}