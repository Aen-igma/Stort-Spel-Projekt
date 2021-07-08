#include "PCH.h"
#include "Material.h"

namespace Aen {
    
    MaterialInstance::~MaterialInstance() {
        for(int i = 0; i < 3; i++)
            m_textures[i] = nullptr;
    }

    MaterialInstance::MaterialInstance()
        :m_dBuffers(), m_material(nullptr), m_textures{nullptr, nullptr, nullptr} {}

    MaterialInstance::MaterialInstance(Material& material)
        :m_dBuffers(material.m_bLayouts.size()), m_material(&material), m_textures{nullptr, nullptr, nullptr} {

        for(uint32_t i = 0; i < m_dBuffers.size(); i++)
            m_dBuffers[i].Create(material.m_bLayouts[i].m_dbLayout);
    }

    void MaterialInstance::Create(Material& material) {

        m_dBuffers.resize(material.m_bLayouts.size());
        m_material = &material;

        for(uint32_t i = 0; i < m_dBuffers.size(); i++)
            m_dBuffers[i].Create(material.m_bLayouts[i].m_dbLayout);
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