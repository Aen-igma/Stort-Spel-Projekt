#pragma once
#include"Texture.h"
#include<unordered_map>

namespace Aen {

	class AEN_DECLSPEC TextureHandler {
		public:

		static Texture* CreateTexture(const std::string& name) {
			m_textures.emplace(name, AEN_NEW Texture());
			return m_textures.at(name);
		}

		static void RemoveTexture(const std::string& name) {
			if(m_textures.count(name) > 0) {
				delete m_textures.at(name);
				m_textures.at(name) = nullptr;
				m_textures.erase(name);
			}
		}

		static Texture& GetMaterial(const std::string& name) {
			if(m_textures.count(name) > 0)
				return *m_textures.at(name);

			throw;
		}

		private:

		TextureHandler();

		static void Destroy() {
			for(auto& i : m_textures)
				if(i.second) {
					delete i.second;
					i.second = nullptr;
				}
		}

		static std::unordered_map<std::string, Texture*> m_textures;

		friend class Texture;
		friend class GameLoop;
	};
}