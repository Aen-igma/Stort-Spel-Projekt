#include "PCH.h"
#include "AABBComponent.h"
#include "../ComponentHandler.h"
#include"Core/Renderer.h"

namespace Aen {

	AABoundBox::AABoundBox(const size_t& id) 
		:Drawable(id),
		m_isColliding(false),
		#ifdef _DEBUG
			m_canDraw(false)
		#endif
		{}

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

	const bool AABoundBox::CreateAABB() {

		size_t vCount = ComponentHandler::GetMeshInstance(m_id).m_pMesh->m_vertices.GetVertCount();
		size_t vStride = sizeof(DirectX::XMFLOAT3);

		if(!ComponentHandler::GetMeshInstance(m_id).m_pMesh) {
			throw;
			printf("Can't create AABB without mesh");
		}

		std::vector<DirectX::XMFLOAT3> positions;
		positions = ComponentHandler::GetMeshInstance(m_id).m_pMesh->mv_pos;

		m_aabb.CreateFromPoints(m_aabb, vCount, positions.data(), vStride);

		#ifdef _DEBUG
			const size_t size = m_aabb.CORNER_COUNT;
			DirectX::XMFLOAT3 points[size];
			m_aabb.GetCorners(points);
			Vertex verts[8];
			for(uint32_t i = 0; i < size; i++) {
				verts[i].pos = Vec3f(points[i].x, points[i].y, points[i].z);
				AEN_PRINT("Index: ");
				AEN_PRINT(i);
				AEN_PRINT("	");
				AEN_PRINT(verts[i].pos);
				AEN_ENDL;
			}

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

			if(!m_vBuffer.Create(verts, (UINT)size))
				throw;

			m_iBuffer.Create(ind, 36u);
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

				renderer.m_collisionBuffer.BindBuffer<PShader>(0);
				if(m_isColliding) renderer.m_collisionBuffer.GetData().color = { 0.f,1.f,0.f };
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