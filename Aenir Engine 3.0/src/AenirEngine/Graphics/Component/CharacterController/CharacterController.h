#pragma once
#include"../Component.h"
#include"../../Physics/PhysicsHandler.h"
#include"../../Physics/Raycast.h"

namespace Aen {

	class AEN_DECLSPEC CharacterController : public Component {
		public:
		CharacterController(const size_t& id);

		void SetHeight(const float& height);
		void SetSlopeLimit(const float& ang);
		void Move(const Vec3f& dir, const float& deltaTime);
		void SetUpDirection(const Vec3f& up);
		const Vec3f GetPos();

		const bool IsGrounded();

		private:
		~CharacterController();

		const Mat4f GetTranslate();
		void SetPos(const Vec3f& pos);
		void SetPos(const float& x, const float& y, const float& z);

		px::PxPhysics* m_physics;
		px::PxController* m_controller;
		px::PxControllerFilters m_filter;
		CollisionFilter m_callback;

		Raycast m_ray;
		bool m_isGrounded;
		float m_height;

		friend class Entity;
		friend class MeshInstance;
		friend class ComponentHandler;
	};
}