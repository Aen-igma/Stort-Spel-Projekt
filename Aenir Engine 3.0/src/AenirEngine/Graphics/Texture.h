#pragma once
#include"TextureHandler.h"

namespace Aen {

	class Texture {
		public:
		~Texture();
		Texture();
		Texture(const std::string& dir);

		void LoadTexture(const std::string& dir);

		private:
		ShaderResource m_shaderResource;
		int m_id;
		static int m_iDs;
	};
}