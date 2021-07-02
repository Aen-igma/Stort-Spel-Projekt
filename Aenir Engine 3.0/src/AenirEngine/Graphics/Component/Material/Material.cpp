#include "PCH.h"
#include "Material.h"

namespace Aen {

    Material::Material(const std::string& name) 
        :m_name(name) {
        MaterialHandler::GetMaterials().emplace(name, this);
    }

    Material::~Material() {
        MaterialHandler::GetMaterials().erase(m_name);
    }

    
    MaterialInstance::~MaterialInstance() {
        for(int i = 0; i < 3; i++)
            m_textures[i] = nullptr;

        MaterialIHandler::GetMaterialInstances().erase(m_id);
    }

    MaterialInstance::MaterialInstance(const uint32_t& id, Material& material)
        :m_dBuffers(material.m_dbLayouts.size()), m_material(&material), m_textures{nullptr, nullptr, nullptr}, m_id(id) {
        
        MaterialIHandler::GetMaterialInstances().emplace(id, this);

        for(uint32_t i = 0; i < m_dBuffers.size(); i++)
            m_dBuffers[i].Create(material.m_dbLayouts[i]);
    }

    void MaterialInstance::SetDiffuseMap(Texture& texture) {
        m_textures[0] = &texture;
    }

    void MaterialInstance::SetNormalMap(Texture& texture) {
        m_textures[1] = &texture;
    }

    void MaterialInstance::SetEmissionMap(Texture& texture) {
        m_textures[2] = &texture;
    }
}