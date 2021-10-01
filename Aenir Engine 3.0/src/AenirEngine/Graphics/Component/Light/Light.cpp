#include "PCH.h"
#include "Light.h"

namespace Aen {

	Light::~Light() {}

	Light::Light(const LightType& type)
		:m_light(Color::White, Vec4f(0.2f, 0.f, 0.02f, 2.f), Vec3f::zero, 1.f, Vec3f(0.f, 1.f, 0.f), 30.f, type) {}

	Light::Light(const LightType& type, const Vec4f& dist)
		:m_light(Color::White, dist, Vec3f::zero, 1.f, Vec3f(0.f, 1.f, 0.f), 30.f, type) {}

	void Light::SetColor(const Color& color) {
		m_light.m_color = color;
	}

	void Light::SetColor(const float& r, const float& g, const float& b, const float& a) {
		m_light.m_color = Color(r, g, b, a);
	}

	void Light::SetStrength(const float& strength) {
		m_light.m_strength = strength;
	}


	SpotLight::SpotLight()
		:Light(LightType::SpotLight, Vec4f(0.1f, 0.f, 0.02f, 5.f)) {}

	void SpotLight::SetLightDist(const Vec4f& dist) {
		m_light.m_dist = dist;
	}

	void SpotLight::SetLightDist(const float& attA, const float& attB, const float& attC, const float& dist) {
		m_light.m_dist = Vec4f(attA, attB, attC, dist);
	}

	void SpotLight::SetConeSize(const float& ang) {
		m_light.m_angle = ang;
	}


	PointLight::PointLight()
		:Light(LightType::PointLight) {}

	void PointLight::SetLightDist(const Vec4f& dist) {
		m_light.m_dist = dist;
	}

	void PointLight::SetLightDist(const float& attA, const float& attB, const float& attC, const float& dist) {
		m_light.m_dist = Vec4f(attA, attB, attC, dist);
	}

	const Vec4f PointLight::GetDist() {
		return m_light.m_dist;
	}

	DirectionalLight::DirectionalLight()
		:Light(LightType::DirectionalLight, Vec4f::zero) {}
}