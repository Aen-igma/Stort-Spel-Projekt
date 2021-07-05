#pragma once
#include"..\..\Graphics.h"
#include<unordered_map>

namespace Aen {

	class Mesh;

	class MeshHandler {
		private:

		static Mesh& GetMesh(const uint32_t& id) {
			if(GetMeshes().count(id) > 0)
				return *GetMeshes().at(id);
		}

		static std::unordered_map<uint32_t, Mesh*> GetMeshes() {
			static std::unordered_map<uint32_t, Mesh*> meshes;
			return meshes;
		}

		friend class Mesh;
	};
}