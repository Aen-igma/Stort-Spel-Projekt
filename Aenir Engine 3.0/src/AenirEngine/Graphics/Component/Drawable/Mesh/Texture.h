#pragma once
#include"..\..\..\Graphics.h"

namespace Aen {

	class AEN_DECLSPEC Texture {
		public:
		Texture() = default;
		Texture(const std::string& dir);

		void LoadTexture(const std::string& dir);

		private:
		~Texture() = default;

		ShaderResource m_shaderResource;

		friend class Resource;
		friend class Renderer;
		friend class MeshInstance;
		friend class GlobalSettings;
	};
}