#pragma once
#include"../Graphics.h"
#include"ThirdParty/PhysX/PhysX.h"

namespace Aen {
	
	

	class AEN_DECLSPEC PhysXWrap {
		public:
		PhysXWrap();
		~PhysXWrap();

		void InitPhysics(const int& toleranceLength, const int& toleranceSpeed);
		void RunPhysics(const float& deltaTime);

		void CreateStack(const px::PxTransform& t, px::PxU32 size, px::PxReal halfExtent);
		px::PxRigidDynamic* CreateDynamic(const px::PxTransform& t, const px::PxGeometry& geometry, const float& density, const px::PxVec3& velocity = px::PxVec3(0));

		void AddActor(px::PxActor* dynamicActor);
		void RemoveActor(px::PxActor* dynamicActor);
		px::PxPhysics*& GetPxPhysics();
		px::PxScene*& GetScene();

		void SetGravity(const Vec3f& force);
		void SetGravity(const float& x, const float& y, const float& z);

		private:

		px::PxDefaultAllocator		m_DefaultAllocatorCallback;
		px::PxDefaultErrorCallback  m_DefaultErrorCallback;
		px::PxTolerancesScale		m_ToleranceScale;
		px::PxDefaultCpuDispatcher* mp_Dispatcher;
		px::PxFoundation*		    mp_Foundation;
		px::PxMaterial*			    mp_Material;
		px::PxPhysics*				mp_Physics;
		px::PxCooking*				mp_Cooking;
		px::PxScene*			    mp_Scene;
		px::PxReal					m_StackZ;
		px::PxPvd*					mp_Pvd;
	};
}