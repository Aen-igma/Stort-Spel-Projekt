#include"PCH.h"
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
			0, 2, 1,
			0, 3, 2,
		};

		m_iBuffer.Create(ind, 6);
	}

	void ScreenQuad::Draw() {
		m_vBuffer.BindBuffer();
		m_iBuffer.BindBuffer();
		m_iBuffer.DrawIndexed();
	}
}