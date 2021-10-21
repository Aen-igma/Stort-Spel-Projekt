#include "PCH.h"
#include "Drawable.h"

namespace Aen {

	Drawable::Drawable() :m_layer(0u) {
		m_boundingBox = DirectX::BoundingBox(DirectX::XMFLOAT3(0.f, 0.f, 0.f), DirectX::XMFLOAT3(1.f, 1.f, 1.f));
	}

	Drawable::~Drawable() {}

	void Drawable::SetLayer(const int& layer) {
		m_layer = Clamp(layer, -3, 3);
	}

	const int& Drawable::GetLayer()
	{
		return m_layer;
	}

	const DirectX::BoundingBox& Drawable::GetAABB()
	{
		return m_boundingBox;
	}
}