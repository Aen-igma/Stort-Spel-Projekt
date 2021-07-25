#include"PCH.h"
#include"SpotLight.h"

namespace Aen {

	void SpotLight::SetColor(const Color& color) {
		m_spotLight.color = color;
	}

	void SpotLight::SetColor(const float& r, const float& g, const float& b, const float& a) {
		m_spotLight.color = Color(r, g, b, a);
	}

	void SpotLight::SetLightDist(const Vec4f& dist) {
		m_spotLight.lightDist = dist;
	}

	void SpotLight::SetLightDist(const float& attA, const float& attB, const float& attC, const float& dist) {
		m_spotLight.lightDist = Vec4f(attA, attB, attB, dist);
	}

	void SpotLight::SetStrength(const float& strength) {
		m_spotLight.strength = strength;
	}

	void SpotLight::SetConeSize(const float& ang) {
		m_spotLight.angle = ang;
	}

	void SpotLight::SetPos(const Vec3f& pos) {
		m_spotLight.pos = pos;
	}

	void SpotLight::SetRot(const Mat4f& rot) {
		m_spotLight.dir = Transform(rot, Vec3f(0.f, 1.f, 0.f)).Normalized();
	}

}