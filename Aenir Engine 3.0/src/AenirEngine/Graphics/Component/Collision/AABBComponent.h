#pragma once
#include "../Tranform/Tranform.h"
#include "../Drawable/Mesh/Mesh.h"
#include "../Component.h"
#include "BuritoMath.h"

namespace Aen
{
	class AABoundBox
	{
	public:
		AABoundBox(const size_t& id, Mesh& mesh);
		~AABoundBox();

		void Create();
	private:
		DirectX::BoundingBox m_aabb;

		

	};
}
