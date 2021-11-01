#include "PCH.h"
#include "AABBComponent.h"
#include "../ComponentHandler.h"
#include"Core/Renderer.h"

namespace Aen {

	AABoundBox::AABoundBox(const size_t& id) 
		:Drawable(id), m_offset(Vec3f::zero), m_isColliding(false)
		#ifdef _DEBUG
		,m_canDraw(false)
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
		#endif

		m_canDraw = true;
	}
	
	AABoundBox::~AABoundBox() {}

	const bool AABoundBox::Intersects(AABoundBox& otherBox) {
		if (m_aabb.Intersects(otherBox.m_aabb) && m_isOn) {
			otherBox.m_isColliding = true;
			m_isColliding = true;
			return true;
		}

		otherBox.m_isColliding = false;
		m_isColliding = false;
		return false;
	}

	void AABoundBox::ToggleActive(bool b) {
		m_isOn = b;
	}

	void AABoundBox::ToggleActive() {
		m_isOn = !m_isOn;
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

				Vec3f p = Vec3f(m_aabb.Center.x, m_aabb.Center.y, m_aabb.Center.z);
				renderer.m_cbTransform.GetData().m_mdlMat = MatTranslate(p).Transposed();
				renderer.m_cbTransform.UpdateBuffer();
				renderer.m_cbTransform.BindBuffer<VShader>(0u);

				renderer.m_collisionBuffer.BindBuffer<PShader>(0);
				if(m_isColliding) renderer.m_collisionBuffer.GetData().color = {0.f,1.f,0.f};
				else renderer.m_collisionBuffer.GetData().color = {1.f,0.f,0.f};
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

			if(ComponentHandler::RigidExist(m_id))
				m_aabb.Center = ComponentHandler::GetRigid(m_id).GetPos().smVec + m_offset.smVec;
			else if(ComponentHandler::CharacterControllerExist(m_id))
				m_aabb.Center = ComponentHandler::GetCharacterController(m_id).GetPos().smVec + m_offset.smVec;
			else
				m_aabb.Center = ComponentHandler::GetTranslation(m_id).GetPos().smVec + m_offset.smVec;

			#ifdef _DEBUG
			if(m_canDraw) {
				m_vBuffer.UpdateBuffer(m_verts, 8);
				Vec3f p = Vec3f(m_aabb.Center.x, m_aabb.Center.y, m_aabb.Center.z);
				renderer.m_cbTransform.GetData().m_mdlMat = MatTranslate(p).Transposed();
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
}