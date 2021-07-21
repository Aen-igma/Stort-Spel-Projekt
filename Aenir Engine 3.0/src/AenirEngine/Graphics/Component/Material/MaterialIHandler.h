#pragma once
#include"Material.h"
#include<unordered_map>

namespace Aen {

	class AEN_DECLSPEC MaterialIHandler {
		private:

		MaterialIHandler();

		static const bool MaterialInstanceExist(const uint32_t& id) {
			return m_materialInstance.count(id) > 0;
		}

		static void CreateMaterialInstance(const uint32_t& id) {
			m_materialInstance.emplace(id, AEN_NEW MaterialInstance());
		}

		static void RemoveMaterial(const uint32_t& id) {
			if(m_materialInstance.count(id) > 0) {
				delete m_materialInstance.at(id);
				m_materialInstance.at(id) = nullptr;
				m_materialInstance.erase(id);
			}
		}

		static MaterialInstance& GetMaterialInstance(const uint32_t& id) {
			if(m_materialInstance.count(id) > 0)
				return *m_materialInstance.at(id);

			throw;
		}

		static std::unordered_map<uint32_t, MaterialInstance*> m_materialInstance;

		friend class MaterialInstance;
		friend class Entity;
		friend class Renderer;
	};
}