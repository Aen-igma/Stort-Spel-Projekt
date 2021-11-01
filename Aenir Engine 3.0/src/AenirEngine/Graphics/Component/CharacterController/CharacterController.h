#pragma once
#include"../Component.h"
#include"../../Physics/PhysicsHandler.h"

namespace Aen {

	class AEN_DECLSPEC CharacterController : public Component {
		public:
		CharacterController(const size_t& id);

		void SetHeight(const float& height);
		void SetSlopeLimit(const float& ang);
		void Move(const Vec3f& dir, const float& deltaTime);
		void SetUpDirection(const Vec3f& up);
		const Vec3f GetPos();
		
		private:
		~CharacterController();

		const Mat4f GetTranslate();
		void SetPos(const Vec3f& pos);
		void SetPos(const float& x, const float& y, const float& z);

		px::PxScene* m_pScene;
		px::PxPhysics* m_physics;
		px::PxControllerManager* m_cManager;
		px::PxController* m_controller;

		friend class Entity;
		friend class MeshInstance;
		friend class ComponentHandler;
	};
}