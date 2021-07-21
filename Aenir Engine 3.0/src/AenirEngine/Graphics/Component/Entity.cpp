#include "PCH.h"
#include "Entity.h"

namespace Aen {

	uint32_t Entity::m_iDs(0);

	Entity::~Entity() {
		TransformHandler::RemoveTranform(m_id);
		CameraHandler::RemoveCamera(m_id);
		MeshIHandler::RemoveMeshInstance(m_id);
		MaterialIHandler::RemoveMaterial(m_id);

		EntityHandler::RemoveEntity(m_id);
	}

	Entity::Entity()
		:m_id(m_iDs), m_parentId(-1), m_hasParent(false) {
		EntityHandler::m_entities.emplace(m_id, this);
		m_iDs++;
	}

	void Entity::SetParent(Entity& parent) {
		m_hasParent = true;
		m_parentId = parent.m_id;
	}

	void Entity::RemoveParent() {
		m_hasParent = false;
		m_parentId = -1;
	}

	void Entity::SetPos(const Vec3f& pos) {
		TransformHandler::GetTranslation(m_id).SetPos(pos);
	}

	void Entity::SetPos(const float& x, const float& y, const float& z) {
		TransformHandler::GetTranslation(m_id).SetPos(x, y, z);
	}

	void Entity::Move(const Vec3f& pos) {
		TransformHandler::GetTranslation(m_id).Move(pos);
	}

	void Entity::Move(const float& x, const float& y, const float& z) {
		TransformHandler::GetTranslation(m_id).Move(x, y, z);
	}

	void Entity::SetRot(const Vec3f& rot) {
		TransformHandler::GetRotation(m_id).SetRot(rot);
	}

	void Entity::SetRot(const float& p, const float& y, const float& r) {
		TransformHandler::GetRotation(m_id).SetRot(p, y, r);
	}

	void Entity::Rotate(const Vec3f& rot) {
		TransformHandler::GetRotation(m_id).Rotate(rot);
	}

	void Entity::Rotate(const float& p, const float& y, const float& r) {
		TransformHandler::GetRotation(m_id).Rotate(p, y, r);
	}

	void Entity::SetScale(const Vec3f& scale) {
		TransformHandler::GetScale(m_id).SetScale(scale);
	}

	void Entity::SetScale(const float& x, const float& y, const float& z) {
		TransformHandler::GetScale(m_id).SetScale(x, y, z);
	}

	const Vec3f& Entity::GetPos() {
		return TransformHandler::GetTranslation(m_id).GetPos();
	}

	const Vec3f& Entity::GetRot() {
		return TransformHandler::GetRotation(m_id).GetRot();
	}

	const Vec3f& Entity::GetScale() {
		return TransformHandler::GetScale(m_id).GetScale();
	}
}