#include "PCH.h"
#include "AABBComponent.h"
#include "../ComponentHandler.h"
#include"Core/Renderer.h"
#include"Core/GlobalSettings.h"
#include"AABBComponent.h"

namespace Aen {

	AABoundBox::AABoundBox(const size_t& id)
		:Drawable(id), m_offset(Vec3f::zero), m_isColliding(false), m_isOn(false), m_camBox(false)
		#ifdef _DEBUG
		, m_canDraw(false)
		#endif 
		{
		SetBoundsToMesh();

		#ifdef _DEBUG
			DirectX::BoundingBox tBox;
			tBox.Extents = m_aabb.Extents;

			const size_t size = tBox.CORNER_COUNT;
			DirectX::XMFLOAT3 points[size];
			tBox.GetCorners(points);
			for(uint32_t i = 0; i < size; i++)
				m_verts[i].pos = Vec3f(points[i].x, points[i].y, points[i].z);

			DWORD ind[]{
				0, 1, 2, 
				0, 2, 3, 
				1, 5, 6, 
				1, 6, 2, 
				5, 4, 7, 
				5, 7, 6, 
				4, 0, 3, 
				4, 3, 7, 
				6, 3, 2, 
				6, 7, 3, 
				5, 0, 4, 
				5, 1, 0, 
			};

			if(!m_vBuffer.Create(m_verts, (UINT)size, D3D11_USAGE_DYNAMIC))
				throw;

			m_iBuffer.Create(ind, 36u);

			m_canDraw = true;
		#endif

	}
	
	AABoundBox::~AABoundBox() {}

	const bool AABoundBox::Intersects(AABoundBox& otherBox) {
		if (m_aabb.Intersects(otherBox.m_aabb) && m_isOn && otherBox.m_isOn) {
			otherBox.m_isColliding = true;
			m_isColliding = true;
			return true;
		}

		otherBox.m_isColliding = false;
		m_isColliding = false;
		return false;
	}

	const bool AABoundBox::Intersects(OBBox& volume)
	{
		if (m_aabb.Intersects(volume.m_obb) && m_isOn && volume.m_isOn) {
			volume.m_isColliding = true;
			m_isColliding = true;
			return true;
		}

		volume.m_isColliding = false;
		m_isColliding = false;
		return false;
	}

	void AABoundBox::ToggleActive(bool b) {
		m_isOn = b;
	}

	void AABoundBox::ToggleActive() {
		m_isOn = !m_isOn;
	}

	void AABoundBox::SetCamBox(const bool& b) {
		m_camBox = b;
	}

	void AABoundBox::SetBoundsToMesh() {
		if(ComponentHandler::MeshInstanceExist(m_id))
			if(ComponentHandler::GetMeshInstance(m_id).m_pMesh) {
				m_aabb = ComponentHandler::GetMeshInstance(m_id).m_pMesh->m_aabb;

				#ifdef _DEBUG
					DirectX::BoundingBox tBox;
					tBox.Extents = m_aabb.Extents;

					const size_t size = tBox.CORNER_COUNT;
					DirectX::XMFLOAT3 points[size];
					tBox.GetCorners(points);
					for(uint32_t i = 0; i < size; i++)
						m_verts[i].pos = Vec3f(points[i].x, points[i].y, points[i].z);
				#endif
			}
	}

	void AABoundBox::SetBoundingBox(const float& x, const float& y, const float& z) {
		m_aabb.Extents = Vec3f(x, y, z).smVec;

		#ifdef _DEBUG
			DirectX::BoundingBox tBox;
			tBox.Extents = m_aabb.Extents;

			const size_t size = tBox.CORNER_COUNT;
			DirectX::XMFLOAT3 points[size];
			tBox.GetCorners(points);
			for(uint32_t i = 0; i < size; i++)
				m_verts[i].pos = Vec3f(points[i].x, points[i].y, points[i].z);
		#endif
	}

	void AABoundBox::SetBoundingBox(const Vec3f& extents) {
		m_aabb.Extents = extents.smVec;

		#ifdef _DEBUG
			DirectX::BoundingBox tBox;
			tBox.Extents = m_aabb.Extents;

			const size_t size = tBox.CORNER_COUNT;
			DirectX::XMFLOAT3 points[size];
			tBox.GetCorners(points);
			for(uint32_t i = 0; i < size; i++)
				m_verts[i].pos = Vec3f(points[i].x, points[i].y, points[i].z);
		#endif
	}

	void AABoundBox::SetOffset(const Vec3f& offset) {
		m_offset = offset;
	}

	void AABoundBox::SetOffset(const float& x, const float& y, const float& z) {
		m_offset = Vec3f(x, y, z);
	}
	
	void AABoundBox::Draw(Renderer& renderer, const uint32_t& layer) {

		#ifdef _DEBUG
			
			if(m_canDraw) {
				RenderSystem::SetPrimitiveTopology(Topology::TRIANGLELIST);
				Vec3f p = Vec3f(m_aabb.Center.x, m_aabb.Center.y, m_aabb.Center.z);
				renderer.m_cbTransform.GetData().m_mdlMat = MatTranslate(p).Transposed();
				renderer.m_cbTransform.UpdateBuffer();
				renderer.m_cbTransform.BindBuffer<VShader>(0u);
				RenderSystem::SetPrimitiveTopology(Topology::TRIANGLELIST);
				renderer.m_collisionBuffer.BindBuffer<PShader>(0);
				if (!m_isOn) renderer.m_collisionBuffer.GetData().color = { .2f,.2f,.2f };
				else if (m_isColliding) renderer.m_collisionBuffer.GetData().color = { 0.f,1.f,0.f };
				else renderer.m_collisionBuffer.GetData().color = { 1.f,0.f,0.f };
				renderer.m_collisionBuffer.GetData().switcher = 0;
				renderer.m_collisionBuffer.UpdateBuffer();

				RenderSystem::SetRasteriserState(renderer.m_wireFrameState);
				RenderSystem::BindShader(renderer.m_opaqueVS);
				RenderSystem::BindShader(renderer.m_collisionPS);
				RenderSystem::SetInputLayout(renderer.m_opaqueLayout);
				RenderSystem::BindRenderTargetView(renderer.m_backBuffer);
				m_vBuffer.BindBuffer();
				m_iBuffer.BindBuffer();
				m_iBuffer.DrawIndexed();
				RenderSystem::UnBindRenderTargets(1u);
				RenderSystem::UnBindShader<VShader>();
				RenderSystem::UnBindShader<PShader>();
			}
		#endif
	}

	void AABoundBox::DepthDraw(Renderer& renderer, const uint32_t& layer) {

		Vec3f transformation = EntityHandler::GetEntity(m_id).GetTranslation();
		m_aabb.Center = transformation.smVec;

			#ifdef _DEBUG
			RenderSystem::SetPrimitiveTopology(Topology::TRIANGLELIST);

			if(m_camBox) {
				DirectX::XMFLOAT3 points[8u];
				GlobalSettings::GetMainCamera()->GetComponent<Camera>().GetFrustum().GetCorners(points);

				for(uint32_t i = 0u; i < 8u; i++)
					m_verts[i].pos = Vec3f(points[i].x, points[i].y, points[i].z);
			}

			if(m_canDraw) {
				RenderSystem::SetPrimitiveTopology(Topology::TRIANGLELIST);
				m_vBuffer.UpdateBuffer(m_verts, 8);
				Vec3f p = Vec3f(m_aabb.Center.x, m_aabb.Center.y, m_aabb.Center.z);
				renderer.m_cbTransform.GetData().m_mdlMat = (EntityHandler::GetEntity(m_id).GetScaleMat() * MatTranslate(p)).Transposed();
				renderer.m_cbTransform.UpdateBuffer();
				renderer.m_cbTransform.BindBuffer<VShader>(0u);
				RenderSystem::SetInputLayout(renderer.m_opaqueLayout);
				RenderSystem::BindShader<VShader>(renderer.m_opaqueVS);
				RenderSystem::UnBindShader<PShader>();
				m_vBuffer.BindBuffer();
				m_iBuffer.BindBuffer();
				m_iBuffer.DrawIndexed();
			}
			#endif
	}
	bool AABoundBox::FrustumCull(Renderer& renderer)
	{
		Vec3f transformation = EntityHandler::GetEntity(m_id).GetTranslation();
		m_aabb.Center = transformation.smVec;
#ifdef _DEBUG
		if (m_aabb.Intersects(GlobalSettings::GetMainCamera()->GetComponent<Camera>().GetFrustum()))
			return true;
		else
			return false;
#endif
		return false;
	}
}