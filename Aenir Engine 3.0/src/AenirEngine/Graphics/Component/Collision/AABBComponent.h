#pragma once
#include "../Tranform/Tranform.h"
#include "../Drawable/Mesh/Mesh.h"
#include "../Drawable/Drawable.h"
#include "../Component.h"
#include "BuritoMath.h"

namespace Aen
{
	class AEN_DECLSPEC AABoundBox : public Drawable
	{
	public:
		AABoundBox(const size_t& id);
		const bool Intersects(AABoundBox& otherBox);
		/// <summary>
		/// Creates AABB based on mesh. If mesh is NULL it will throw
		/// </summary>
		/// <returns></returns>
		const bool CreateAABB();
		const bool CreateAABB(const Vec3f extents, const Vec3f center);
		void Update();
		~AABoundBox();
	private:
		DirectX::BoundingBox m_aabb;

		Vec3f m_positionOffset;
		Vec3f m_sizeOffset;

		bool m_isColliding;

		#ifdef _DEBUG
			bool m_canDraw;
			VBuffer<Vertex> m_vBuffer;
			IBuffer m_iBuffer;
		#endif 


		// Inherited via Drawable
		virtual void Draw(Renderer& renderer, const uint32_t& layer) override;
		virtual void DepthDraw(Renderer& renderer, const uint32_t& layer) override;

	};
}
