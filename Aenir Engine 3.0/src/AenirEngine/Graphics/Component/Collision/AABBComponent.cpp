#include "PCH.h"
#include "AABBComponent.h"
#include "../ComponentHandler.h"
#include"Core/Renderer.h"

namespace Aen {

	AABoundBox::AABoundBox(const size_t& id) 
		:Drawable(id),
		#ifdef _DEBUG
			m_canDraw(false)
		#endif
		{}

	const bool AABoundBox::Intersects(AABoundBox& otherBox) const {
		if(m_aabb.Intersects(otherBox.m_aabb))
			return true;
		return false;
	}

	const bool AABoundBox::CreateAABB() {
		size_t vertCount = ComponentHandler::GetMeshInstance(m_id).m_pMesh->m_vertices.GetVertCount();
		size_t vertStride = ComponentHandler::GetMeshInstance(m_id).m_pMesh->m_vertices.GetStride();

		if(!ComponentHandler::GetMeshInstance(m_id).m_pMesh) {
			throw;
			printf("Can't create AABB without mesh");
		}

		DirectX::BoundingBox::CreateFromPoints(m_aabb, vertCount, ComponentHandler::GetMeshInstance(m_id).m_pMesh->mp_posV, vertStride);

		#ifdef _DEBUG
			const size_t size = m_aabb.CORNER_COUNT;
			DirectX::XMFLOAT3 points[size];
			m_aabb.GetCorners(points);
			Vertex verts[8];
			for(uint32_t i = 0; i < size; i++)
				verts[i].pos = Vec3f(points[i].x, points[i].y, points[i].z);

			DWORD ind[24]{
				0, 1, 2,
				2, 1, 3,
				1, 4, 3,
				3, 4, 5,
				4, 6, 5,
				5, 6, 7,
				6, 0, 7,
				7, 0, 2
			};

			if(!m_vBuffer.Create(verts, (UINT)size))
				throw;

			m_iBuffer.Create(ind, 24u);
			m_canDraw = true;
		#endif

		return true;
	}

	const bool AABoundBox::CreateAABB(const Vec3f extents, const Vec3f center) {
		m_aabb.Extents = extents.smVec;
		m_aabb.Center = center.smVec;
		return true;
	}

	void AABoundBox::Update() {
		if(ComponentHandler::RigidExist(m_id))
			m_aabb.Center = ComponentHandler::GetRigid(m_id).GetPos().smVec;
		else if(ComponentHandler::CharacterControllerExist(m_id))
			m_aabb.Center = ComponentHandler::GetCharacterController(m_id).GetPos().smVec;
		else
			m_aabb.Center = ComponentHandler::GetTranslation(m_id).GetPos().smVec;
	}

	AABoundBox::~AABoundBox() {}

	void AABoundBox::Draw(Renderer& renderer, const uint32_t& layer) {
		#ifdef _DEBUG
			if(m_canDraw) {
				renderer.m_cbTransform.GetData().m_mdlMat = MatTranslate(m_aabb.Center.x, m_aabb.Center.y, m_aabb.Center.z).Transposed();
				renderer.m_cbTransform.UpdateBuffer();
				renderer.m_cbTransform.BindBuffer<VShader>(0u);
				RenderSystem::SetRasteriserState(renderer.m_wireFrameState);
				RenderSystem::BindShader(renderer.m_opaqueVS);
				RenderSystem::BindShader(renderer.m_collisionPS);
				RenderSystem::SetInputLayout(renderer.m_opaqueLayout);
				RenderSystem::BindRenderTargetView(renderer.m_backBuffer, renderer.m_depthMap);
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
		#ifdef _DEBUG
			if(m_canDraw) {
				renderer.m_cbTransform.GetData().m_mdlMat = MatTranslate(m_aabb.Center.x, m_aabb.Center.y, m_aabb.Center.z).Transposed();
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