#include"PCH.h"
#include"Material.h"
#include"Core/GlobalSettings.h"

namespace Aen {

    Material::Material(const bool& useDefaultShader) 
        :m_pShaderModel(nullptr), m_dBuffer(),
        m_textures{nullptr, nullptr, nullptr, nullptr} {
        
        if(useDefaultShader)
            CreateDefault();
    }
    
    Material::Material(ShaderModel& shaderModel)
        :m_pShaderModel(&shaderModel), m_dBuffer(shaderModel.m_dbLayout.second), 
        m_textures{nullptr, nullptr, nullptr, nullptr} {}

    void Material::Create(ShaderModel& shaderModel) {
        m_pShaderModel = &shaderModel;
        m_dBuffer.Create(shaderModel.m_dbLayout.second);
    }

    void Material::CreateDefault() {
        m_pShaderModel = GlobalSettings::GetDefaultShader();
        m_dBuffer.Create(m_pShaderModel->m_dbLayout.second);
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

    ElRef Material::operator[](const std::string& name) {
        return m_dBuffer[name];
    }

    MaterialInstance::~MaterialInstance() {
        m_pMaterial = nullptr;
    }

    MaterialInstance::MaterialInstance(Material& material)
        :m_pMaterial(&material) {}

    void MaterialInstance::SetMaterial(Material& material) {
        m_pMaterial = &material;
    }

}