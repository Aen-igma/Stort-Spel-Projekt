#pragma once
#include"Shader.h"
#include"Helper.h"
#include"ThirdParty/WICLoader/Inc\WICTextureLoader.h"
namespace Aen {

	struct AEN_DECLSPEC ShaderResource : public GCore {
		public:
		~ShaderResource();
		ShaderResource();

		void LoadResource(const std::string& dir);
		
		friend class RenderSystem;

		private:
		ComShaderResourceView m_srv;
	};
}
