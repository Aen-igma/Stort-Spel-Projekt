#include "PCH.h"
#include "Drawable.h"

namespace Aen {

	Drawable::Drawable() {
		this->m_drawableAABB = new DirectX::BoundingBox(DirectX::XMFLOAT3(0, 0, 0),
			DirectX::XMFLOAT3(1.f, 1.f, 1.f));
	}

	Drawable::~Drawable() {
		//Don't know if deleting here is needed
		delete this->m_drawableAABB;
	}

	DirectX::BoundingBox* Drawable::GetDrawableAABB()
	{
		return this->m_drawableAABB;
	}
}