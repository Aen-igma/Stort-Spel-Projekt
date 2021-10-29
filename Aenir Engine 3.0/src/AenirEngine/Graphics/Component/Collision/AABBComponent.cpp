#include "PCH.h"
#include "AABBComponent.h"
#include "../ComponentHandler.h"
Aen::AABoundBox::AABoundBox(const size_t& id) :Component(id), m_id(id)
{
	

}

const bool Aen::AABoundBox::Intersects(AABoundBox& otherBox) const
{
	if (m_aabb.Intersects(otherBox.m_aabb))
		return true;
	return false;
}

const bool Aen::AABoundBox::CreateAABB()
{
	size_t vertCount = ComponentHandler::GetMeshInstance(m_id).m_pMesh->m_vertices.GetVertCount();
	size_t vertStride = ComponentHandler::GetMeshInstance(m_id).m_pMesh->m_vertices.GetStride();

	if (!ComponentHandler::GetMeshInstance(m_id).m_pMesh) {
		throw;
		printf("Can't create AABB without mesh");
	}

	DirectX::BoundingBox::CreateFromPoints(m_aabb, vertCount, ComponentHandler::GetMeshInstance(m_id).m_pMesh->mp_posV, vertStride);
	return true;
}

const bool Aen::AABoundBox::CreateAABB(const Vec3f extents, const Vec3f center)
{
	m_aabb.Extents = extents.smVec;
	m_aabb.Center = center.smVec;
	return true;
}

void Aen::AABoundBox::Update()
{
	if (ComponentHandler::RigidExist(m_id))
		m_aabb.Center = ComponentHandler::GetRigid(m_id).GetPos().smVec;
	else if (ComponentHandler::CharacterControllerExist(m_id))
		m_aabb.Center = ComponentHandler::GetCharacterController(m_id).GetPos().smVec;
	else
		m_aabb.Center = ComponentHandler::GetTranslation(m_id).GetPos().smVec;
}

Aen::AABoundBox::~AABoundBox()
{
}
