#pragma once
#include"ComponentHandler.h"
#include"Resource.h"

namespace Aen {

	class AEN_DECLSPEC Entity {
		public:
		template<class T> 
		void AddComponent();

		template<class T>
		T& GetComponent() const;

		void SetTag(const std::string& tag);
		void SetRenderLayer(const int& layer);
		void SetParent(Entity& parent);
		void RemoveParent();

		void SetPos(const Vec3f& pos);
		void SetPos(const float& x, const float& y, const float& z);
		void Move(const Vec3f& pos);
		void Move(const float& x, const float& y, const float& z);
		void MoveRelative(const Vec3f& pos);
		void MoveRelative(const float& x, const float& y, const float& z);

		void SetRot(const Vec3f& rot);
		void SetRot(const float& p, const float& y, const float& r);
		void Rotate(const Vec3f& rot);
		void Rotate(const float& p, const float& y, const float& r);

		void SetScale(const Vec3f& scale);
		void SetScale(const float& x, const float& y, const float& z);

		void SetTransformation(const Mat4f& m);

		const Vec3f GetPos() const;
		const Vec3f GetRot() const;
		const Vec3f GetScale() const;
		const size_t& GetID() const;
		const std::string& GetTag() const;

		const Vec3f GetTranslation() const;
		const Mat4f GetTransformation() const;

		private:
		Entity(const size_t& id);
		~Entity();

		const bool HasId(const size_t& id) const;
		const Mat4f GetPosMat() const;
		const Mat4f GetRotMat() const;
		const Mat4f GetScaleMat() const;

		const size_t m_id;
		size_t m_parentId;
		size_t m_layer;
		bool m_hasParent;
		std::string m_tag;

		Mat4f tempBonParent;

		friend class Renderer;
		friend class MeshInstance;
		friend class AABoundBox;
		friend class EntityHandler;
	};

	// --------------------- AddComponent ---------------------------

	template<>
	inline void Entity::AddComponent<Translation>() {
		if(!ComponentHandler::TranslationExist(m_id))
			ComponentHandler::CreateTranslation(m_id);
	}

	template<>
	inline void Entity::AddComponent<Rotation>() {
		if(!ComponentHandler::RotationExist(m_id))
			ComponentHandler::CreateRotation(m_id);
	}

	template<>
	inline void Entity::AddComponent<Scale>() {
		if(!ComponentHandler::ScaleExist(m_id))
			ComponentHandler::CreateScale(m_id);
	}

	template<>
	inline void Entity::AddComponent<Camera>() {
		if(!ComponentHandler::CameraExist(m_id))
			ComponentHandler::CreateCamera(m_id);

		AddComponent<Translation>();
		AddComponent<Rotation>();
	}

	template<>
	inline void Entity::AddComponent<MeshInstance>() {
		if(!ComponentHandler::MeshInstanceExist(m_id))
			ComponentHandler::CreateMeshInstance(m_id, m_layer + 3);

		AddComponent<Translation>();
		AddComponent<Rotation>();
		AddComponent<Scale>();
	}

	template<>
	inline void Entity::AddComponent<UIComponent>() {
		m_layer = 3;
		if (!ComponentHandler::UIComponentExist(m_id))
			ComponentHandler::CreateUI(m_id, m_layer + 3);
	}

	template<>
	inline void Entity::AddComponent<PSSystemcomponent>() {
		if (!ComponentHandler::PSExist(m_id))
			ComponentHandler::CreatePS(m_id, m_layer + 3);
	}

	template<>
	inline void Entity::AddComponent<Mesh>() {
		if(!ComponentHandler::MeshInstanceExist(m_id))
			ComponentHandler::CreateMeshInstance(m_id, m_layer + 3);

		AddComponent<Translation>();
		AddComponent<Rotation>();
		AddComponent<Scale>();
	}

	template<>
	inline void Entity::AddComponent<SpotLight>() {
		if(!ComponentHandler::SpotLightExist(m_id))
			ComponentHandler::CreateSpotLight(m_id);

		AddComponent<Translation>();
		AddComponent<Rotation>();
	}

	template<>
	inline void Entity::AddComponent<PointLight>() {
		if(!ComponentHandler::PointLightExist(m_id))
			ComponentHandler::CreatePointLight(m_id);

		AddComponent<Translation>();
	}

	template<>
	inline void Entity::AddComponent<DirectionalLight>() {
		if(!ComponentHandler::DirectionalLightExist(m_id))
			ComponentHandler::CreateDirectionalLight(m_id);

		AddComponent<Rotation>();
	}

	template<>
	inline void Entity::AddComponent<StaticBody>() {
		if (!ComponentHandler::StaticBodyExist(m_id))
			ComponentHandler::CreateStaticBody(m_id);
	}

	template<>
	inline void Entity::AddComponent<DynamicBody>() {
		if (!ComponentHandler::DynamicBodyExist(m_id))
			ComponentHandler::CreateDynamicBody(m_id);
	}

	template<>
	inline void Entity::AddComponent<CharacterController>() {
		if (!ComponentHandler::CharacterControllerExist(m_id))
			ComponentHandler::CreateCharacterController(m_id);

		AddComponent<Translation>();
		AddComponent<Rotation>();
		AddComponent<Scale>();
	}


	template<>
	inline void Entity::AddComponent<AABoundBox>(){
		if (!ComponentHandler::AABBExist(m_id))
			ComponentHandler::CreateAABB(m_id, m_layer + 3);
		
		AddComponent<Translation>();
	}

	template<>
	inline void Entity::AddComponent<OBBox>() {
		if (!ComponentHandler::OBBExist(m_id))
			ComponentHandler::CreateOBB(m_id, m_layer + 3);

		AddComponent<Translation>();
		AddComponent<Rotation>();
	}

	template<>
	inline void Entity::AddComponent<Animator>() {
		if (!ComponentHandler::AnimatorExists(m_id))
			ComponentHandler::CreateAnimator(m_id);

		AddComponent<Translation>();
		AddComponent<Rotation>();
		AddComponent<Scale>();
	}
	// --------------- GetComponent -----------------

	template<>
	inline Translation& Entity::GetComponent() const {
		return ComponentHandler::GetTranslation(m_id);
	}

	template<>
	inline Rotation& Entity::GetComponent() const {
		return ComponentHandler::GetRotation(m_id);
	}

	template<>
	inline Scale& Entity::GetComponent() const {
		return ComponentHandler::GetScale(m_id);
	}

	template<>
	inline Camera& Entity::GetComponent() const {
		return ComponentHandler::GetCamera(m_id);
	}

	template<>
	inline MeshInstance& Entity::GetComponent() const {
		return ComponentHandler::GetMeshInstance(m_id);
	}

	template<>
	inline SpotLight& Entity::GetComponent() const {
		return ComponentHandler::GetSpotLight(m_id);
	}

	template<>
	inline PointLight& Entity::GetComponent() const {
		return ComponentHandler::GetPointLight(m_id);
	}

	template<>
	inline DirectionalLight& Entity::GetComponent() const {
		return ComponentHandler::GetDirectionalLight(m_id);
	}

	template<>
	inline StaticBody& Entity::GetComponent() const {
		return ComponentHandler::GetStaticBody(m_id);
	}

	template<>
	inline DynamicBody& Entity::GetComponent() const {
		return ComponentHandler::GetDynamicBody(m_id);
	}

	template<>
	inline CharacterController& Entity::GetComponent() const {
		return ComponentHandler::GetCharacterController(m_id);
	}

	template<>
	inline UIComponent& Entity::GetComponent() const {
		return ComponentHandler::GetUI(m_id);
	}

	template<>
	inline PSSystemcomponent& Entity::GetComponent() const{
		return ComponentHandler::GetPS(m_id);
	}

	template<>
	inline AABoundBox& Entity::GetComponent() const{
		return ComponentHandler::GetAABB(m_id);
	}

	template<>
	inline OBBox& Entity::GetComponent() const {
		return ComponentHandler::GetOBB(m_id);
	}

	template<>
	inline Animator& Entity::GetComponent() const {
		return ComponentHandler::GetAnimator(m_id);
	}
}