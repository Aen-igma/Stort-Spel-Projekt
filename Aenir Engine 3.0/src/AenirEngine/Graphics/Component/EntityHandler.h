#pragma once
#include<unordered_map>

namespace Aen {

	class Entity;

	class EntityHandler {
		private:

		static void RemoveEntity(const uint32_t& id) {
			if(GetEntities().count(id) > 0)
				GetEntities().erase(id);
		}

		static Entity& GetEntity(const uint32_t& id) {
			if(GetEntities().count(id) > 0)
				return  *GetEntities().at(id);
		}

		static std::unordered_map<uint32_t, Entity*> GetEntities() {
			static std::unordered_map<uint32_t, Entity*> entities;
			return entities;
		}

		friend class Entity;
	};
}