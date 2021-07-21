#include"PCH.h"
#include"TranformHandler.h"

namespace Aen {

	std::unordered_map<uint32_t, Translation*> TransformHandler::m_translations;
	std::unordered_map<uint32_t, Rotation*>	TransformHandler::m_rotations;
	std::unordered_map<uint32_t, Scale*> TransformHandler::m_scales;

}