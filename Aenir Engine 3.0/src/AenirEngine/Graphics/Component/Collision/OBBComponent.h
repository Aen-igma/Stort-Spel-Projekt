#pragma once
#include "../Tranform/Tranform.h"
#include "../Drawable/Mesh/Mesh.h"
#include "../Drawable/Drawable.h"
#include "../Component.h"
#include "BuritoMath.h"

class AABoundBox;

namespace Aen
{
	class AEN_DECLSPEC OBBox : public Drawable
	{
	public:
		OBBox(const size_t& id);
		~OBBox();

		const bool Intersects(OBBox& volume);
		const bool Intersects(AABoundBox& volume);
		
		void SetOffset(const Vec3f& offset);
		void SetOffset(const float& x, const float& y, const float& z);

		/// <summary>
		/// Creates OBB based on mesh. If mesh is NULL it will throw.
		/// </summary>
		void SetBoundsToMesh();
		void SetBoundingBox(const Vec3f& extents);
		void SetBoundingBox(const float& x, const float& y, const float& z);

		void Transform(Aen::Mat4f transform);
		void Transform(sm::Matrix transform);
		void SetRotation(Aen::Vec3f v);
		void SetRotation(const float& x, const float& y, const float& z);

		/// <summary>
		/// Used for turning off/on collision checking
		/// </summary>
		/// <param name="b"></param>
		void ToggleActive(bool b);
		/// <summary>
		/// Used for turning off/on collision checking
		/// </summary>
		void ToggleActive();

		const bool GetIsOn() const;
		
	private:
		DirectX::BoundingOrientedBox m_obb;

		Vec3f m_offset;
		void UpdateVerts();
#ifdef _DEBUG
		VBuffer<Vertex> m_vBuffer;
		IBuffer m_iBuffer;
		Vertex m_verts[8];
#endif 
		bool m_canDraw;

		bool m_isColliding;
		bool m_isOn;
		

		// Inherited via Drawable
		virtual void Draw(Renderer& renderer, const uint32_t& layer) override;

		virtual void DepthDraw(Renderer& renderer, const uint32_t& layer) override;
		friend class AABoundBox;

	};
}