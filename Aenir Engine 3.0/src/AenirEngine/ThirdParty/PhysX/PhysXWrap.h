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
	//PxMaterial*			    m_Material;
	PxPhysics*				m_Physics;
	PxCooking*				m_Cooking;
	PxScene*			    m_Scene;
	PxReal					stackZ;
	PxPvd*					m_Pvd;

public:
	PhysXWrap();
	~PhysXWrap();

	//void CreateStack(const PxTransform& t, PxU32 size, PxReal halfExtent);
	//PxRigidDynamic* CreateDynamic(const PxTransform& t, const PxGeometry& geometry, const float& density, const PxVec3& velocity = PxVec3(0));

	/*void AddActor(PxRigidStatic *staticActor);
	void AddActor(PxRigidDynamic *dynamicActor);*/
	void InitPhysics(int toleranceLength, int toleranceSpeed);
	void ClosePhysics();
	void RunPhysics(const float& deltaTime);
};

/*
#include <iostream>

using namespace std;

class Singleton {
   static Singleton *instance;
   int data;

   // Private constructor so that no objects can be created.
   Singleton() {
	  data = 0;
   }

   public:
   static Singleton *getInstance() {
	  if (!instance)
	  instance = new Singleton;
	  return instance;
   }

   int getData() {
	  return this -> data;
   }

   void setData(int data) {
	  this -> data = data;
   }
};

//Initialize pointer to zero so that it can be initialized in first call to getInstance
Singleton *Singleton::instance = 0;

int main(){
   Singleton *s = s->getInstance();
   cout << s->getData() << endl;
   s->setData(100);
   cout << s->getData() << endl;
   return 0;
}
*/