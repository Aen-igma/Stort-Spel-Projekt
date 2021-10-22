#include "PCH.h"
#include "CharacterController.h"

namespace Aen {

	CharacterController::CharacterController() :m_pScene(PhysicsHandler::GetInstance()->GetScene()), m_physics(PhysicsHandler::GetInstance()->GetPxPhysics()),
		m_cManager(nullptr), m_controller(nullptr) {
		m_cManager = PxCreateControllerManager(*m_pScene, false);

		px::PxCapsuleControllerDesc desc;
		desc.climbingMode = px::PxCapsuleClimbingMode::eCONSTRAINED;
		desc.nonWalkableMode = px::PxControllerNonWalkableMode::ePREVENT_CLIMBING_AND_FORCE_SLIDING;
		desc.radius = 0.5f;
		desc.height = 1.3f;
		desc.slopeLimit = 45.f;
		desc.stepOffset = 0.25f;
		desc.contactOffset = 0.1f;
		desc.upDirection = px::PxVec3(0.f, 1.f, 0.f);
		desc.material = m_physics->createMaterial(0.f, 0.f, 0.f);
		if(!desc.isValid())
			throw;

		m_controller = m_cManager->createController(desc);
	}

	void CharacterController::SetHeight(const float& height) {
		m_controller->resize(height);
	}

	void CharacterController::SetSlopeLimit(const float& ang) {
		m_controller->setSlopeLimit(ang);
	}

	void CharacterController::Move(const Vec3f& dir, const float& deltaTime) {
		m_controller->move(px::PxVec3(dir.x, dir.y, dir.z), 0.f, deltaTime, px::PxControllerFilters());
	}

	CharacterController::~CharacterController() {
		m_controller->release();
		m_cManager->release();
		m_pScene = nullptr;
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
		return Vec3f(p.x, p.y, p.z);
	}


}