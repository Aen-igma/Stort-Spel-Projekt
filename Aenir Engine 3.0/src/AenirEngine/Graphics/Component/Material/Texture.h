#pragma once
#include"..\..\Graphics.h"

namespace Aen {

	class Texture {
		public:
		Texture() = default;
		Texture(const std::string& dir);

		void LoadTexture(const std::string& dir);

		private:
		~Texture() = default;

		ShaderResource m_shaderResource;

		friend class TextureHandler;
	};
}