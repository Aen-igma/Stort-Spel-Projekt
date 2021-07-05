#pragma once
#include"..\..\Graphics.h"
#include<unordered_map>

namespace Aen {

	class MeshInstance;

	class MeshIHandler {
		private:

		static MeshInstance& GetMeshInstance(const uint32_t& id) {
			if(GetMesheInstances().count(id) > 0)
				return *GetMesheInstances().at(id);
		}

		static std::unordered_map<uint32_t, MeshInstance*> GetMesheInstances() {
			static std::unordered_map<uint32_t, MeshInstance*> mesheInstances;
			return mesheInstances;
		}

		friend class MeshInstance;
	};
}