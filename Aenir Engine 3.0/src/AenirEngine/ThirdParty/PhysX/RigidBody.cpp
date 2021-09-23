#include "PCH.h"
#include"RigidBody.h"

namespace Aen
{
	RigidBody::RigidBody()
	{
		m_Material = NULL;
		m_LocalPhysics = PhysXService::GetInstance()->GetPxPhysics();
		m_StaticBody = NULL;
		m_DynamicBody = NULL;
		

	}
	RigidBody::~RigidBody()
	{

	}

	void RigidBody::CreateMaterial(float staticFriction, float dynamicFriction, float restitution)
	{
		/*this->m_Material = PhysXService::GetInstance()->GetPxPhysics()->
			createMaterial(staticFriction, dynamicFriction, restitution);*/
		m_Material = m_LocalPhysics->createMaterial(staticFriction, dynamicFriction, restitution);
	}

	void RigidBody::CreatePlane(float nx, float ny, float nz, float distance)
	{
		/*this->m_StaticBody = PxCreatePlane(*PhysXService::GetInstance()->GetPxPhysics(), 
			PxPlane(nx, ny, nz, distance), *this->m_Material);*/
		m_StaticBody = PxCreatePlane(*m_LocalPhysics,
			PxPlane(nx, ny, nz, distance), *m_Material);
		PhysXService::GetInstance()->AddActor(m_StaticBody);
	}

	void RigidBody::CreateCube(float density, float cubeX, float cubeY, float cubeZ,
		float tX, float tY, float tZ,
		float vX, float vY, float vZ)
	{
		const PxTransform t = PxTransform(PxVec3(tX, tY, tZ));
		const PxGeometry geometry = PxBoxGeometry(cubeX, cubeY, cubeZ);
		PxVec3 velocity = PxVec3(vX, vY, vZ);

		m_DynamicBody = PxCreateDynamic(*m_LocalPhysics, t, geometry, *m_Material, density);
		m_DynamicBody->setAngularDamping(0.5f);
		m_DynamicBody->setLinearVelocity(velocity);
		
		PhysXService::GetInstance()->AddActor(m_DynamicBody);
	}

}
/*
* CreateDynamic(PxTransform(PxVec3(0, 40, 100)), PxBoxGeometry(5.f, 3.f, 3.f), 50.f, PxVec3(0, -10, -100));
* 
PxRigidDynamic* PhysXWrap::CreateDynamic(const PxTransform& t, 
const PxGeometry& geometry, const float& density, const PxVec3& velocity)
{
	PxRigidDynamic* dynamic = PxCreateDynamic(*m_Physics, t, geometry, *m_Material, density);
	dynamic->setAngularDamping(0.5f);
	dynamic->setLinearVelocity(velocity);
	m_Scene->addActor(*dynamic);

	return dynamic;
}

*/
/*
void PhysXWrap::CreateStack(const PxTransform& t, PxU32 size, PxReal halfExtent)
{

PxShape* shape = m_Physics->createShape(PxBoxGeometry(halfExtent, halfExtent, halfExtent), *m_Material);
	//PxShape* shape = m_Physics->createShape(PxSphereGeometry(2), *m_Material);
	for (PxU32 i = 0; i < size; i++)
	{
		for (PxU32 j = 0; j < size - i; j++)
		{
			PxTransform localTm(PxVec3(PxReal(j * 2) - PxReal(size - i), PxReal(i * 2 + 1), 0) * halfExtent);
			PxRigidDynamic* body = m_Physics->createRigidDynamic(t.transform(localTm));
			body->attachShape(*shape);
			PxRigidBodyExt::updateMassAndInertia(*body, 10.0f);
			m_Scene->addActor(*body);
		}
	}
	shape->release();
}
*/