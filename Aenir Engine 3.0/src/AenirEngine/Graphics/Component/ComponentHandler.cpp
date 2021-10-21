#include"PCH.h"
#include"ComponentHandler.h"

namespace Aen {

	std::unordered_map<uint32_t, Camera*> ComponentHandler::m_cameras;
	std::unordered_map<uint32_t, MeshInstance*> ComponentHandler::m_mesheInstances;
	std::unordered_map<uint32_t, Translation*> ComponentHandler::m_translations;
	std::unordered_map<uint32_t, Rotation*>	ComponentHandler::m_rotations;
	std::unordered_map<uint32_t, Scale*> ComponentHandler::m_scales;
	std::unordered_map<uint32_t, RigidBody*> ComponentHandler::m_rigids;
	std::unordered_map<uint32_t, Drawable*> ComponentHandler::m_drawables;
	std::multimap<uint32_t, Light*> ComponentHandler::m_lights;

	std::array<std::unordered_map<uint32_t, Drawable*>, 7> ComponentHandler::m_meshLayer;

}