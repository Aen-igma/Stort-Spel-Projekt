#include"PCH.h"
#include"ComponentHandler.h"

namespace Aen {

	std::unordered_map<size_t, Camera*> ComponentHandler::m_cameras;
	std::unordered_map<size_t, MeshInstance*> ComponentHandler::m_mesheInstances;
	std::unordered_map<size_t, Translation*> ComponentHandler::m_translations;
	std::unordered_map<size_t, Rotation*>	ComponentHandler::m_rotations;
	std::unordered_map<size_t, Scale*> ComponentHandler::m_scales;
	std::unordered_map<size_t, RigidBody*> ComponentHandler::m_rigids;
	std::unordered_map<size_t, CharacterController*> ComponentHandler::m_characterControllers;
	std::unordered_map<size_t, AABoundBox*> ComponentHandler::m_AABBs;
	std::unordered_map<size_t, UIComponent*> ComponentHandler::m_UI;
	std::multimap<size_t, Light*> ComponentHandler::m_lights;

	std::array<std::unordered_map<size_t, Drawable*>, 7> ComponentHandler::m_meshLayer;

}