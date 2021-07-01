#pragma once
#include"Graphics.h"
#include<unordered_map>

namespace Aen {

	class Texture;

	struct TextureHandler {
		
		static std::unordered_map<int, Texture*> GetTextures() {
			static std::unordered_map<int, Texture*> textures;
			return textures;
		}
	};
}