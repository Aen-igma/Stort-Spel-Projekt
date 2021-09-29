#include "PCH.h"
#include"RigidBody.h"

namespace Aen
{
	RigidBody::RigidBody()
		:mp_Material(NULL), mp_LocalPhysics(NULL), mp_StaticBody(NULL), mp_DynamicBody(NULL)
	{
		mp_LocalPhysics = PhysXService::GetInstance()->GetPxPhysics();
	}

	RigidBody::~RigidBody()
	{
		if (mp_DynamicBody != NULL)
		{
			PhysXService::GetInstance()->RemoveActor(mp_DynamicBody);
			mp_DynamicBody->release();
			mp_Material->release();
		}
	}

	const Mat4f RigidBody::GetTransform() {

		PxTransform t;
		ZeroMemory(&t, sizeof(PxTransform));
		if(mp_DynamicBody)
			t = mp_DynamicBody->getGlobalPose();

		/*if(mp_StaticBody)
			t = mp_StaticBody->getGlobalPose();*/

		return MatQuaternion(t.q.x, t.q.y, t.q.z, t.q.w) * MatTranslate(t.p.x, t.p.y, t.p.z);
	}

	void RigidBody::SetPos(const Vec3f& pos) {

		PxTransform t(pos.x, pos.y, pos. z);

		if(mp_DynamicBody)
			mp_DynamicBody->setGlobalPose(t);

		if(mp_StaticBody)
			mp_StaticBody->setGlobalPose(t);
	}

	void RigidBody::SetPos(const float& x, const float& y, const float& z) {

		PxTransform t(PxVec3(x, y, z));

		if(mp_DynamicBody)
			mp_DynamicBody->setGlobalPose(t);

		if(mp_StaticBody)
			mp_StaticBody->setGlobalPose(t);
	}

	void RigidBody::SetRot(const Vec3f& rot) {}

	void RigidBody::SetRot(const float& p, const float& y, const float& r) {}

	void RigidBody::CreateMaterial(float staticFriction, float dynamicFriction, float restitution)
	{
		mp_Material = mp_LocalPhysics->createMaterial(staticFriction, dynamicFriction, restitution);
	}

	void RigidBody::CreatePlane(Aen::Vec4f planeParameters)
	{
		mp_StaticBody = PxCreatePlane(*mp_LocalPhysics,
			PxPlane(planeParameters.x, planeParameters.y, planeParameters.z, planeParameters.w), *mp_Material);
		PhysXService::GetInstance()->AddActor(mp_StaticBody);
	}

	void RigidBody::CreateCube(float density, Aen::Vec3f cubeLengths, Aen::Vec3f transform, Aen::Vec3f velocity)
	{
		mp_DynamicBody = PxCreateDynamic(*mp_LocalPhysics, 
			PxTransform(PxVec3(transform.x, transform.y, transform.z)),
			PxBoxGeometry(PxReal(cubeLengths.x), PxReal(cubeLengths.y), PxReal(cubeLengths.z)), 
			*mp_Material, density);
		mp_DynamicBody->setAngularDamping(0.5f);
		mp_DynamicBody->setLinearVelocity(PxVec3(velocity.x, velocity.y, velocity.z));
		PhysXService::GetInstance()->AddActor(mp_DynamicBody);

	}

}