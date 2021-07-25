#include"PCH.h"
#include"DirectionalLight.h"

namespace Aen {



	void DirectionalLight::SetColor(const Color& color) {
		m_directionalLight.color = color;
	}

	void DirectionalLight::SetColor(const float& r, const float& g, const float& b, const float& a) {
		m_directionalLight.color = Color(r, g, b, a);
	}

	void DirectionalLight::SetStrength(const float& strength) {
		m_directionalLight.strength = strength;
	}

	void DirectionalLight::SetRot(const Mat4f& rot) {
		m_directionalLight.dir = Transform(rot, Vec3f(0.f, 1.f, 0.f)).Normalized();
	}

}