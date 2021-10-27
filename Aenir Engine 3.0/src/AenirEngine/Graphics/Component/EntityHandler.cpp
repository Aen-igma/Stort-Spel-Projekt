#include"PCH.h"
#include"EntityHandler.h"

namespace Aen {

	std::unordered_map<size_t, Entity*> EntityHandler::m_entities;
	std::multimap<size_t, Entity*> EntityHandler::m_tagedEntities;
	size_t EntityHandler::m_iDs;

}