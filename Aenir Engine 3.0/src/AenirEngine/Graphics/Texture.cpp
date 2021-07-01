#include "PCH.h"
#include "Texture.h"

namespace Aen {

	int Texture::m_iDs(0);

	Texture::~Texture() {
		TextureHandler::GetTextures().erase(m_id);
	}

	Texture::Texture()
		:m_shaderResource(), m_id(m_iDs) {
		TextureHandler::GetTextures().insert(std::pair<int, Texture*>(m_iDs, this));
		m_iDs++;
	}

	Texture::Texture(const std::string& dir) 
		:m_shaderResource(), m_id(m_iDs) {
		m_shaderResource.LoadResource(dir);
		TextureHandler::GetTextures().insert(std::pair<int, Texture*>(m_iDs, this));
		m_iDs++;
	}
	
	void Texture::LoadTexture(const std::string& dir) {
		m_shaderResource.LoadResource(dir);
	}
}