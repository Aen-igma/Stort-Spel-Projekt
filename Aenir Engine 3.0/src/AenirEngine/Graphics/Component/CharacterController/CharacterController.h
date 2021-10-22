#pragma once
#include"../../Graphics.h"
#include"../../Physics/PhysicsHandler.h"

namespace Aen {

	class AEN_DECLSPEC CharacterController {
		public:
		CharacterController();

		void SetHeight(const float& height);
		void SetSlopeLimit(const float& ang);
		void Move(const Vec3f& dir, const float& deltaTime);
		void SetUpDirection(const Vec3f& up);
		
		private:
		~CharacterController();

		void SetPos(const Vec3f& pos);
		void SetPos(const float& x, const float& y, const float& z);
		const Vec3f GetPos();

		px::PxScene* m_pScene;
		px::PxPhysics* m_physics;
		px::PxControllerManager* m_cManager;
		px::PxController* m_controller;

		friend class Entity;
		friend class MeshInstance;
		friend class ComponentHandler;
	};
}