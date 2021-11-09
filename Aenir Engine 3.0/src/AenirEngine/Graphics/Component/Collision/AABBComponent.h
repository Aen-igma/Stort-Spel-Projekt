#pragma once
#include "../Tranform/Tranform.h"
#include "../Drawable/Mesh/Mesh.h"
#include "../Drawable/Drawable.h"
#include "../Component.h"
#include "BuritoMath.h"
#include"../Camera/Camera.h"
#include"OBBComponent.h"

namespace Aen
{
	class AEN_DECLSPEC AABoundBox : public Drawable
	{
	public:
		AABoundBox(const size_t& id);
		~AABoundBox();
		const bool Intersects(AABoundBox& otherBox);
		const bool Intersects(OBBox& volume);
		/// <summary>
		/// Creates AABB based on mesh. If mesh is NULL it will throw
		/// </summary>
		/// <returns></returns>
		void SetBoundsToMesh();
		void SetBoundingBox(const Vec3f& extents);
		void SetBoundingBox(const float& x, const float& y, const float& z);

		void SetOffset(const Vec3f& offset);
		void SetOffset(const float& x, const float& y, const float& z);

		void ToggleActive(bool b);
		void ToggleActive();

		void SetCamBox(const bool& b);

	private:
		DirectX::BoundingBox m_aabb;

		Vec3f m_offset;

		#ifdef _DEBUG
		VBuffer<Vertex> m_vBuffer;
		IBuffer m_iBuffer;
		Vertex m_verts[8];
		bool m_canDraw;
		#endif 

		bool m_isColliding;
		bool m_isOn;

		bool m_camBox;

		// Inherited via Drawable
		virtual void Draw(Renderer& renderer, const uint32_t& layer) override;
		virtual void DepthDraw(Renderer& renderer, const uint32_t& layer) override;

		friend class OBBox;

	};
}
