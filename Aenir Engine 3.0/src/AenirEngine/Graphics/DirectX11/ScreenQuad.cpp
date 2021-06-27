#include "ScreenQuad.h"

namespace Aen {
	const bool ScreenQuad::Initialize(const Window& window) {

		QuadVB quad[6] = {
			Vec3f(-1.f, -1.f, 0.f), Vec2f(0.f, 1.f),
			Vec3f(-1.f, 1.f, 0.f), Vec2f(0.f, 0.f),
			Vec3f(1.f, 1.f, 0.f), Vec2f(1.f, 0.f),
			Vec3f(-1.f, -1.f, 0.f), Vec2f(0.f, 1.f),
			Vec3f(1.f, 1.f, 0.f), Vec2f(1.f, 0.f),
			Vec3f(1.f, -1.f, 0.f), Vec2f(1.f, 1.f),
		};

		return vBuffer.Initialize(quad, 6);
	}

	void ScreenQuad::Draw() {
		UINT offset = 0;
		dContext->IASetVertexBuffers(0, 1, vBuffer.buffer.GetAddressOf(), vBuffer.stride.get(), &offset);
		dContext->Draw(6, 0);
	}
}