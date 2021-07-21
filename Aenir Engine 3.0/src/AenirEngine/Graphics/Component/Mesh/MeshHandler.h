#pragma once
#include"Mesh.h"
#include<unordered_map>

namespace Aen {

	class AEN_DECLSPEC MeshHandler {
		public:

		static const bool MeshExist(const std::string& name) {
			return m_meshes.count(name) > 0;
		}

		static Mesh* CreateMesh(const std::string& name) {
			m_meshes.emplace(name, AEN_NEW Mesh());
			return m_meshes.at(name);
		}

		static void CreateMesh(const std::string& name, const std::string& dir) {
			m_meshes.emplace(name, AEN_NEW Mesh(dir));
		}

		static void RemoveMesh(const std::string& name) {
			if(m_meshes.count(name) > 0) {
				delete m_meshes.at(name);
				m_meshes.at(name) = nullptr;
				m_meshes.erase(name);

			}
		}
		
		static Mesh& GetMesh(const std::string& name) {
			if(m_meshes.count(name) > 0)
				return *m_meshes.at(name);

			throw;
		}
		private:

		MeshHandler();

		static void Destroy() {
			for(auto& i : m_meshes)
				if(i.second) {
					delete i.second;
					i.second = nullptr;
				}
		}

		static std::unordered_map<std::string, Mesh*> m_meshes;

		friend class Mesh;
		friend class GameLoop;
	};
}