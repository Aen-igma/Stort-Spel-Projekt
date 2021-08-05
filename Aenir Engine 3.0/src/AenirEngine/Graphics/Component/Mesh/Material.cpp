#include"PCH.h"
#include"Material.h"
#include"Core/GlobalSettings.h"
#include"Importer/ObjImporter.h"

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

    void Material::SetDiffuseMap(const std::string& textureName) {
        if(!Aen::ResourceHandler::TextureExist(textureName)) throw;
        m_textures[0] = &Aen::ResourceHandler::GetTexture(textureName);
    }

    void Material::SetNormalMap(const std::string& textureName) {
        if(!Aen::ResourceHandler::TextureExist(textureName)) throw;
        m_textures[1] = &Aen::ResourceHandler::GetTexture(textureName);
    }

    void Material::SetEmissionMap(const std::string& textureName) {
        if(!Aen::ResourceHandler::TextureExist(textureName)) throw;
        m_textures[2] = &Aen::ResourceHandler::GetTexture(textureName);
    }

    void Material::SetOpacityMap(const std::string& textureName) {
        if(!Aen::ResourceHandler::TextureExist(textureName)) throw;
        m_textures[3] = &Aen::ResourceHandler::GetTexture(textureName);
    }

    void Material::LoadeAndSetDiffuseMap(const std::string& dir) {
        m_textures[0] = &Aen::ResourceHandler::CreateTexture(GetNameFromPath(dir));
        m_textures[0]->LoadTexture(dir);
    }

    void Material::LoadeAndSetNormalMap(const std::string& dir) {
        m_textures[1] = &Aen::ResourceHandler::CreateTexture(GetNameFromPath(dir));
        m_textures[1]->LoadTexture(dir);
    }

    void Material::LoadeAndSetEmissionMap(const std::string& dir) {
        m_textures[2] = &Aen::ResourceHandler::CreateTexture(GetNameFromPath(dir));
        m_textures[2]->LoadTexture(dir);
    }

    void Material::LoadeAndSetOpacityMap(const std::string& dir) {
        m_textures[3] = &Aen::ResourceHandler::CreateTexture(GetNameFromPath(dir));
        m_textures[3]->LoadTexture(dir);
    }

    ElRef Material::operator[](const std::string& name) {
        return m_dBuffer[name];
    }
}