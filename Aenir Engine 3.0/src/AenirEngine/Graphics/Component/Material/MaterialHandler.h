#pragma once
#include"..\..\Graphics.h"
#include<unordered_map>

namespace Aen {

	class Material;

	class MaterialHandler {
		public:

		static Material& GetMaterial(const std::string& name) {
			if(GetMaterials().count(name) > 0)
				return *GetMaterials().at(name);

			throw;
		}

		private:
		static std::unordered_map<std::string, Material*> GetMaterials() {
			static std::unordered_map<std::string, Material*> materials;
			return materials;
		}

		static void Destroy() {
			for(auto& i : GetMaterials())
				if(i.second) {
					delete i.second;
					i.second = nullptr;
				}
		}

		friend class Material;
		friend class GameLoop;
	};
}