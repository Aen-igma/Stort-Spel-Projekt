#include "PCH.h"
#include "CharacterController.h"

namespace Aen {

	CollisionFilter::CollisionFilter() {}

	CollisionFilter::~CollisionFilter() {}

	bool CollisionFilter::filter(const px::PxController& a, const px::PxController& b) {
		
		/*if (a.getActor() == b.getActor())
			return false;*/
		return true;
	}

	CharacterController::CharacterController(const size_t& id)
		:Component(id), m_physics(PhysicsHandler::GetInstance()->GetPxPhysics()),
		m_controller(nullptr), m_isGrounded(false) {

		px::PxCapsuleControllerDesc desc;
		desc.climbingMode = px::PxCapsuleClimbingMode::eCONSTRAINED;
		desc.nonWalkableMode = px::PxControllerNonWalkableMode::ePREVENT_CLIMBING_AND_FORCE_SLIDING;
		desc.radius = 0.5f;
		desc.height = 0.99f;
		desc.slopeLimit = 45.f;
		desc.stepOffset = 0.25f;
		desc.contactOffset = 0.01f;
		desc.upDirection = px::PxVec3(0.f, 1.f, 0.f);
		desc.material = m_physics->createMaterial(0.f, 0.f, 0.f);
		if (!desc.isValid())
			throw;

		m_height = 0.99f;

		m_controller = PhysicsHandler::GetCManager()->createController(desc);
		px::PxVec3 d = -m_controller->getUpDirection();
		m_ray.SetDirection(d.x, d.y, d.z);
		m_ray.SetMaxDist(0.01f);

		m_filter.mCCTFilterCallback = &m_callback;
	}

	void CharacterController::SetHeight(const float& height) {
		m_height = height;
		m_controller->resize(height);
	}

	void CharacterController::SetSlopeLimit(const float& ang) {
		m_controller->setSlopeLimit(ang);
	}

	void CharacterController::Move(const Vec3f& dir, const float& deltaTime) {
		m_controller->move(px::PxVec3(dir.x, dir.y, dir.z), 0.f, deltaTime, m_filter);
	}

	CharacterController::~CharacterController() {
		m_controller->release();
	}

	const Mat4f CharacterController::GetTranslate() {
		px::PxExtendedVec3 p = m_controller->getPosition();
		return MatTranslate(Vec3f((float)p.x, (float)p.y, (float)p.z));
	}

	void CharacterController::SetPos(const Vec3f& pos) {
		m_controller->setPosition(px::PxExtendedVec3(pos.x, pos.y, pos.z));
	}

	void CharacterController::SetPos(const float& x, const float& y, const float& z) {
		m_controller->setPosition(px::PxExtendedVec3(x, y, z));
	}

	void CharacterController::SetUpDirection(const Vec3f& up) {
		m_controller->setUpDirection(px::PxVec3(up.x, up.y, up.z));
	}

	const Vec3f CharacterController::GetPos() {
		px::PxExtendedVec3 p = m_controller->getPosition();
		return Vec3f((float)p.x, (float)p.y, (float)p.z);
	}

	const bool CharacterController::IsGrounded() {
		px::PxExtendedVec3 p = m_controller->getFootPosition();
		m_ray.SetOrigin(Vec3f((float)p.x, (float)p.y, (float)p.z) + m_ray.GetDirection() * 0.08f);
		m_ray.Update();
		return m_ray.Hit();
	}

}