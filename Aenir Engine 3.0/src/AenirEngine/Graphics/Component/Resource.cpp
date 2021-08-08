#include"PCH.h"
#include"Resource.h"

namespace Aen {

	std::unordered_map<std::string, ShaderModel*> Resource::m_shaders;
	std::unordered_map<std::string, Material*> Resource::m_materials;
	std::unordered_map<std::string, Texture*> Resource::m_textures;
	std::unordered_map<std::string, Mesh*> Resource::m_meshes;

}