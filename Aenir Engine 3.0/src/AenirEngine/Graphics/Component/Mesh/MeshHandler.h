#pragma once
#include"Mesh.h"
#include<unordered_map>

namespace Aen {

	class MeshHandler {
		public:

		static const bool MeshExist(const std::string& name) {
			return GetMeshes().count(name) > 0;
		}

		static void CreateMesh(const std::string& name) {
			GetMeshes().emplace(name, AEN_NEW Mesh());
		}

		static void CreateMesh(const std::string& name, const std::string& dir) {
			GetMeshes().emplace(name, AEN_NEW Mesh(dir));
		}

		static void RemoveMesh(const std::string& name) {
			if(GetMeshes().count(name) > 0) {
				delete GetMeshes().at(name);
				GetMeshes().at(name) = nullptr;
				GetMeshes().erase(name);

			}
		}
		
		static void Destroy() {
			for(auto& i : GetMeshes())
				if(i.second) {
					delete i.second;
					i.second = nullptr;
				}
		}

		static Mesh& GetMesh(const std::string& name) {
			if(GetMeshes().count(name) > 0)
				return *GetMeshes().at(name);

			throw;
		}
		
		private:

		static std::unordered_map<std::string, Mesh*> GetMeshes() {
			static std::unordered_map<std::string, Mesh*> meshes;
			return meshes;
		}

		friend class Mesh;
		friend class GameLoop;
	};
}