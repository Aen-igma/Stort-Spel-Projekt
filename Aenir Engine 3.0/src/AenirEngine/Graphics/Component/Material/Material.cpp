#include"PCH.h"
#include"Material.h"
#include"Core/GlobalSettings.h"

namespace Aen {

    Material::Material(const bool& useDefaultShader) :m_pShader(nullptr), m_dBuffers(0),
        m_textures{nullptr, nullptr, nullptr, nullptr} {
        
        if(useDefaultShader)
            CreateDefault();
    }
    
    Material::Material(ShaderModel& shaderModel)
        :m_pShader(&shaderModel), m_dBuffers(shaderModel.m_dbLayouts.size()), 
        m_textures{nullptr, nullptr, nullptr, nullptr} {
        
        for(uint32_t i = 0; i < m_dBuffers.size(); i++)
            m_dBuffers[i] = AEN_NEW DBuffer(shaderModel.m_dbLayouts[i].second);
    }

    void Material::Create(ShaderModel& shaderModel) {
        m_pShader = &shaderModel;
        m_dBuffers.resize(shaderModel.m_dbLayouts.size());

        for(uint32_t i = 0; i < m_dBuffers.size(); i++)
            m_dBuffers[i] = AEN_NEW DBuffer(shaderModel.m_dbLayouts[i].second);
    }

    void Material::CreateDefault() {
        m_pShader = GlobalSettings::GetDefaultShader();
        m_dBuffers.resize(m_pShader->m_dbLayouts.size());

        for(uint32_t i = 0; i < m_dBuffers.size(); i++)
            m_dBuffers[i] = AEN_NEW DBuffer(m_pShader->m_dbLayouts[i].second);
    }

    void Material::SetDiffuseMap(Texture& texture) {
        m_textures[0] = &texture;
    }

    void Material::SetNormalMap(Texture& texture) {
        m_textures[1] = &texture;
    }

    void Material::SetEmissionMap(Texture& texture) {
        m_textures[2] = &texture;
    }

    void Material::SetOpacityMap(Texture& texture) {
        m_textures[3] = &texture;
    }

    Material::~Material() {
        for(auto& i : m_dBuffers)
            if(i) {
                delete i;
                i = nullptr;
            }
    }


    MaterialInstance::~MaterialInstance() {
        m_pMaterial = nullptr;
    }

    MaterialInstance::MaterialInstance(Material*& material)
        :m_pMaterial(material) {}

    void MaterialInstance::SetMaterial(Material*& material) {
        m_pMaterial = material;
    }

}