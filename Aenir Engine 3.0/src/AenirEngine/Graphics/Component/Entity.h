#pragma once
#include"Material/MaterialIHandler.h"
#include"Material/MaterialHandler.h"
#include"Material/TextureHandler.h"
#include"Tranform/TranformHandler.h"
#include"Camera/CameraHandler.h"
#include"Mesh/MeshIHandler.h"
#include"Mesh/MeshHandler.h"

#include"EntityHandler.h"

namespace Aen {

	class Entity {
		public:
		~Entity();
		Entity();

		template<class T> 
		void AddComponent();

		template<class T>
		T& GetComponent();

		void SetParent(Entity& parent);
		void RemoveParent();

		void SetPos(const Vec3f& pos);
		void SetPos(const float& x, const float& y, const float& z);
		void Move(const Vec3f& pos);
		void Move(const float& x, const float& y, const float& z);

		void SetRot(const Vec3f& rot);
		void SetRot(const float& p, const float& y, const float& r);
		void Rotate(const Vec3f& rot);
		void Rotate(const float& p, const float& y, const float& r);

		void SetScale(const Vec3f& scale);
		void SetScale(const float& x, const float& y, const float& z);

		const Vec3f& GetPos();
		const Vec3f& GetRot();
		const Vec3f& GetScale();

		private:
		static uint32_t m_iDs;

		uint32_t m_id;
		uint32_t m_parentId;
		bool m_hasParent;
	};

	// --------------------- AddComponent ---------------------------

	template<>
	inline void Entity::AddComponent<Translation>() {
		if(!TransformHandler::TranslationExist(m_id))
			TransformHandler::CreateTranslation(m_id);
	}

	template<>
	inline void Entity::AddComponent<Rotation>() {
		if(!TransformHandler::RotationExist(m_id))
			TransformHandler::CreateRotation(m_id);
	}

	template<>
	inline void Entity::AddComponent<Scale>() {
		if(!TransformHandler::ScaleExist(m_id))
			TransformHandler::CreateScale(m_id);
	}

	template<>
	inline void Entity::AddComponent<Camera>() {
		if(!CameraHandler::CameraExist(m_id))
			CameraHandler::CreateCamera(m_id);

		AddComponent<Translation>();
		AddComponent<Rotation>();
	}

	template<>
	inline void Entity::AddComponent<MeshInstance>() {
		if(!MeshIHandler::MeshInstanceExist(m_id))
			MeshIHandler::CreateMeshInstance(m_id);

		AddComponent<Translation>();
		AddComponent<Rotation>();
		AddComponent<Scale>();
	}

	template<>
	inline void Entity::AddComponent<MaterialInstance>() {
		if(!MaterialIHandler::MaterialInstanceExist(m_id))
			MaterialIHandler::CreateMaterialInstance(m_id);
	}


	// --------------- GetComponent -----------------

	template<>
	inline Translation& Entity::GetComponent() {
		return TransformHandler::GetTranslation(m_id);
	}

	template<>
	inline Rotation& Entity::GetComponent() {
		return TransformHandler::GetRotation(m_id);
	}

	template<>
	inline Scale& Entity::GetComponent() {
		return TransformHandler::GetScale(m_id);
	}

	template<>
	inline Camera& Entity::GetComponent() {
		return CameraHandler::GetCamera(m_id);
	}

	template<>
	inline MeshInstance& Entity::GetComponent() {
		return MeshIHandler::GetMeshInstance(m_id);
	}

	template<>
	inline MaterialInstance& Entity::GetComponent() {
		return MaterialIHandler::GetMaterialInstance(m_id);
	}
}