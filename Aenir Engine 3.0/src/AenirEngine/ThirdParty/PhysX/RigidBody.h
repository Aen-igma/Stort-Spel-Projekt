#pragma once
#include"Include\PxPhysicsAPI.h"

using namespace physx;

namespace Aen 
{
	class RigidBody
	{
	private:
		PxMaterial* m_Material;
		PxReal		stackZ;


		friend class ComponentHandler;
	public:
		RigidBody();
		~RigidBody();

		void CreateShape();

	};
}