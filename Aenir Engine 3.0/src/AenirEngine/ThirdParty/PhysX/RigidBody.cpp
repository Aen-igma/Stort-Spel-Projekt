#include "PCH.h"
#include"RigidBody.h"

namespace Aen
{
	RigidBody::RigidBody()
		:mp_Material(NULL), mp_LocalPhysics(PhysXService::GetInstance()->GetPxPhysics()), mp_StaticBody(NULL), mp_DynamicBody(NULL)
	{
		/*mp_Material = NULL;
		mp_LocalPhysics = PhysXService::GetInstance()->GetPxPhysics();
		m_LocalPhysics = PxCreateBasePhysics();
		mp_StaticBody = NULL;
		mp_DynamicBody = NULL;*/

		//PxCreateBasePhysics();
	}

	RigidBody::~RigidBody()
	{

	}

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
		const PxTransform t = PxTransform(PxVec3(transform.x, transform.y, transform.z));
		const PxGeometry geometry = PxBoxGeometry(PxReal(cubeLengths.x), PxReal(cubeLengths.y), PxReal(cubeLengths.z));
		PxVec3 vel = PxVec3(velocity.x, velocity.y, velocity.z);
		mp_DynamicBody = PxCreateDynamic(*mp_LocalPhysics, t, geometry, *mp_Material, density);
		mp_DynamicBody->setAngularDamping(0.5f);
		mp_DynamicBody->setLinearVelocity(vel);
		
		PhysXService::GetInstance()->AddActor(mp_DynamicBody);
	}

}