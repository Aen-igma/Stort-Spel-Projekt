#include "PCH.h"
#include "Entity.h"

namespace Aen {

	uint32_t Entity::m_iDs(0);

	Entity::~Entity() {
		ComponentHandler::RemoveCamera(m_id);
		ComponentHandler::RemoveMeshInstance(m_id);
		ComponentHandler::RemoveTranform(m_id);
		ComponentHandler::RemoveSpotLight(m_id);
		ComponentHandler::RemovePointLight(m_id);
		ComponentHandler::RemoveDirectionalLight(m_id);
		ComponentHandler::RemoveRigid(m_id);
		ComponentHandler::RemoveAnimators(m_id);

		EntityHandler::RemoveEntity(m_id);
		ComponentHandler::RemoveMeshFromLayer(m_id, m_layer + 3);
	}

	Entity::Entity()
		:m_id(m_iDs), m_parentId(UINT_MAX), m_layer(0), m_hasParent(false) {
		EntityHandler::m_entities.emplace(m_id, this);
		m_iDs++;
	}

	void Entity::SetRenderLayer(const int& layer) {
		if(m_layer == layer) return;

		if(ComponentHandler::MeshInstanceExist(m_id)) {
			int l = layer;
			Clamp(l, -3, 3);
			ComponentHandler::GetLayer(m_layer + 3).at(m_id) = nullptr;
			ComponentHandler::GetLayer(m_layer + 3).erase(m_id);
			ComponentHandler::SetRenderLayer(ComponentHandler::GetMeshInstance(m_id), m_id, l + 3);
		}

		m_layer = layer;
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
		m_parentId = UINT_MAX;
	}

	void Entity::SetPos(const Vec3f& pos) {
		if(ComponentHandler::RigidExist(m_id))
			ComponentHandler::GetRigid(m_id).SetPos(pos);
		else if(ComponentHandler::TranslationExist(m_id))
			ComponentHandler::GetTranslation(m_id).SetPos(pos);
	}

	void Entity::SetPos(const float& x, const float& y, const float& z) {
		if(ComponentHandler::RigidExist(m_id))
			ComponentHandler::GetRigid(m_id).SetPos(x, y, z);
		else if(ComponentHandler::TranslationExist(m_id))
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
		if(ComponentHandler::RigidExist(m_id))
			ComponentHandler::GetRigid(m_id).SetRot(rot);
		else if(ComponentHandler::RotationExist(m_id))
			ComponentHandler::GetRotation(m_id).SetRot(rot);
	}

	void Entity::SetRot(const float& p, const float& y, const float& r) {
		if(ComponentHandler::RigidExist(m_id))
			ComponentHandler::GetRigid(m_id).SetRot(p, y, r);
		else if(ComponentHandler::RotationExist(m_id))
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

	const uint32_t& Entity::GetID()
	{
		return m_id;
	}



	const Mat4f Entity::GetTransformation() {

		Mat4f pos = (ComponentHandler::TranslationExist(m_id)) ? ComponentHandler::GetTranslation(m_id).GetTranform() : Mat4f::identity;
		Mat4f rot = (ComponentHandler::RotationExist(m_id)) ? ComponentHandler::GetRotation(m_id).GetTranform() : Mat4f::identity;
		Mat4f scale = (ComponentHandler::ScaleExist(m_id)) ? ComponentHandler::GetScale(m_id).GetTranform() : Mat4f::identity;

		Mat4f parentMatrix;
		if(m_hasParent)
			parentMatrix = EntityHandler::GetEntity(m_parentId).GetTransformation();

		return scale * rot * pos * parentMatrix;
	}

	const Mat4f Entity::GetPosMat() {
		Mat4f pos = (ComponentHandler::TranslationExist(m_id)) ? ComponentHandler::GetTranslation(m_id).GetTranform() : Mat4f::identity;

		Mat4f parentMatrix;
		if(m_hasParent)
			parentMatrix = EntityHandler::GetEntity(m_parentId).GetPosMat();

		return pos * parentMatrix;
	}

	const Mat4f Entity::GetRotMat() {
		Mat4f rot = (ComponentHandler::RotationExist(m_id)) ? ComponentHandler::GetRotation(m_id).GetTranform() : Mat4f::identity;

		Mat4f parentMatrix;
		if(m_hasParent)
			parentMatrix = EntityHandler::GetEntity(m_parentId).GetRotMat();

		return rot * parentMatrix;
	}

	const Mat4f Entity::GetScaleMat() {
		Mat4f scale = (ComponentHandler::ScaleExist(m_id)) ? ComponentHandler::GetScale(m_id).GetTranform() : Mat4f::identity;

		Mat4f parentMatrix;
		if(m_hasParent)
			parentMatrix = EntityHandler::GetEntity(m_parentId).GetScaleMat();

		return scale * parentMatrix;
	}
}