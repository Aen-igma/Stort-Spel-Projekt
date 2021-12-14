#include"PCH.h"
#include"ComponentHandler.h"

namespace Aen {

	std::unordered_map<size_t, Camera*> ComponentHandler::m_cameras;
	std::unordered_map<size_t, MeshInstance*> ComponentHandler::m_mesheInstances;
	std::unordered_map<size_t, Translation*> ComponentHandler::m_translations;
	std::unordered_map<size_t, Rotation*>	ComponentHandler::m_rotations;
	std::unordered_map<size_t, Scale*> ComponentHandler::m_scales;
	std::unordered_map<size_t, StaticBody*> ComponentHandler::m_staticBodies;
	std::unordered_map<size_t, DynamicBody*> ComponentHandler::m_dynamicBodies;
	std::unordered_map<size_t, CharacterController*> ComponentHandler::m_characterControllers;
	std::unordered_map<size_t, AABoundBox*> ComponentHandler::m_AABBs;
	std::unordered_map<size_t, OBBox*> ComponentHandler::m_OBBs;
	std::unordered_map<size_t, UIComponent*> ComponentHandler::m_UI;
	std::unordered_map<size_t, UI*> ComponentHandler::m_UI2;
	std::unordered_map<size_t, PSSystemcomponent*> ComponentHandler::m_PS;
	std::multimap<size_t, Light*> ComponentHandler::m_lights;
	std::unordered_map<size_t, Animator*> ComponentHandler::m_animators;

	std::array<std::unordered_map<size_t, Drawable*>, 7> ComponentHandler::m_meshLayer;
	
}