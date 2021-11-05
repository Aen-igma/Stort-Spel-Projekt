#pragma once
#include"Drawable/Mesh/Material.h"
#include"Drawable/Mesh/Texture.h"
#include"Drawable/Mesh/Mesh.h"

#include<unordered_map>

namespace Aen {

	class AEN_DECLSPEC Resource {
		public:

		// -------------- ShaderModel ----------- //

		static const bool ShaderExist(const std::string& name) {
			return m_shaders.count(name) > 0;
		}

		static ShaderModel& CreateShader(const std::string& name, Window& window) {
			if (ShaderExist(name)) return *m_shaders.at(name);
			m_shaders.emplace(name, AEN_NEW ShaderModel(window));
			return *m_shaders.at(name);
		}

		static void RemoveShader(const std::string& name) {
			if(m_shaders.count(name) > 0) {
				delete m_shaders.at(name);
				m_shaders.at(name) = nullptr;
				m_shaders.erase(name);
			}
		}

		static void RemoveAllShaders() {
			for (auto& s : m_shaders) {
				if (s.second) {
					delete s.second;
					s.second = nullptr;
				}
			}

			m_shaders.clear();
		}

		static ShaderModel& GetShader(const std::string& name) {
			if(m_shaders.count(name) > 0)
				return *m_shaders.at(name);

			throw;
		}

		// -------------- Material ------------- //

		static const bool MaterialExist(const std::string& name) {
			return m_materials.count(name) > 0;
		}

		static Material& CreateMaterial(const std::string& name, const bool& useDefaultShader = true) {
			m_materials.emplace(name, AEN_NEW Material(useDefaultShader));
			return *m_materials.at(name);
		}

		static Material& CreateMaterial(const std::string& name, const std::string& shaderName) {
			if (MaterialExist(name)) return *m_materials.at(name);
			if(m_shaders.count(shaderName) == 0) throw;
			m_materials.emplace(name, AEN_NEW Material(*m_shaders.at(shaderName)));
			return *m_materials.at(name);
		}

		static void RemoveMaterial(const std::string& name) {
			if(m_materials.count(name) > 0) {
				delete m_materials.at(name);
				m_materials.at(name) = nullptr;
				m_materials.erase(name);
			}
		}

		static void RemoveAllMaterials() {
			for (auto& m : m_materials) {
				if (m.second && m.first != "DefaultMaterial") {
					delete m.second;
					m.second = nullptr;
				}
			}

			m_materials.clear();
		}

		static Material& GetMaterial(const std::string& name) {
			if(m_materials.count(name) > 0)
				return *m_materials.at(name);

			throw;
		}

		// ---------------- Texture --------------- //

		static const bool TextureExist(const std::string& name) {
			return m_textures.count(name) > 0;
		}

		static Texture& CreateTexture(const std::string& name) {
			if (TextureExist(name)) return *m_textures.at(name);
			m_textures.emplace(name, AEN_NEW Texture());
			return *m_textures.at(name);
		}

		static void RemoveTexture(const std::string& name) {
			if(m_textures.count(name) > 0) {
				delete m_textures.at(name);
				m_textures.at(name) = nullptr;
				m_textures.erase(name);
			}
		}

		static void RemoveAllTextures() {
			for (auto& t : m_textures) {
				if (t.second && t.first != "DefaultTexture") {
					delete t.second;
					t.second = nullptr;
				}
			}

			m_textures.clear();
		}

		static Texture& GetTexture(const std::string& name) {
			if(m_textures.count(name) > 0)
				return *m_textures.at(name);

			throw;
		}

		// ------------------ Mesh ----------------- //

		static const bool MeshExist(const std::string& name) {
			return m_meshes.count(name) > 0;
		}

		static Mesh& CreateMesh(const std::string& name) {
			if (MeshExist(name)) return *m_meshes.at(name);
			m_meshes.emplace(name, AEN_NEW Mesh());
			return *m_meshes.at(name);
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

		static void RemoveAllMeshes() {
			for (auto& m : m_meshes) {
				if (m.second) {
					delete m.second;
					m.second = nullptr;
				}
			}

			m_meshes.clear();
		}

		static Mesh& GetMesh(const std::string& name) {
			if(m_meshes.count(name) > 0)
				return *m_meshes.at(name);

			throw;
		}

		private:

		Resource();


		static void Destroy() {

			// -------------- ShaderModel ----------- //

			for(auto& i : m_shaders)
				if(i.second) {
					delete i.second;
					i.second = nullptr;
				}

			// -------------- Material -------------- //

			for(auto& i : m_materials)
				if(i.second) {
					delete i.second;
					i.second = nullptr;
				}

			// ---------------- Texture --------------- //

			for(auto& i : m_textures)
				if(i.second) {
					delete i.second;
					i.second = nullptr;
				}

			// --------------- Mesh -------------- //

			for(auto& i : m_meshes)
				if(i.second) {
					delete i.second;
					i.second = nullptr;
				}
		}

		static std::unordered_map<std::string, ShaderModel*> m_shaders;
		static std::unordered_map<std::string, Material*> m_materials;
		static std::unordered_map<std::string, Texture*> m_textures;
		static std::unordered_map<std::string, Mesh*> m_meshes;

		friend class Mesh;
		friend class Texture;
		friend class Material;
		friend class ShaderModel;
		friend class GameLoop;
	};

}