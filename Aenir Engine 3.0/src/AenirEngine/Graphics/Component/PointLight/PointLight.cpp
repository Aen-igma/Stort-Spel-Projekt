#include"PCH.h"
#include"PointLight.h"

namespace Aen {

	void PointLight::SetColor(const Color& color) {
		m_pointLight.color = color;
	}

	void PointLight::SetColor(const float& r, const float& g, const float& b, const float& a) {
		m_pointLight.color = Color(r, g, b, a);
	}

	void PointLight::SetLightDist(const Vec4f& dist) {
		m_pointLight.lightDist = dist;
	}

	void PointLight::SetLightDist(const float& attA, const float& attB, const float& attC, const float& dist) {
		m_pointLight.lightDist = Vec4f(attA, attB, attC, dist);
	}

	void PointLight::SetStrength(const float& strength) {
		m_pointLight.strength = strength;
	}

	void PointLight::SetPos(const Vec3f& pos) {
		m_pointLight.pos = pos;
	}

}