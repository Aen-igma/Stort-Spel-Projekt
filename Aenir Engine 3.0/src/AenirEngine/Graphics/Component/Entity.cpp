#include "PCH.h"
#include "Entity.h"
#include"EntityHandler.h"

namespace Aen {

	Entity::~Entity() {
		ComponentHandler::RemoveCamera(m_id);
		ComponentHandler::RemoveMeshInstance(m_id);
		ComponentHandler::RemoveTranform(m_id);
		ComponentHandler::RemoveSpotLight(m_id);
		ComponentHandler::RemovePointLight(m_id);
		ComponentHandler::RemoveDirectionalLight(m_id);
		ComponentHandler::RemoveStaticBody(m_id);
		ComponentHandler::RemoveDynamicBody(m_id);
		ComponentHandler::RemoveCharacterController(m_id);
		ComponentHandler::RemoveUI(m_id);
		ComponentHandler::RemoveAABB(m_id);
		ComponentHandler::RemoveAnimators(m_id);


		ComponentHandler::RemoveMeshFromLayer(m_id, m_layer + 3);
		ComponentHandler::RemoveMeshFromLayer(m_id, 6);
		ComponentHandler::RemoveMeshFromLayer(m_id, 5);
		EntityHandler::RemoveFromTaged(m_id, m_tag);
	}

	Entity::Entity(const size_t& id)
		:m_id(id), m_parentId(UINT_MAX), m_layer(0), m_hasParent(false), m_tag("NONE") {}

	void Entity::SetTag(const std::string& tag) {

		if(m_tag == tag)
			return;

		EntityHandler::RemoveFromTaged(m_id, m_tag);

		m_tag = tag;

		if(m_tag != "NONE")
			EntityHandler::m_tagedEntities.emplace(tag, this);
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
		if(ComponentHandler::CharacterControllerExist(m_id))
			ComponentHandler::GetCharacterController(m_id).SetPos(pos);
		else if(ComponentHandler::DynamicBodyExist(m_id))
			ComponentHandler::GetDynamicBody(m_id).SetPos(pos);
		else if(ComponentHandler::StaticBodyExist(m_id))
			ComponentHandler::GetStaticBody(m_id).SetPos(pos);
		else if(ComponentHandler::TranslationExist(m_id))
			ComponentHandler::GetTranslation(m_id).SetPos(pos);
	}

	void Entity::SetPos(const float& x, const float& y, const float& z) {
		if(ComponentHandler::CharacterControllerExist(m_id))
			ComponentHandler::GetCharacterController(m_id).SetPos(x, y, z);
		else if(ComponentHandler::DynamicBodyExist(m_id))
			ComponentHandler::GetDynamicBody(m_id).SetPos(x, y, z);
		else if(ComponentHandler::StaticBodyExist(m_id))
			ComponentHandler::GetStaticBody(m_id).SetPos(x, y, z);
		else if(ComponentHandler::TranslationExist(m_id))
			ComponentHandler::GetTranslation(m_id).SetPos(x, y, z);
	}

	void Entity::Move(const Vec3f& pos) {
		if(ComponentHandler::StaticBodyExist(m_id))
			ComponentHandler::GetStaticBody(m_id).Move(pos);
		else if (ComponentHandler::TranslationExist(m_id))
			ComponentHandler::GetTranslation(m_id).Move(pos);
	}

	void Entity::Move(const float& x, const float& y, const float& z) {
		if (ComponentHandler::StaticBodyExist(m_id))
			ComponentHandler::GetStaticBody(m_id).Move(x,y,z);
		else if (ComponentHandler::TranslationExist(m_id))
			ComponentHandler::GetTranslation(m_id).Move(x,y,z);
	}

	void Entity::MoveRelative(const Vec3f& pos) {
		if (ComponentHandler::StaticBodyExist(m_id))
			ComponentHandler::GetStaticBody(m_id).MoveRelative(pos);
		else if (ComponentHandler::TranslationExist(m_id))
			ComponentHandler::GetTranslation(m_id).Move(Transform(ComponentHandler::GetRotation(m_id).GetTranform(), pos));
	}

	void Entity::MoveRelative(const float& x, const float& y, const float& z) {
		if (ComponentHandler::StaticBodyExist(m_id))
			ComponentHandler::GetStaticBody(m_id).MoveRelative(x,y,z);
		else if (ComponentHandler::TranslationExist(m_id))
			ComponentHandler::GetTranslation(m_id).Move(Transform(ComponentHandler::GetRotation(m_id).GetTranform(), Vec3f(x, y, z)));
	}

	void Entity::SetRot(const Vec3f& rot) {
		if(ComponentHandler::DynamicBodyExist(m_id))
			ComponentHandler::GetDynamicBody(m_id).SetRot(rot);
		if(ComponentHandler::StaticBodyExist(m_id))
			ComponentHandler::GetStaticBody(m_id).SetRot(rot);
		else if(ComponentHandler::RotationExist(m_id))
			ComponentHandler::GetRotation(m_id).SetRot(rot);

		if(ComponentHandler::CameraExist(m_id))
			ComponentHandler::GetCamera(m_id).LookTowards(Transform(MatRotate(rot), Vec3f(0.f, 0.f, -1.f)).Normalized());
	}

	void Entity::SetRot(const float& p, const float& y, const float& r) {
		if(ComponentHandler::DynamicBodyExist(m_id))
			ComponentHandler::GetDynamicBody(m_id).SetRot(p, y, r);
		if(ComponentHandler::StaticBodyExist(m_id))
			ComponentHandler::GetStaticBody(m_id).SetRot(p, y, r);
		else if(ComponentHandler::RotationExist(m_id))
			ComponentHandler::GetRotation(m_id).SetRot(p, y, r);

		if(ComponentHandler::CameraExist(m_id))
			ComponentHandler::GetCamera(m_id).LookTowards(Transform(MatRotate(p, y, r), Vec3f(0.f, 0.f, -1.f)).Normalized());
	}

	void Entity::Rotate(const Vec3f& rot) {
		if(ComponentHandler::RotationExist(m_id))
			ComponentHandler::GetRotation(m_id).Rotate(rot);

		if (ComponentHandler::StaticBodyExist(m_id))
			ComponentHandler::GetStaticBody(m_id).Rotate(rot);

		if(ComponentHandler::CameraExist(m_id))
			ComponentHandler::GetCamera(m_id).LookTowards(Transform(MatRotate(rot), Vec3f(0.f, 0.f, -1.f)).Normalized());
	}

	void Entity::Rotate(const float& p, const float& y, const float& r) {
		if(ComponentHandler::RotationExist(m_id))
			ComponentHandler::GetRotation(m_id).Rotate(p, y, r);

		if (ComponentHandler::StaticBodyExist(m_id))
			ComponentHandler::GetStaticBody(m_id).Rotate(p,y,r);

		if(ComponentHandler::CameraExist(m_id))
			ComponentHandler::GetCamera(m_id).LookTowards(Transform(MatRotate(p, y, r), Vec3f(0.f, 0.f, -1.f)).Normalized());
	}

	void Entity::SetScale(const Vec3f& scale) {
		if(ComponentHandler::ScaleExist(m_id))
			ComponentHandler::GetScale(m_id).SetScale(scale);
	}

	void Entity::SetScale(const float& x, const float& y, const float& z) {
		if(ComponentHandler::ScaleExist(m_id))
			ComponentHandler::GetScale(m_id).SetScale(x, y, z);
	}

	void Entity::SetTransformation(const Mat4f & m) {
		tempBonParent = m;
	}

	const Vec3f Entity::GetPos() const {

		Vec3f pos;
		if(ComponentHandler::DynamicBodyExist(m_id))
			pos = ComponentHandler::GetDynamicBody(m_id).GetPos();
		else if(ComponentHandler::StaticBodyExist(m_id))
			pos = ComponentHandler::GetStaticBody(m_id).GetPos();
		else if(ComponentHandler::CharacterControllerExist(m_id))
			pos = ComponentHandler::GetCharacterController(m_id).GetPos();
		else
			pos = ComponentHandler::GetTranslation(m_id).GetPos();

		return pos;
	}

	const Vec3f Entity::GetRot() const {
		Vec3f rot;
		if(ComponentHandler::DynamicBodyExist(m_id))
			rot = ComponentHandler::GetDynamicBody(m_id).GetRot();
		else if(ComponentHandler::StaticBodyExist(m_id))
			rot = ComponentHandler::GetStaticBody(m_id).GetRot();
		else
			rot = ComponentHandler::GetRotation(m_id).GetRot();

		return rot;
	}

	const Vec3f Entity::GetScale() const {
		if(ComponentHandler::ScaleExist(m_id))
			return ComponentHandler::GetScale(m_id).GetScale();
		return Vec3f::one;
	}

	const size_t& Entity::GetID() const {
		return m_id;
	}

	const std::string& Entity::GetTag() const {
		return m_tag;
	}

	const bool Entity::HasId(const size_t& id) const {
		return m_id == id;
	}

	const Mat4f Entity::GetTransformation() const {

		Mat4f pos(Mat4f::identity); 
		if(ComponentHandler::DynamicBodyExist(m_id))
			pos = ComponentHandler::GetDynamicBody(m_id).GetTranslate(); 
		if(ComponentHandler::StaticBodyExist(m_id))
			pos = ComponentHandler::GetStaticBody(m_id).GetTranslate(); 
		else if(ComponentHandler::CharacterControllerExist(m_id))
			pos = ComponentHandler::GetCharacterController(m_id).GetTranslate();
		else if(ComponentHandler::TranslationExist(m_id))
			pos = ComponentHandler::GetTranslation(m_id).GetTranform(); 

		Mat4f rot(Mat4f::identity);
		if(ComponentHandler::DynamicBodyExist(m_id))
			rot = ComponentHandler::GetDynamicBody(m_id).GetRotMat();
		else if(ComponentHandler::StaticBodyExist(m_id))
			rot = ComponentHandler::GetStaticBody(m_id).GetRotMat();
		else if(ComponentHandler::RotationExist(m_id))
			rot = ComponentHandler::GetRotation(m_id).GetTranform();

		Mat4f scale = (ComponentHandler::ScaleExist(m_id)) ? ComponentHandler::GetScale(m_id).GetTranform() : Mat4f::identity;

		Mat4f parentMatrix;
		if(m_hasParent)
			parentMatrix = EntityHandler::GetEntity(m_parentId).GetTransformation();

		return scale * rot * pos * tempBonParent * parentMatrix;
	}

	const Vec3f Entity::GetTranslation() const {

		Vec3f pos = GetPos();

		Vec3f parent;
		if (m_hasParent) {
			parent = EntityHandler::GetEntity(m_parentId).GetTranslation();
		}

		return pos + parent;
	}

	const Mat4f Entity::GetPosMat() const {
		Mat4f pos = (ComponentHandler::TranslationExist(m_id)) ? ComponentHandler::GetTranslation(m_id).GetTranform() : Mat4f::identity;

		Mat4f parentMatrix;
		if(m_hasParent)
			parentMatrix = EntityHandler::GetEntity(m_parentId).GetPosMat();

		return pos * parentMatrix;
	}

	const Mat4f Entity::GetRotMat() const {
		Mat4f rot = (ComponentHandler::RotationExist(m_id)) ? ComponentHandler::GetRotation(m_id).GetTranform() : Mat4f::identity;

		Mat4f parentMatrix;
		if(m_hasParent)
			parentMatrix = EntityHandler::GetEntity(m_parentId).GetRotMat();

		return rot * parentMatrix;
	}

	const Mat4f Entity::GetScaleMat() const {
		Mat4f scale = (ComponentHandler::ScaleExist(m_id)) ? ComponentHandler::GetScale(m_id).GetTranform() : Mat4f::identity;

		Mat4f parentMatrix;
		if(m_hasParent)
			parentMatrix = EntityHandler::GetEntity(m_parentId).GetScaleMat();

		return scale * parentMatrix;
	}
}