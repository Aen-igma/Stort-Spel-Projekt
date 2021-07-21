#pragma once
#include"Material.h"
#include<unordered_map>

namespace Aen {

	class AEN_DECLSPEC MaterialHandler {
		public:

		static const bool MaterialExist(const std::string& name) {
			return m_materials.count(name) > 0;
		}

		static Material* CreateMaterial(const std::string& name) {
			m_materials.emplace(name, AEN_NEW Material());
			return m_materials.at(name);
		}

		static void RemoveMaterial(const std::string& name) {
			if(m_materials.count(name) > 0) {
				delete m_materials.at(name);
				m_materials.at(name) = nullptr;
				m_materials.erase(name);
			}
		}

		static Material& GetMaterial(const std::string& name) {
			if(m_materials.count(name) > 0)
				return *m_materials.at(name);

			throw;
		}

		private:

		MaterialHandler();

		static void Destroy() {
			for(auto& i : m_materials)
				if(i.second) {
					delete i.second;
					i.second = nullptr;
				}
		}
		
		static std::unordered_map<std::string, Material*> m_materials;

		friend class Material;
		friend class GameLoop;
	};
}