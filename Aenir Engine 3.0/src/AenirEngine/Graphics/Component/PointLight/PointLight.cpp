#include"PCH.h"
#include"PointLight.h"

namespace Aen {

	uint32_t PointLight::m_indexer(0u);
	SBuffer<CB_PointLight> PointLight::m_cbPointLight;

	PointLight::PointLight()
		:m_index(m_indexer) {
		m_indexer++;
	}

	void PointLight::SetColor(const Color& color) {
		m_cbPointLight.GetData(m_index).color = color;
	}

	void PointLight::SetColor(const float& r, const float& g, const float& b, const float& a) {
		m_cbPointLight.GetData(m_index).color = Color(r, g, b, a);
	}

	void PointLight::SetLightDist(const Vec4f& dist) {
		m_cbPointLight.GetData(m_index).lightDist = dist;
	}

	void PointLight::SetLightDist(const float& attA, const float& attB, const float& attC, const float& dist) {
		m_cbPointLight.GetData(m_index).lightDist = Vec4f(attA, attB, attC, dist);
	}

	void PointLight::Initialize() {
		m_cbPointLight.Create(300);
	}

	void PointLight::SetPos(const Vec3f& pos) {
		m_cbPointLight.GetData(m_index).pos = pos;
	}

}