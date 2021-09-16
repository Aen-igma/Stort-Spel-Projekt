#pragma once

#include"Include\PxPhysicsAPI.h"

using namespace physx;

class PhysXWrap
{
private:

	PxDefaultAllocator		m_DefaultAllocatorCallback;
	PxDefaultErrorCallback  m_DefaultErrorCallback;
	PxDefaultCpuDispatcher* m_Dispatcher;
	PxTolerancesScale		m_ToleranceScale;
	PxFoundation*		    m_Foundation;
	PxPhysics*				m_Physics;
	PxScene*			    m_Scene;
	PxMaterial*			    m_Material;
	PxPvd*					m_Pvd;
	PxReal stackZ;

public:
	PhysXWrap();
	~PhysXWrap();

	void createStack(const PxTransform& t, PxU32 size, PxReal halfExtent);
	PxRigidDynamic* createDynamic(const PxTransform& t, const PxGeometry& geometry, const PxVec3& velocity = PxVec3(0));

	void initPhysics();
	void closePhysics();
	void runPhysics();
};