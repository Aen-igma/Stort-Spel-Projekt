#pragma once
#include<unordered_map>
#include"AenDefines.h"

namespace Aen {

	class Entity;

	class AEN_DECLSPEC EntityHandler {
		private:

		EntityHandler();

		static void RemoveEntity(const uint32_t& id) {
			if(m_entities.count(id) > 0)
				m_entities.erase(id);
		}

		static Entity& GetEntity(const uint32_t& id) {
			if(m_entities.count(id) > 0)
				return  *m_entities.at(id);
		}
		
		static std::unordered_map<uint32_t, Entity*> m_entities;
		
		friend class Entity;
		friend class Renderer;
		friend class MeshInstance;
	};
}