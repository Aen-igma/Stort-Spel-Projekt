#pragma once
#include"VBuffer.h"
#include"CBuffer.h"
#include"IBuffer.h"
#include"DX11Core.h"

namespace Aen {
	struct QuadVB {
		Vec3f pos;
		Vec2f uv;
	};

	class ScreenQuad : public GCore {
		private:
		VBuffer<QuadVB> vBuffer;

		public:

		const bool Initialize(const Aen::Window& window);
		void Draw();
	};
}