#include "PCH.h"
#include"DynamicBody.h"
#include"../ComponentHandler.h"

namespace Aen {
	
	DynamicBody::~DynamicBody() {
		RemoveRigid();
		mp_Material->release();
	}

	DynamicBody::DynamicBody(const size_t& id)
		:Component(id), m_mass(25.f), m_density(50.f), m_sleep(1.f), m_aDamp(0.5f), mp_Material(nullptr), 
		mp_LocalPhysics(PhysicsHandler::GetInstance()->GetPxPhysics()), mp_DynamicBody(nullptr), m_scale(Vec3f::one) {
		mp_Material = mp_LocalPhysics->createMaterial(1.f, 1.f, 0.2f);
		mp_Material->setFrictionCombineMode(px::PxCombineMode::eAVERAGE);
		mp_Material->setRestitutionCombineMode(px::PxCombineMode::eAVERAGE);

		px::PxTransform t(0.f, 0.f, 0.f);
		px::PxBoxGeometry cube(m_scale.x * 0.5f, m_scale.y * 0.5f, m_scale.x * 0.5f);
		mp_DynamicBody = px::PxCreateDynamic(*mp_LocalPhysics, t, cube, *mp_Material, m_density);
		PhysicsHandler::GetInstance()->AddActor(mp_DynamicBody);
	}

	void DynamicBody::SetStaticFriction(const float& value) {
		mp_Material->setStaticFriction(value);
	}

	void DynamicBody::SetDynamicFriction(const float& value) {
		mp_Material->setDynamicFriction(value);
	}

	void DynamicBody::SetRestitution(const float& value) {
		mp_Material->setRestitution(value);
	}

	void DynamicBody::SetFrictionCombineMode(const CombineMode& mode) {
		mp_Material->setFrictionCombineMode((px::PxCombineMode::Enum)mode);
	}

	void DynamicBody::SetRestitutionCombineMode(const CombineMode& mode) {
		mp_Material->setRestitutionCombineMode((px::PxCombineMode::Enum)mode);
	}

	void DynamicBody::SetMaterialFlag(const MaterialFlag& flag) {
		mp_Material->setFlags((px::PxMaterialFlag::Enum)flag);
	}

	void DynamicBody::SetGeometry(const DynamicGeometryType& geometry, const Vec3f& scale) {

		if(geometry == m_gType)
			return;

		m_scale = scale;
		m_gType = geometry;
		px::PxTransform t = mp_DynamicBody->getGlobalPose();
		RemoveRigid();

		switch(geometry) {
			case DynamicGeometryType::SPHERE: {
			float r = Max(m_scale.x, Max(m_scale.y, m_scale.x)) * 0.5;
			px::PxSphereGeometry sphere(r);
			mp_DynamicBody = PxCreateDynamic(*mp_LocalPhysics, t, sphere, *mp_Material, m_density);
			} break;
			case DynamicGeometryType::CAPSULE: {
				float rc = Max(m_scale.x, m_scale.z);
				px::PxCapsuleGeometry capsule(rc * 0.5f, m_scale.y);
				mp_DynamicBody = PxCreateDynamic(*mp_LocalPhysics, t, capsule, *mp_Material, m_density);
			} break;
			case DynamicGeometryType::CUBE: {
				px::PxBoxGeometry cube(m_scale.x * 0.5f, m_scale.y * 0.5f, m_scale.z * 0.5f);
				mp_DynamicBody = PxCreateDynamic(*mp_LocalPhysics, t, cube, *mp_Material, m_density);
			} break;
		}

		mp_DynamicBody->setMass(m_mass);
		mp_DynamicBody->setAngularDamping(m_aDamp);
		mp_DynamicBody->setSleepThreshold(m_sleep);
		PhysicsHandler::GetInstance()->AddActor(mp_DynamicBody);
	}

	void DynamicBody::SetBoundsToMesh() {

		Vec3f bounds;
		if(ComponentHandler::MeshInstanceExist(m_id))
			if(ComponentHandler::GetMeshInstance(m_id).m_pMesh)
				bounds.smVec = ComponentHandler::GetMeshInstance(m_id).m_pMesh->m_obb.Extents;

		m_scale = bounds;
		m_gType = DynamicGeometryType::CUBE;
		px::PxTransform t = mp_DynamicBody->getGlobalPose();
		RemoveRigid();

		px::PxBoxGeometry cube(m_scale.x * 0.5f, m_scale.y * 0.5f, m_scale.z * 0.5f);
		mp_DynamicBody = PxCreateDynamic(*mp_LocalPhysics, t, cube, *mp_Material, m_density);

		PhysicsHandler::GetInstance()->AddActor(mp_DynamicBody);
	}

	void DynamicBody::SetAngularDamping(const float& damp) {
		mp_DynamicBody->setAngularDamping(damp);
	}

	void DynamicBody::SetSleepThresHold(const float& value) {
		mp_DynamicBody->setSleepThreshold(value);
	}

	void DynamicBody::SetMass(const float& mass) {
		mp_DynamicBody->setMass(mass);
		m_mass = mass;
	}

	void DynamicBody::AddForce(const Vec3f& force, const ForceMode& mode) {
		mp_DynamicBody->addForce(px::PxVec3(force.x, force.y, force.z), (px::PxForceMode::Enum)mode, true);
	}

	void DynamicBody::AddForce(const float& x, const float& y, const float& z, const ForceMode& mode) {
		mp_DynamicBody->addForce(px::PxVec3(x, y, z), (px::PxForceMode::Enum)mode, true);
	}

	void DynamicBody::AddTorque(const Vec3f& force, const ForceMode& mode) {
		mp_DynamicBody->addTorque(px::PxVec3(force.x, force.y, force.z), (px::PxForceMode::Enum)mode, true);
	}

	void DynamicBody::AddTorque(const float& x, const float& y, const float& z, const ForceMode& mode) {
		mp_DynamicBody->addTorque(px::PxVec3(x, y, z), (px::PxForceMode::Enum)mode, true);
	}


	void DynamicBody::RemoveRigid() {
		PhysicsHandler::GetInstance()->RemoveActor(mp_DynamicBody);
		mp_DynamicBody->release();
		mp_DynamicBody = nullptr;
	}

	const Mat4f DynamicBody::GetTransform() {
		px::PxTransform t = mp_DynamicBody->getGlobalPose();
		return MatQuaternion(t.q.x, t.q.y, t.q.z, t.q.w) * MatTranslate(t.p.x, t.p.y, t.p.z);
	}

	const Mat4f DynamicBody::GetTranslate() {
		px::PxTransform t = mp_DynamicBody->getGlobalPose();
		return MatTranslate(t.p.x, t.p.y, t.p.z);
	}

	const Mat4f DynamicBody::GetRotMat() {
		px::PxTransform t = mp_DynamicBody->getGlobalPose();
		return MatQuaternion(t.q.x, t.q.y, t.q.z, t.q.w);
	}

	void DynamicBody::SetPos(const Vec3f& pos) {
		px::PxTransform t = mp_DynamicBody->getGlobalPose();
		t.p = px::PxVec3(pos.x, pos.y, pos.z);
		mp_DynamicBody->setGlobalPose(t);
	}

	void DynamicBody::SetPos(const float& x, const float& y, const float& z) {
		px::PxTransform t = mp_DynamicBody->getGlobalPose();
		t.p = px::PxVec3(x, y, z);
		mp_DynamicBody->setGlobalPose(t);
	}

	void DynamicBody::SetRot(const Vec3f& rot) {
		Vec4f tempRot = EulerToQuat(rot);
		px::PxTransform t = mp_DynamicBody->getGlobalPose();
		t.q = px::PxQuat(tempRot.x, tempRot.y, tempRot.z, tempRot.w);
		mp_DynamicBody->setGlobalPose(t);
	}

	void DynamicBody::SetRot(const float& p, const float& y, const float& r) {
		Vec4f tempRot = EulerToQuat(p, y, r);
		px::PxTransform t = mp_DynamicBody->getGlobalPose();
		t.q = px::PxQuat(tempRot.x, tempRot.y, tempRot.z, tempRot.w);
		mp_DynamicBody->setGlobalPose(t);
	}

	const Vec3f DynamicBody::GetPos() {
		px::PxTransform t = mp_DynamicBody->getGlobalPose();
		return Vec3f(t.p.x, t.p.y, t.p.z);
	}

	const Vec3f DynamicBody::GetRot() {
		return Vec3f();
	}
}