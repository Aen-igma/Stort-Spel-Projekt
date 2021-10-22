#include "PCH.h"
#include "Drawable.h"

namespace Aen {

	Drawable::Drawable(const size_t& id)
		:Component(id) {}

	Drawable::~Drawable() {
	}

	void Drawable::SetLayer(int& layer) {
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