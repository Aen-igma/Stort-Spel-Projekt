#pragma once
#include"Tranform/Tranform.h"
#include"Material/Material.h"
#include"Camera/Camera.h"
#include"Mesh/Mesh.h"

namespace Aen {

	class Entity {
		public:
		~Entity();
		Entity();

		template<class T> 
		void AddComponent();

		template<class T>
		T& GetComponent();

		private:
		static uint32_t m_iDs;
		uint32_t m_id;
	};

	// --------------------- AddComponent ---------------------------

	template<>
	inline void Entity::AddComponent<Translation>() {
		if(!TranformHandler::TranslationExist(m_id))
			TranformHandler::CreateTranslation(m_id);

		throw;
	}

	template<>
	inline void Entity::AddComponent<Rotation>() {
		if(!TranformHandler::RotationExist(m_id))
			TranformHandler::CreateRotation(m_id);

		throw;
	}

	template<>
	inline void Entity::AddComponent<Scale>() {
		if(!TranformHandler::ScaleExist(m_id))
			TranformHandler::CreateScale(m_id);

		throw;
	}

	template<>
	inline void Entity::AddComponent<Camera>() {
		if(!CameraHandler::CameraExist(m_id))
			CameraHandler::CreateCamera(m_id);

		AddComponent<Translation>();
		AddComponent<Rotation>();

		throw;
	}

	template<>
	inline void Entity::AddComponent<MeshInstance>() {
		if(!MeshIHandler::MeshInstanceExist(m_id))
			MeshIHandler::CreateMeshInstance(m_id);

		AddComponent<Translation>();
		AddComponent<Rotation>();
		AddComponent<Scale>();

		throw;
	}
}