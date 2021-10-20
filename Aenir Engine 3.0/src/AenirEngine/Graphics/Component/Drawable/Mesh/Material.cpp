#include"PCH.h"
#include"Material.h"
#include"Core/GlobalSettings.h"
#include"Importer/ObjImporter.h"

namespace Aen {

    ShaderModel::ShaderModel(Window& window)
        :m_window(window) {}

    const bool ShaderModel::Create(const ShaderModelDesc& desc) {

        // Shaders

        if(!m_PShader.Create(desc.PSDir))
            return false;

        if(!m_CShader.Create(desc.CSDir))
            return false;

        // Reflections

        m_PSReflect.Create(m_PShader);
        m_CSReflect.Create(m_CShader);

        // GBuffer

        uint32_t outputCount = m_PSReflect.GetOutputDesc().size();

        if(outputCount > 8)
            throw;

        m_gBuffer.Create(m_window, outputCount);

        // Samplers

        UINT samplerRegister = UINT_MAX;
        for(auto& i : m_PSReflect.GetBindDescs()) {
            if(i.second.Type == D3D_SIT_SAMPLER) {
                samplerRegister = i.second.BindPoint;
                break;
            }
        }

        if(samplerRegister == UINT_MAX)
            return false;

        m_samplerData.first = samplerRegister;
        m_samplerData.second.Create(desc.samplerType);

        // Buffer Name Test

        if(desc.bufferName.empty())
            throw;

        m_bufferName = desc.bufferName;

        // Slots
        
        std::optional<D3D11_SHADER_INPUT_BIND_DESC> bDescDiffuseMap     = m_PSReflect.GetBindDescByName(    "Aen_DiffuseMap"        );
        std::optional<D3D11_SHADER_INPUT_BIND_DESC> bDescNormalMap      = m_PSReflect.GetBindDescByName(    "Aen_NormalMap"         );
        std::optional<D3D11_SHADER_INPUT_BIND_DESC> bDescEmissionMap    = m_PSReflect.GetBindDescByName(    "Aen_EmissionMap"       );
        std::optional<D3D11_SHADER_INPUT_BIND_DESC> bDescOpacityMap     = m_PSReflect.GetBindDescByName(    "Aen_OpacityMap"        );
        std::optional<D3D11_SHADER_INPUT_BIND_DESC> bDescTranform       = m_PSReflect.GetBindDescByName(    "Aen_CB_Transform"      );
        std::optional<D3D11_SHADER_INPUT_BIND_DESC> bDescLightCount     = m_PSReflect.GetBindDescByName(    "Aen_CB_LightCount"     );
        std::optional<D3D11_SHADER_INPUT_BIND_DESC> bDescCamera         = m_PSReflect.GetBindDescByName(    "Aen_CB_Camera"         );
        std::optional<D3D11_SHADER_INPUT_BIND_DESC> bDescUseTexture     = m_PSReflect.GetBindDescByName(    "Aen_CB_UseTexture"     );
        std::optional<D3D11_SHADER_INPUT_BIND_DESC> bDescLight          = m_PSReflect.GetBindDescByName(    "Aen_SB_Light"          );
        std::optional<D3D11_SHADER_INPUT_BIND_DESC> bDescLightGrid      = m_PSReflect.GetBindDescByName(    "Aen_LightGrid"         );
        std::optional<D3D11_SHADER_INPUT_BIND_DESC> bDescLightIndexList = m_PSReflect.GetBindDescByName(    "Aen_LightIndexList"    );
        std::optional<D3D11_SHADER_INPUT_BIND_DESC> bDescDBuffer        = m_PSReflect.GetBindDescByName(    m_bufferName            );

        m_slots[0] = (bDescDiffuseMap)          ? bDescDiffuseMap.value().BindPoint         : UINT_MAX;
        m_slots[1] = (bDescNormalMap)           ? bDescNormalMap.value().BindPoint          : UINT_MAX;
        m_slots[2] = (bDescEmissionMap)         ? bDescEmissionMap.value().BindPoint        : UINT_MAX;
        m_slots[3] = (bDescOpacityMap)          ? bDescOpacityMap.value().BindPoint         : UINT_MAX;

        m_slots[4] = (bDescTranform)            ? bDescTranform.value().BindPoint           : UINT_MAX;
        m_slots[5] = (bDescLightCount)          ? bDescLightCount.value().BindPoint         : UINT_MAX;
        m_slots[6] = (bDescCamera)              ? bDescCamera.value().BindPoint             : UINT_MAX;
        m_slots[7] = (bDescUseTexture)          ? bDescUseTexture.value().BindPoint         : UINT_MAX;
        m_slots[8] = (bDescLight)               ? bDescLight.value().BindPoint              : UINT_MAX;

        m_slots[9] = (bDescLightGrid)           ? bDescLightGrid.value().BindPoint          : UINT_MAX;
        m_slots[10] = (bDescLightIndexList)     ? bDescLightIndexList.value().BindPoint     : UINT_MAX;
        m_slots[11] = (bDescDBuffer)            ? bDescDBuffer.value().BindPoint            : UINT_MAX;

        bDescTranform       = m_CSReflect.GetBindDescByName(    "Aen_CB_Transform"      );
        bDescLightCount     = m_CSReflect.GetBindDescByName(    "Aen_CB_LightCount"     );
        bDescCamera         = m_CSReflect.GetBindDescByName(    "Aen_CB_Camera"         );
        bDescUseTexture     = m_CSReflect.GetBindDescByName(    "Aen_CB_UseTexture"     );
        bDescLight          = m_CSReflect.GetBindDescByName(    "Aen_SB_Light"          );
        bDescDBuffer        = m_CSReflect.GetBindDescByName(    m_bufferName            );

        m_slots[12] = (bDescTranform)            ? bDescTranform.value().BindPoint           : UINT_MAX;
        m_slots[13] = (bDescLightCount)          ? bDescLightCount.value().BindPoint         : UINT_MAX;
        m_slots[14] = (bDescCamera)              ? bDescCamera.value().BindPoint             : UINT_MAX;
        m_slots[15] = (bDescUseTexture)          ? bDescUseTexture.value().BindPoint         : UINT_MAX;
        m_slots[16] = (bDescLight)               ? bDescLight.value().BindPoint              : UINT_MAX;
        m_slots[17] = (bDescDBuffer)             ? bDescDBuffer.value().BindPoint            : UINT_MAX;

        return true;
    }

    Material::Material(const bool& useDefaultShader) 
        :m_pShaderModel(nullptr), m_dBuffer(),
        m_textures{nullptr, nullptr, nullptr, nullptr} {
        
        if(useDefaultShader)
            CreateDefault();
    }
    
    Material::Material(ShaderModel& shaderModel)
        :m_pShaderModel(&shaderModel), m_dBuffer(shaderModel.m_dbLayout), 
        m_textures{nullptr, nullptr, nullptr, nullptr} {}

    Material::Material(const std::string& shaderName)
        : m_pShaderModel(&Resource::GetShader(shaderName)), m_dBuffer(Resource::GetShader(shaderName).m_dbLayout),
        m_textures{nullptr, nullptr, nullptr, nullptr} {}

    void Material::Create(ShaderModel& shaderModel) {
        m_pShaderModel = &shaderModel;
        m_dBuffer.Create(shaderModel.m_dbLayout);
    }

    void Material::Create(const std::string& shaderName) {
        if(!Resource::ShaderExist(shaderName)) throw;
        m_pShaderModel = &Resource::GetShader(shaderName);
        m_dBuffer.Create(m_pShaderModel->m_dbLayout);
    }

    void Material::CreateDefault() {
        m_pShaderModel = GlobalSettings::GetDefaultShader();
        m_dBuffer.Create(m_pShaderModel->m_dbLayout);
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
        if(!Aen::Resource::TextureExist(textureName)) throw;
        m_textures[0] = &Aen::Resource::GetTexture(textureName);
    }

    void Material::SetNormalMap(const std::string& textureName) {
        if(!Aen::Resource::TextureExist(textureName)) throw;
        m_textures[1] = &Aen::Resource::GetTexture(textureName);
    }

    void Material::SetEmissionMap(const std::string& textureName) {
        if(!Aen::Resource::TextureExist(textureName)) throw;
        m_textures[2] = &Aen::Resource::GetTexture(textureName);
    }

    void Material::SetOpacityMap(const std::string& textureName) {
        if(!Aen::Resource::TextureExist(textureName)) throw;
        m_textures[3] = &Aen::Resource::GetTexture(textureName);
    }

    void Material::LoadeAndSetDiffuseMap(const std::string& dir) {
        m_textures[0] = &Aen::Resource::CreateTexture(GetNameFromPath(dir));
        m_textures[0]->LoadTexture(dir);
    }

    void Material::LoadeAndSetNormalMap(const std::string& dir) {
        m_textures[1] = &Aen::Resource::CreateTexture(GetNameFromPath(dir));
        m_textures[1]->LoadTexture(dir);
    }

    void Material::LoadeAndSetEmissionMap(const std::string& dir) {
        m_textures[2] = &Aen::Resource::CreateTexture(GetNameFromPath(dir));
        m_textures[2]->LoadTexture(dir);
    }

    void Material::LoadeAndSetOpacityMap(const std::string& dir) {
        m_textures[3] = &Aen::Resource::CreateTexture(GetNameFromPath(dir));
        m_textures[3]->LoadTexture(dir);
    }

    ElRef Material::operator[](const std::string& name) {
        return m_dBuffer[name];
    }
}