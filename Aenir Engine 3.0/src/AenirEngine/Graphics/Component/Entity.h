#pragma once
#include"ComponentHandler.h"
#include"Resource.h"

namespace Aen {

	class AEN_DECLSPEC Entity {
		public:
		template<class T> 
		void AddComponent();

		template<class T>
		T& GetComponent();

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

		const Vec3f GetPos();
		const Vec3f GetRot();
		const Vec3f GetScale();
		const size_t& GetID();
		const std::string& GetTag();

		const Vec3f GetTranslation();
		const Mat4f GetTransformation();

		private:
		Entity(const size_t& id);
		~Entity();

		const bool HasId(const size_t& id);
		const Mat4f GetPosMat();
		const Mat4f GetRotMat();
		const Mat4f GetScaleMat();

		const size_t m_id;
		size_t m_parentId;
		size_t m_layer;
		bool m_hasParent;
		std::string m_tag;

		friend class Renderer;
		friend class MeshInstance;
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
		m_layer = 3;
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
	inline void Entity::AddComponent<RigidBody>() {
		if (!ComponentHandler::RigidExist(m_id))
			ComponentHandler::CreateRigid(m_id);

		AddComponent<Translation>();
		AddComponent<Rotation>();
		AddComponent<Scale>();
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
		//AddComponent<MeshInstance>();
		if (!ComponentHandler::AABBExist(m_id))
			ComponentHandler::CreateAABB(m_id, m_layer + 3);
		
		if (!ComponentHandler::TranslationExist(m_id))
			AddComponent<Translation>();
	}

	template<>
	inline void Entity::AddComponent<OBBox>() {
		AddComponent<MeshInstance>();
		if (!ComponentHandler::OBBExist(m_id))
			ComponentHandler::CreateOBB(m_id, m_layer + 3);

		if (!ComponentHandler::TranslationExist(m_id))
			AddComponent<Translation>();
	}
	// --------------- GetComponent -----------------

	template<>
	inline Translation& Entity::GetComponent() {
		return ComponentHandler::GetTranslation(m_id);
	}

	template<>
	inline Rotation& Entity::GetComponent() {
		return ComponentHandler::GetRotation(m_id);
	}

	template<>
	inline Scale& Entity::GetComponent() {
		return ComponentHandler::GetScale(m_id);
	}

	template<>
	inline Camera& Entity::GetComponent() {
		return ComponentHandler::GetCamera(m_id);
	}

	template<>
	inline MeshInstance& Entity::GetComponent() {
		return ComponentHandler::GetMeshInstance(m_id);
	}

	template<>
	inline SpotLight& Entity::GetComponent() {
		return ComponentHandler::GetSpotLight(m_id);
	}

	template<>
	inline PointLight& Entity::GetComponent() {
		return ComponentHandler::GetPointLight(m_id);
	}

	template<>
	inline DirectionalLight& Entity::GetComponent() {
		return ComponentHandler::GetDirectionalLight(m_id);
	}

	template<>
	inline RigidBody& Entity::GetComponent() {
		return ComponentHandler::GetRigid(m_id);
	}

	template<>
	inline CharacterController& Entity::GetComponent() {
		return ComponentHandler::GetCharacterController(m_id);
	}

	template<>
	inline UIComponent& Entity::GetComponent() {
		return ComponentHandler::GetUI(m_id);
	}

	template<>
	inline PSSystemcomponent& Entity::GetComponent() {
		return ComponentHandler::GetPS(m_id);
	}

	template<>
	inline AABoundBox& Entity::GetComponent() {
		return ComponentHandler::GetAABB(m_id);
	}

	template<>
	inline OBBox& Entity::GetComponent() {
		return ComponentHandler::GetOBB(m_id);
	}
}