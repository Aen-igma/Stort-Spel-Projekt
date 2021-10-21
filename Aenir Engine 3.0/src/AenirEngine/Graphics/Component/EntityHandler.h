#pragma once
#include<unordered_map>
#include"AenDefines.h"
#include"Entity.h"

namespace Aen {

	class AEN_DECLSPEC EntityHandler {
		public:

		static Entity& CreateEntity() {
			m_entities.emplace(m_iDs, AEN_NEW Entity(m_iDs));
			size_t id = m_iDs;
			m_iDs++;
			return *m_entities.at(id);
		}
		
		static void RemoveEntity(Entity& e) {
			size_t id = e.GetID();
			if(m_entities.count(id) > 0) {
				delete m_entities.at(id);
				m_entities.at(id) = nullptr;
				m_entities.erase(id);
			}
		}

		static auto GetTagedEntities(const std::string& tag) {
			size_t cTag = std::stoi(tag);
			return std::pair<std::multimap<size_t, Entity*>::iterator, std::multimap<size_t, Entity*>::iterator>(
				m_tagedEntities.lower_bound(cTag), m_tagedEntities.upper_bound(cTag));
		}

		private:
		EntityHandler();
		
		static Entity& GetEntity(const size_t& id) {
			if(m_entities.count(id) > 0)
				return *m_entities.at(id);
		}
		
		static void Destroy() {

			for(auto i : m_entities) {
				delete i.second;
				i.second = nullptr;
			}
		}

		static std::unordered_map<size_t, Entity*> m_entities;
		static std::multimap<size_t, Entity*> m_tagedEntities;
		static size_t m_iDs;
		
		friend class Entity;
		friend class Renderer;
		friend class ImGuiHandler;
		friend class MeshInstance;
		friend class GameLoop;
	};
}