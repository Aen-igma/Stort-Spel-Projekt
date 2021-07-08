#pragma once
#include"Material.h"
#include<unordered_map>

namespace Aen {

	class MaterialIHandler {
		private:

		static const bool MaterialInstanceExist(const uint32_t& id) {
			return GetMaterialInstances().count(id) > 0;
		}

		static void CreateMaterialInstance(const uint32_t& id) {
			GetMaterialInstances().emplace(id, AEN_NEW MaterialInstance());
		}

		static void RemoveMaterial(const uint32_t& id) {
			if(GetMaterialInstances().count(id) > 0) {
				delete GetMaterialInstances().at(id);
				GetMaterialInstances().at(id) = nullptr;
				GetMaterialInstances().erase(id);
			}
		}

		static MaterialInstance& GetMaterialInstance(const uint32_t& id) {
			if(GetMaterialInstances().count(id) > 0)
				return *GetMaterialInstances().at(id);

			throw;
		}

		static std::unordered_map<uint32_t, MaterialInstance*> GetMaterialInstances() {
			static std::unordered_map<uint32_t, MaterialInstance*> materialInstance;
			return materialInstance;
		}

		friend class MaterialInstance;
		friend class Entity;
	};
}