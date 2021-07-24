#include"PCH.h"
#include"ResourceHandler.h"

namespace Aen {

	std::unordered_map<std::string, ShaderModel*> ResourceHandler::m_shaders;
	std::unordered_map<std::string, Material*> ResourceHandler::m_materials;
	std::unordered_map<std::string, Texture*> ResourceHandler::m_textures;
	std::unordered_map<std::string, Mesh*> ResourceHandler::m_meshes;

}