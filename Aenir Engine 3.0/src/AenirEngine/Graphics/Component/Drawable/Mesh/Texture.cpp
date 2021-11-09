#include "PCH.h"
#include "Texture.h"
#include<thread>

namespace Aen {

	Texture::Texture(const std::string& dir)
		:m_shaderResource() {
		m_shaderResource.LoadResource(dir);
	}
	
	void Texture::LoadTexture(const std::string& dir) {
		//std::thread worker(&ShaderResource::LoadResource, &m_shaderResource, dir);
		//worker.join();
		m_shaderResource.LoadResource(dir);
	}
}