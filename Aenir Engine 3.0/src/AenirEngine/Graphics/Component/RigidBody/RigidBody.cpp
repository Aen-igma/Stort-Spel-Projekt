#include "PCH.h"
#include"RigidBody.h"

namespace Aen {
	RigidBody::RigidBody()
		:m_mass(25.f), m_density(50.f), m_sleep(1.f), m_aDamp(0.5f), m_gType(GeometryType::PLANE), m_rigidType(RigidType::STATIC), mp_Material(nullptr), 
		mp_LocalPhysics(PhysicsHandler::GetInstance()->GetPxPhysics()), mp_StaticBody(nullptr), mp_DynamicBody(nullptr) {
		mp_Material = mp_LocalPhysics->createMaterial(1.f, 1.f, 0.2f);
		mp_Material->setFrictionCombineMode(px::PxCombineMode::eAVERAGE);
		mp_Material->setRestitutionCombineMode(px::PxCombineMode::eAVERAGE);

		px::PxPlane plane(px::PxVec3(0.f, 0.f, 0.f), px::PxVec3(0.f, 1.f, 0.f));
		mp_StaticBody = PxCreatePlane(*mp_LocalPhysics, plane, *mp_Material);
		PhysicsHandler::GetInstance()->AddActor(mp_StaticBody);
	}

	void RigidBody::SetStaticFriction(const float& value) {
		mp_Material->setStaticFriction(value);
	}

	void RigidBody::SetDynamicFriction(const float& value) {
		mp_Material->setDynamicFriction(value);
	}

	void RigidBody::SetRestitution(const float& value) {
		mp_Material->setRestitution(value);
	}

	void RigidBody::SetFrictionCombineMode(const CombineMode& mode) {
		mp_Material->setFrictionCombineMode((px::PxCombineMode::Enum)mode);
	}

	void RigidBody::SetRestitutionCombineMode(const CombineMode& mode) {
		mp_Material->setRestitutionCombineMode((px::PxCombineMode::Enum)mode);
	}

	void RigidBody::SetMaterialFlag(const MaterialFlag& flag) {
		mp_Material->setFlags((px::PxMaterialFlag::Enum)flag);
	}

	void RigidBody::SetRigidType(const RigidType& type) {

		px::PxTransform t(0.f, 0.f, 0.f);
		if(mp_StaticBody)
			t = mp_StaticBody->getGlobalPose();
		else if(mp_DynamicBody)
			t = mp_DynamicBody->getGlobalPose();

		RemoveRigid();

		m_rigidType = type;

		px::PxSphereGeometry sphere(0.5f);
		px::PxBoxGeometry cube(0.5f, 0.5f, 0.5f);
		px::PxCapsuleGeometry capsule(0.5f, 1.f);
		if(m_rigidType == RigidType::DYNAMIC) {

			switch(m_gType) {
				case GeometryType::SPHERE:
				mp_DynamicBody = px::PxCreateDynamic(*mp_LocalPhysics, t, sphere, *mp_Material, m_density);
				break;
				case GeometryType::CAPSULE:
				mp_DynamicBody = px::PxCreateDynamic(*mp_LocalPhysics, t, capsule, *mp_Material, m_density);
				break;
				case GeometryType::CUBE:
				mp_DynamicBody = px::PxCreateDynamic(*mp_LocalPhysics, t, cube, *mp_Material, m_density);
				break;
				default:
				mp_DynamicBody = mp_LocalPhysics->createRigidDynamic(t);
				break;
			}

			mp_DynamicBody->setMass(m_mass);
			mp_DynamicBody->setAngularDamping(m_aDamp);
			mp_DynamicBody->setSleepThreshold(m_sleep);
			PhysicsHandler::GetInstance()->AddActor(mp_DynamicBody);
		} else {

			px::PxPlane plane(px::PxVec3(0.f, 0.f, 0.f), px::PxVec3(0.f, 1.f, 0.f));
			switch(m_gType) {
				case GeometryType::SPHERE:
				mp_StaticBody = px::PxCreateStatic(*mp_LocalPhysics, t, sphere, *mp_Material);
				break;
				case GeometryType::CAPSULE:
				mp_StaticBody = px::PxCreateStatic(*mp_LocalPhysics, t, capsule, *mp_Material);
				break;
				case GeometryType::PLANE:
				mp_StaticBody = PxCreatePlane(*mp_LocalPhysics, plane, *mp_Material);
				break;
				case GeometryType::CUBE:
				mp_StaticBody = px::PxCreateStatic(*mp_LocalPhysics, t, cube, *mp_Material);
				break;
			}

			PhysicsHandler::GetInstance()->AddActor(mp_StaticBody);
		}
	}

	void RigidBody::SetGeometry(const GeometryType& geometry) {

		if(geometry == m_gType)
			return;
		
		m_gType = geometry;
		px::PxTransform t(0.f, 0.f, 0.f);
		px::PxSphereGeometry sphere(0.5f);
		px::PxBoxGeometry cube(0.5f, 0.5f, 0.5f);
		px::PxCapsuleGeometry capsule(0.5f, 1.f);

		if(m_rigidType == RigidType::DYNAMIC) {

			if(geometry == GeometryType::PLANE) return;
			
			if(mp_DynamicBody) {
				t = mp_DynamicBody->getGlobalPose();
				RemoveRigid();
			}

			switch(geometry) {
				case GeometryType::SPHERE:
				mp_DynamicBody = PxCreateDynamic(*mp_LocalPhysics, t, sphere, *mp_Material, m_density);
				break;
				case GeometryType::CAPSULE:
				mp_DynamicBody = PxCreateDynamic(*mp_LocalPhysics, t, capsule, *mp_Material, m_density);
				break;
				case GeometryType::CUBE:
				mp_DynamicBody = PxCreateDynamic(*mp_LocalPhysics, t, cube, *mp_Material, m_density);
				break;
			}

			mp_DynamicBody->setMass(m_mass);
			mp_DynamicBody->setAngularDamping(m_aDamp);
			mp_DynamicBody->setSleepThreshold(m_sleep);
			PhysicsHandler::GetInstance()->AddActor(mp_DynamicBody);
		} else {

			if(mp_StaticBody) {
				t = mp_StaticBody->getGlobalPose();
				RemoveRigid();
			}

			px::PxPlane plane(px::PxVec3(0.f, 0.f, 0.f), px::PxVec3(0.f, 1.f, 0.f));
			switch(geometry) {
				case GeometryType::SPHERE:
				mp_StaticBody = PxCreateStatic(*mp_LocalPhysics, t, sphere, *mp_Material);
				break;
				case GeometryType::CAPSULE:
				mp_StaticBody = PxCreateStatic(*mp_LocalPhysics, t, capsule, *mp_Material);
				break;
				case GeometryType::PLANE:
				mp_StaticBody = PxCreatePlane(*mp_LocalPhysics, plane, *mp_Material);
				break;
				case GeometryType::CUBE:
				mp_StaticBody = PxCreateStatic(*mp_LocalPhysics, t, cube, *mp_Material);
				break;
			}

			PhysicsHandler::GetInstance()->AddActor(mp_StaticBody);
		}
	}

	void RigidBody::SetAngularDamping(const float& damp) {
		if(mp_DynamicBody)
			mp_DynamicBody->setAngularDamping(damp);
	}

	void RigidBody::SetSleepThresHold(const float& value) {
		if(mp_DynamicBody)
			mp_DynamicBody->setSleepThreshold(value);
	}

	void RigidBody::SetMass(const float& mass) {
		if(mp_DynamicBody) {
			mp_DynamicBody->setMass(mass);
			m_mass = mass;
		}
	}

	void RigidBody::AddForce(const Vec3f& force, const ForceMode& mode) {
		if(mp_DynamicBody)
			mp_DynamicBody->addForce(px::PxVec3(force.x, force.y, force.z), (px::PxForceMode::Enum)mode, true);
	}

	void RigidBody::AddForce(const float& x, const float& y, const float& z, const ForceMode& mode) {
		if(mp_DynamicBody)
			mp_DynamicBody->addForce(px::PxVec3(x, y, z), (px::PxForceMode::Enum)mode, true);
	}

	void RigidBody::AddTorque(const Vec3f& force, const ForceMode& mode) {
		px::PxForceMode;
		if(mp_DynamicBody)
			mp_DynamicBody->addTorque(px::PxVec3(force.x, force.y, force.z), (px::PxForceMode::Enum)mode, true);
	}

	void RigidBody::AddTorque(const float& x, const float& y, const float& z, const ForceMode& mode) {
		if(mp_DynamicBody)
			mp_DynamicBody->addTorque(px::PxVec3(x, y, z), (px::PxForceMode::Enum)mode, true);
	}

	RigidBody::~RigidBody() {
		RemoveRigid();
		mp_Material->release();
	}

	void RigidBody::RemoveRigid() {
		if(m_rigidType == RigidType::DYNAMIC) {
			if (mp_DynamicBody) {
				PhysicsHandler::GetInstance()->RemoveActor(mp_DynamicBody);
				mp_DynamicBody->release();
				mp_DynamicBody = nullptr;
			}
		} else {
			if (mp_StaticBody) {
				PhysicsHandler::GetInstance()->RemoveActor(mp_StaticBody);
				mp_StaticBody->release();
				mp_StaticBody = nullptr;
			}
		}
	}

	const Mat4f RigidBody::GetTransform() {

		px::PxTransform t;
		ZeroMemory(&t, sizeof(px::PxTransform));
		if(m_rigidType == RigidType::DYNAMIC) {
			t = mp_DynamicBody->getGlobalPose();
			return MatQuaternion(t.q.x, t.q.y, t.q.z, t.q.w) * MatTranslate(t.p.x, t.p.y, t.p.z);
		} else {
			t = mp_StaticBody->getGlobalPose();
			return MatTranslate(t.p.x, t.p.y, t.p.z);
		}

	}

	void RigidBody::SetPos(const Vec3f& pos) {

		px::PxTransform t(pos.x, pos.y, pos. z);

		if(m_rigidType == RigidType::DYNAMIC)
			mp_DynamicBody->setGlobalPose(t);
		else
			mp_StaticBody->setGlobalPose(t);
	}

	void RigidBody::SetPos(const float& x, const float& y, const float& z) {

		px::PxTransform t(px::PxVec3(x, y, z));

		if(m_rigidType == RigidType::DYNAMIC)
			mp_DynamicBody->setGlobalPose(t);
		else
			mp_StaticBody->setGlobalPose(t);
	}

	void RigidBody::SetRot(const Vec3f& rot) {}

	void RigidBody::SetRot(const float& p, const float& y, const float& r) {}
}