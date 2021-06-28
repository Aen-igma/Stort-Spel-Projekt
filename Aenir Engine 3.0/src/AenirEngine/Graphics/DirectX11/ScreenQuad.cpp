#include "ScreenQuad.h"

namespace Aen {

	ScreenQuad::ScreenQuad() {

		QuadVB quad[4] = {
				Vec3f(-1.f, -1.f, 0.f), Vec2f(0.f, 1.f),
				Vec3f(-1.f, 1.f, 0.f), Vec2f(0.f, 0.f),
				Vec3f(1.f, 1.f, 0.f), Vec2f(1.f, 0.f),
				Vec3f(1.f, -1.f, 0.f), Vec2f(1.f, 1.f)
		};

		m_vBuffer.Create(quad, 4);

		DWORD ind[6]{
			0, 1, 2,
			0, 2, 3
		};

		m_iBuffer.Create(ind, 6);
	}

	void ScreenQuad::Draw() {
		m_vBuffer.SetVBuffer();
		m_iBuffer.SetBuffer();
		m_iBuffer.DrawIndexed();
	}
}