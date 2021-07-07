#pragma once
#include"..\..\Graphics.h"
#include<unordered_map>

namespace Aen {

	class MeshInstance;

	class MeshIHandler {
		private:

		static const bool MeshInstanceExist(const uint32_t& id) {
			return GetMesheInstances().count(id) > 0;
		}

		static void CreateMeshInstance(const uint32_t& id) {
			GetMesheInstances().emplace(id, AEN_NEW MeshInstance());
		}

		static void RemoveMeshInstance(const uint32_t& id) {
			if(GetMesheInstances().count(id) > 0) {
				delete GetMesheInstances().at(id);
				GetMesheInstances().at(id) = nullptr;
				GetMesheInstances().erase(id);
			}
		}

		static MeshInstance& GetMeshInstance(const uint32_t& id) {
			if(GetMesheInstances().count(id) > 0)
				return *GetMesheInstances().at(id);
		}

		static std::unordered_map<uint32_t, MeshInstance*> GetMesheInstances() {
			static std::unordered_map<uint32_t, MeshInstance*> mesheInstances;
			return mesheInstances;
		}

		friend class MeshInstance;
		friend class Entity;
	};
}