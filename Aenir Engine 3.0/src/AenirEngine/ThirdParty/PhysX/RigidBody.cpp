#include "PCH.h"
#include"RigidBody.h"

namespace Aen
{
	RigidBody::RigidBody()
	{
		this->m_Material = NULL;
		this->stackZ = 10.0f;
	}
	RigidBody::~RigidBody()
	{
	}
}