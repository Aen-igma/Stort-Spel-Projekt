#pragma once

#include"Include\PxPhysicsAPI.h"

using namespace physx;

class PhysXWrap
{
private:

	PxDefaultAllocator		m_DefaultAllocatorCallback;
	PxDefaultErrorCallback  m_DefaultErrorCallback;
	PxTolerancesScale		m_ToleranceScale;
	PxDefaultCpuDispatcher* m_Dispatcher;
	PxFoundation*		    m_Foundation;
	PxMaterial*			    m_Material;
	PxPhysics*				m_Physics;
	PxCooking*				m_Cooking;
	PxScene*			    m_Scene;
	PxReal					stackZ;
	PxPvd*					m_Pvd;

public:
	PhysXWrap();
	~PhysXWrap();

	void CreateStack(const PxTransform& t, PxU32 size, PxReal halfExtent);
	PxRigidDynamic* CreateDynamic(const PxTransform& t, const PxGeometry& geometry, const float& density, const PxVec3& velocity = PxVec3(0));
	void CreateDemo();

	void AddActor(PxRigidStatic *staticActor);
	void AddActor(PxRigidDynamic* dynamicActor); 
	PxPhysics* GetPxPhysics() const;


	void InitPhysics(int toleranceLength, int toleranceSpeed);
	void ClosePhysics();
	void RunPhysics(const float& deltaTime);
};