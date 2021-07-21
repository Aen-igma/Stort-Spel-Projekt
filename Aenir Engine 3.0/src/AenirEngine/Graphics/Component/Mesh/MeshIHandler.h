#pragma once
#include"Mesh.h"
#include<unordered_map>

namespace Aen {

	class AEN_DECLSPEC MeshIHandler {
		private:

		MeshIHandler();

		static const bool MeshInstanceExist(const uint32_t& id) {
			return m_mesheInstances.count(id) > 0;
		}

		static void CreateMeshInstance(const uint32_t& id) {
			m_mesheInstances.emplace(id, AEN_NEW MeshInstance());
		}

		static void RemoveMeshInstance(const uint32_t& id) {
			if(m_mesheInstances.count(id) > 0) {
				delete m_mesheInstances.at(id);
				m_mesheInstances.at(id) = nullptr;
				m_mesheInstances.erase(id);
			}
		}

		static MeshInstance& GetMeshInstance(const uint32_t& id) {
			if(m_mesheInstances.count(id) > 0)
				return *m_mesheInstances.at(id);
		}
		
		static std::unordered_map<uint32_t, MeshInstance*> m_mesheInstances;

		friend class MeshInstance;
		friend class Entity;
		friend class Renderer;
	};
}