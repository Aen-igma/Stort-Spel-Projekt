#include"PCH.h"
#include"ComponentHandler.h"

namespace Aen {

	std::unordered_map<uint32_t, Camera*> ComponentHandler::m_cameras;
	std::unordered_map<uint32_t, MaterialInstance*> ComponentHandler::m_materialInstance;
	std::unordered_map<uint32_t, MeshInstance*> ComponentHandler::m_mesheInstances;
	std::unordered_map<uint32_t, Translation*> ComponentHandler::m_translations;
	std::unordered_map<uint32_t, Rotation*>	ComponentHandler::m_rotations;
	std::unordered_map<uint32_t, Scale*> ComponentHandler::m_scales;

	std::unordered_map<uint32_t, PointLight*> ComponentHandler::m_pointLights;

}