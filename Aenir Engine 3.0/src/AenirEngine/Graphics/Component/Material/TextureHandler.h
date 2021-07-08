#pragma once
#include"Texture.h"
#include<unordered_map>

namespace Aen {

	class TextureHandler {
		public:

		static void CreateTexture(const std::string& name) {
			GetTextures().emplace(name, AEN_NEW Texture());
		}

		static void RemoveTexture(const std::string& name) {
			if(GetTextures().count(name) > 0) {
				delete GetTextures().at(name);
				GetTextures().at(name) = nullptr;
				GetTextures().erase(name);
			}
		}

		static Texture& GetMaterial(const std::string& name) {
			if(GetTextures().count(name) > 0)
				return *GetTextures().at(name);

			throw;
		}

		private:
		static std::unordered_map<std::string, Texture*> GetTextures() {
			static std::unordered_map<std::string, Texture*> textures;
			return textures;
		}

		static void Destroy() {
			for(auto& i : GetTextures())
				if(i.second) {
					delete i.second;
					i.second = nullptr;
				}
		}

		friend class Texture;
		friend class GameLoop;
	};
}