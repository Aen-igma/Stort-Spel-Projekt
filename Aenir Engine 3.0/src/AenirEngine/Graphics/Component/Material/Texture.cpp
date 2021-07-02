#include "PCH.h"
#include "Texture.h"

namespace Aen {

	Texture::Texture(const std::string& name)
		:m_shaderResource(), m_name(name) {
		TextureHandler::GetTextures().emplace(name, this);
	}

	Texture::Texture(const std::string& name, const std::string& dir)
		:m_shaderResource(), m_name(name) {
		m_shaderResource.LoadResource(dir);
		TextureHandler::GetTextures().emplace(name, this);
	}
	
	void Texture::LoadTexture(const std::string& dir) {
		m_shaderResource.LoadResource(dir);
	}

	Texture::~Texture() {
		TextureHandler::GetTextures().erase(m_name);
	}
}