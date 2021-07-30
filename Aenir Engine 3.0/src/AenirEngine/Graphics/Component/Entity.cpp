#include "PCH.h"
#include "Entity.h"

namespace Aen {

	uint32_t Entity::m_iDs(0);

	Entity::~Entity() {
		ComponentHandler::RemoveCamera(m_id);
		ComponentHandler::RemoveMeshInstance(m_id);
		ComponentHandler::RemoveMaterial(m_id);
		ComponentHandler::RemoveTranform(m_id);
		ComponentHandler::RemoveSpotLight(m_id);
		ComponentHandler::RemovePointLight(m_id);
		ComponentHandler::RemoveDirectionalLight(m_id);

		EntityHandler::RemoveEntity(m_id);
	}

	Entity::Entity()
		:m_id(m_iDs), m_parentId(-1), m_hasParent(false) {
		EntityHandler::m_entities.emplace(m_id, this);
		m_iDs++;
	}

	void Entity::SetParent(Entity& parent) {
		if(parent.m_parentId != m_id) {
			m_hasParent = true;
			m_parentId = parent.m_id;
		} else
			throw;
	}

	void Entity::RemoveParent() {
		m_hasParent = false;
		m_parentId = -1;
	}

	void Entity::SetPos(const Vec3f& pos) {
		ComponentHandler::GetTranslation(m_id).SetPos(pos);
	}

	void Entity::SetPos(const float& x, const float& y, const float& z) {
		ComponentHandler::GetTranslation(m_id).SetPos(x, y, z);
	}

	void Entity::Move(const Vec3f& pos) {
		ComponentHandler::GetTranslation(m_id).Move(pos);
	}

	void Entity::Move(const float& x, const float& y, const float& z) {
		ComponentHandler::GetTranslation(m_id).Move(x, y, z);
	}

	void Entity::MoveRelative(const Vec3f& pos) {
		ComponentHandler::GetTranslation(m_id).Move(Transform(ComponentHandler::GetRotation(m_id).GetTranform(), pos));
	}

	void Entity::MoveRelative(const float& x, const float& y, const float& z) {
		ComponentHandler::GetTranslation(m_id).Move(Transform(ComponentHandler::GetRotation(m_id).GetTranform(), Vec3f(x, y, z)));
	}

	void Entity::SetRot(const Vec3f& rot) {
		ComponentHandler::GetRotation(m_id).SetRot(rot);
	}

	void Entity::SetRot(const float& p, const float& y, const float& r) {
		ComponentHandler::GetRotation(m_id).SetRot(p, y, r);
	}

	void Entity::Rotate(const Vec3f& rot) {
		ComponentHandler::GetRotation(m_id).Rotate(rot);
	}

	void Entity::Rotate(const float& p, const float& y, const float& r) {
		ComponentHandler::GetRotation(m_id).Rotate(p, y, r);
	}

	void Entity::SetScale(const Vec3f& scale) {
		ComponentHandler::GetScale(m_id).SetScale(scale);
	}

	void Entity::SetScale(const float& x, const float& y, const float& z) {
		ComponentHandler::GetScale(m_id).SetScale(x, y, z);
	}

	const Vec3f& Entity::GetPos() {
		return ComponentHandler::GetTranslation(m_id).GetPos();
	}

	const Vec3f& Entity::GetRot() {
		return ComponentHandler::GetRotation(m_id).GetRot();
	}

	const Vec3f& Entity::GetScale() {
		return ComponentHandler::GetScale(m_id).GetScale();
	}
}