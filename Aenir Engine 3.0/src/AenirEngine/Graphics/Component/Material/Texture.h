#pragma once
#include"TextureHandler.h"

namespace Aen {

	class Texture {
		public:
		Texture(const std::string& name);
		Texture(const std::string& name, const std::string& dir);

		void LoadTexture(const std::string& dir);

		private:
		~Texture();

		ShaderResource m_shaderResource;
		std::string m_name;

		friend class TextureHandler;
	};
}