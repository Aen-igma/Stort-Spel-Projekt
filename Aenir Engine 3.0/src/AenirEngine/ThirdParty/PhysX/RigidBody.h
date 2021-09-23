#pragma once
//#include"Include\PxPhysicsAPI.h"
#include"PhysXService.h"

using namespace physx;

namespace Aen
{
	class RigidBody {
	private:

		PxMaterial* m_Material;
		PxPhysics* m_LocalPhysics;
		PxRigidStatic* m_StaticBody;
		PxRigidDynamic* m_DynamicBody;

		friend class ComponentHandler;
	public:
		RigidBody();
		~RigidBody();

		void CreateMaterial(float staticFriction = 0.5f, float dynamicFriction = 0.5f, float restitution = 0.6f);
		void CreatePlane(float nx = 0.f, float ny = 1.f, float nz = 0.f, float distance = 0.f);
		void CreateCube(float density = 50.f, float cubeX = 2.f, float cubeY = 2.f, float cubeZ = 2.f,
			float tX = 0.f, float tY = 1.f, float tZ = 100.f,
			float vX = 0.f, float vY = 0.f, float vZ = 0.f);
	};
}