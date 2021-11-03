#include"PCH.h"
#include"PhysicsHandler.h"

namespace Aen {
	PhysXWrap* PhysicsHandler::mp_PhysXService(nullptr);
	px::PxControllerManager* PhysicsHandler::m_cManager(nullptr);
}