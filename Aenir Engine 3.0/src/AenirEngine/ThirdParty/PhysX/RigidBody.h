#pragma once
//#include"Include\PxPhysicsAPI.h"
#include"PhysXService.h"
#include"BuritoMath.h"
#include"AenDefines.h"

using namespace physx;

namespace Aen
{
	class AEN_DECLSPEC RigidBody {
	private:
		
		PxMaterial* mp_Material;
		PxPhysics* mp_LocalPhysics;
		PxRigidStatic* mp_StaticBody;
		PxRigidDynamic* mp_DynamicBody;

		friend class ComponentHandler;
		friend class MeshInstance;
		friend class Entity;

		const Mat4f GetTransform();
		void SetPos(const Vec3f& pos);
		void SetPos(const float& x, const float& y, const float& z);

		void SetRot(const Vec3f& rot);
		void SetRot(const float& p, const float& y, const float& r);
	public:
		RigidBody();
		~RigidBody();
		

		void CreateMaterial(float staticFriction = 0.5f, float dynamicFriction = 0.5f, float restitution = 0.6f);
		//nX, nY, nZ, distance
		void CreatePlane(Aen::Vec4f planeParameters = Aen::Vec4f(0.f, 1.f, 0.f, 0.f)); 
		void CreateCube(float density = 50.f, Aen::Vec3f cubeLengths = Aen::Vec3f(0.5f, 0.5f, 0.5f),
			Aen::Vec3f transform = Aen::Vec3f::zero,
			Aen::Vec3f velocity = Aen::Vec3f::zero);
	};
}