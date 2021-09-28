#pragma once

#include"Include\PxPhysicsAPI.h"

using namespace physx;

class PhysXWrap
{
private:

	PxDefaultAllocator		m_DefaultAllocatorCallback;
	PxDefaultErrorCallback  m_DefaultErrorCallback;
	PxTolerancesScale		m_ToleranceScale;
	PxDefaultCpuDispatcher* mp_Dispatcher;
	PxFoundation*		    mp_Foundation;
	PxMaterial*			    mp_Material;
	PxPhysics*				mp_Physics;
	PxCooking*				mp_Cooking;
	PxScene*			    mp_Scene;
	PxReal					m_StackZ;
	PxPvd*					mp_Pvd;

public:
	PhysXWrap();
	~PhysXWrap();

	void CreateStack(const PxTransform& t, PxU32 size, PxReal halfExtent);
	PxRigidDynamic* CreateDynamic(const PxTransform& t, const PxGeometry& geometry, const float& density, const PxVec3& velocity = PxVec3(0));
	void CreateDemo();

	void AddActor(PxRigidStatic *staticActor);
	void AddActor(PxRigidDynamic* dynamicActor); 
	PxPhysics*& GetPxPhysics();


	void InitPhysics(int toleranceLength, int toleranceSpeed);
	void ClosePhysics();
	void RunPhysics(const float& deltaTime);
};