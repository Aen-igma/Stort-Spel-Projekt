#include "PCH.h"
#include "Texture.h"

namespace Aen {

	Texture::Texture(const std::string& dir)
		:m_shaderResource() {
		m_shaderResource.LoadResource(dir);
	}
	
	void Texture::LoadTexture(const std::string& dir) {
		m_shaderResource.LoadResource(dir);
	}
}