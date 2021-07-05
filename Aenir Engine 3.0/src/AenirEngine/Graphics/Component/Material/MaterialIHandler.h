#pragma once
#include"..\..\Graphics.h"
#include<unordered_map>

namespace Aen {

	class MaterialInstance;

	class MaterialIHandler {
		private:

		static MaterialInstance& GetMaterialInstance(const uint32_t& id) {
			if(GetMaterialInstances().count(id) > 0)
				return *GetMaterialInstances().at(id);
		}

		static std::unordered_map<uint32_t, MaterialInstance*> GetMaterialInstances() {
			static std::unordered_map<uint32_t, MaterialInstance*> materialInstance;
			return materialInstance;
		}

		friend class MaterialInstance;
	};
}