#include"PCH.h"
#include"Material.h"
#include"Core/GlobalSettings.h"
#include"Importer/ObjImporter.h"

namespace Aen {

    ShaderModel::ShaderModel(Window& window)
        :m_window(window) {}

    const bool ShaderModel::Create(const ShaderModelDesc& desc) {

        // Shaders

        if(!m_VShaderPass1.Create(desc.VSDirPass1))
            return false;

        if(!m_PShaderPass1.Create(desc.PSDirPass1))
            return false;

        if(!m_VShaderPass2.Create(desc.VSDirPass2))
            return false;

        if(!m_PShaderPass2.Create(desc.PSDirPass2))
            return false;

        // Input Layouts

        m_iLayoutPass1.Create(m_VShaderPass1);
        m_iLayoutPass2.Create(m_VShaderPass2);

        // Reflections

        m_VSReflectPass1.Create(m_VShaderPass1);
        m_PSReflectPass1.Create(m_PShaderPass1);
        m_VSReflectPass2.Create(m_VShaderPass2);
        m_PSReflectPass2.Create(m_PShaderPass2);

        // Pass Transition Test

        UINT inputCount = 0;
        UINT outputCount = 0;

        for(auto& i : m_PSReflectPass2.GetBindDescs())
            if(i.second.Type == D3D_SIT_TEXTURE)
                inputCount ++;
        
        outputCount = static_cast<UINT>(m_PSReflectPass1.GetOutputDesc().size());
        
        if(inputCount != outputCount)
            throw;

        // GBuffer

        m_gBuffer.Create(m_window, static_cast<uint32_t>(m_PSReflectPass1.GetOutputDesc().size()));

        // Samplers

        UINT samplerRegister = UINT_MAX;
        for(auto& i : m_PSReflectPass1.GetBindDescs()) {
            if(i.second.Type == D3D_SIT_SAMPLER) {
                samplerRegister = i.second.BindPoint;
                break;
            }
        }

        if(samplerRegister == UINT_MAX)
            return false;

        m_samplerDataPass1.first = samplerRegister;
        m_samplerDataPass1.second.Create(desc.samplerTypePass1);

        samplerRegister = UINT_MAX;
        for(auto& i : m_PSReflectPass2.GetBindDescs()) {
            if(i.second.Type == D3D_SIT_SAMPLER) {
                samplerRegister = i.second.BindPoint;
                break;
            }
        }

        if(samplerRegister == UINT_MAX)
            return false;

        m_samplerDataPass2.first = samplerRegister;
        m_samplerDataPass2.second.Create(desc.samplerTypePass2);

        if(m_PSReflectPass1.GetOutputDesc().size() == 0)
            return false;

        // Buffer Name Test

        if(desc.bufferName.empty())
            throw;

        m_bufferName = desc.bufferName;

        // Slots

        std::optional<D3D11_SHADER_INPUT_BIND_DESC> bDescTranform = m_VSReflectPass1.GetBindDescByName("Aen_CB_Transform");
        std::optional<D3D11_SHADER_INPUT_BIND_DESC> bDescLightCount = m_VSReflectPass1.GetBindDescByName("Aen_CB_LightCount");
        std::optional<D3D11_SHADER_INPUT_BIND_DESC> bDescCamera = m_VSReflectPass1.GetBindDescByName("Aen_CB_Camera");
        std::optional<D3D11_SHADER_INPUT_BIND_DESC> bDescUseTexture = m_VSReflectPass1.GetBindDescByName("Aen_CB_UseTexture");
        std::optional<D3D11_SHADER_INPUT_BIND_DESC> bDescLight = m_VSReflectPass1.GetBindDescByName("Aen_SB_Light");

        m_slotsPass1[0] = (bDescTranform) ? bDescTranform.value().BindPoint : UINT_MAX;
        m_slotsPass1[1] = (bDescLightCount) ? bDescLightCount.value().BindPoint : UINT_MAX;
        m_slotsPass1[2] = (bDescCamera) ? bDescCamera.value().BindPoint : UINT_MAX;
        m_slotsPass1[3] = (bDescUseTexture) ? bDescUseTexture.value().BindPoint : UINT_MAX;
        m_slotsPass1[4] = (bDescLight) ? bDescLight.value().BindPoint : UINT_MAX;

        bDescTranform = m_PSReflectPass1.GetBindDescByName("Aen_CB_Transform");
        bDescLightCount = m_PSReflectPass1.GetBindDescByName("Aen_CB_LightCount");
        bDescCamera = m_PSReflectPass1.GetBindDescByName("Aen_CB_Camera");
        bDescUseTexture = m_PSReflectPass1.GetBindDescByName("Aen_CB_UseTexture");
        bDescLight = m_PSReflectPass1.GetBindDescByName("Aen_SB_Light");

        m_slotsPass1[5] = (bDescTranform) ? bDescTranform.value().BindPoint : UINT_MAX;
        m_slotsPass1[6] = (bDescLightCount) ? bDescLightCount.value().BindPoint : UINT_MAX;
        m_slotsPass1[7] = (bDescCamera) ? bDescCamera.value().BindPoint : UINT_MAX;
        m_slotsPass1[8] = (bDescUseTexture) ? bDescUseTexture.value().BindPoint : UINT_MAX;
        m_slotsPass1[9] = (bDescLight) ? bDescLight.value().BindPoint : UINT_MAX;

        std::optional<D3D11_SHADER_INPUT_BIND_DESC> bDescDiffuseMap = m_PSReflectPass1.GetBindDescByName("Aen_DiffuseMap");
        std::optional<D3D11_SHADER_INPUT_BIND_DESC> bDescNormalMap = m_PSReflectPass1.GetBindDescByName("Aen_NormalMap");
        std::optional<D3D11_SHADER_INPUT_BIND_DESC> bDescEmissionMap = m_PSReflectPass1.GetBindDescByName("Aen_EmissionMap");
        std::optional<D3D11_SHADER_INPUT_BIND_DESC> bDescOpacityMap = m_PSReflectPass1.GetBindDescByName("Aen_OpacityMap");

        m_slotsPass1[10] = (bDescDiffuseMap) ? bDescDiffuseMap.value().BindPoint : UINT_MAX;
        m_slotsPass1[11] = (bDescNormalMap) ? bDescNormalMap.value().BindPoint : UINT_MAX;
        m_slotsPass1[12] = (bDescEmissionMap) ? bDescEmissionMap.value().BindPoint : UINT_MAX;
        m_slotsPass1[13] = (bDescOpacityMap) ? bDescOpacityMap.value().BindPoint : UINT_MAX;

        std::optional<D3D11_SHADER_INPUT_BIND_DESC> bDescDBufferVS = m_VSReflectPass1.GetBindDescByName(m_bufferName);
        std::optional<D3D11_SHADER_INPUT_BIND_DESC> bDescDBufferPS = m_PSReflectPass1.GetBindDescByName(m_bufferName);

        m_slotsPass1[14] = (bDescDBufferVS) ? bDescDBufferVS.value().BindPoint : UINT_MAX;
        m_slotsPass1[15] = (bDescDBufferPS) ? bDescDBufferPS.value().BindPoint : UINT_MAX;



        bDescTranform = m_VSReflectPass2.GetBindDescByName("Aen_CB_Transform");
        bDescLightCount = m_VSReflectPass2.GetBindDescByName("Aen_CB_LightCount");
        bDescCamera = m_VSReflectPass2.GetBindDescByName("Aen_CB_Camera");
        bDescUseTexture = m_VSReflectPass2.GetBindDescByName("Aen_CB_UseTexture");
        bDescLight = m_VSReflectPass2.GetBindDescByName("Aen_SB_Light");

        m_slotsPass2[0] = (bDescTranform) ? bDescTranform.value().BindPoint : UINT_MAX;
        m_slotsPass2[1] = (bDescLightCount) ? bDescLightCount.value().BindPoint : UINT_MAX;
        m_slotsPass2[2] = (bDescCamera) ? bDescCamera.value().BindPoint : UINT_MAX;
        m_slotsPass2[3] = (bDescUseTexture) ? bDescUseTexture.value().BindPoint : UINT_MAX;
        m_slotsPass2[4] = (bDescLight) ? bDescLight.value().BindPoint : UINT_MAX;

        bDescTranform = m_PSReflectPass2.GetBindDescByName("Aen_CB_Transform");
        bDescLightCount = m_PSReflectPass2.GetBindDescByName("Aen_CB_LightCount");
        bDescCamera = m_PSReflectPass2.GetBindDescByName("Aen_CB_Camera");
        bDescUseTexture = m_PSReflectPass2.GetBindDescByName("Aen_CB_UseTexture");
        bDescLight = m_PSReflectPass2.GetBindDescByName("Aen_SB_Light");

        m_slotsPass2[5] = (bDescTranform) ? bDescTranform.value().BindPoint : UINT_MAX;
        m_slotsPass2[6] = (bDescLightCount) ? bDescLightCount.value().BindPoint : UINT_MAX;
        m_slotsPass2[7] = (bDescCamera) ? bDescCamera.value().BindPoint : UINT_MAX;
        m_slotsPass2[8] = (bDescUseTexture) ? bDescUseTexture.value().BindPoint : UINT_MAX;
        m_slotsPass2[9] = (bDescLight) ? bDescLight.value().BindPoint : UINT_MAX;

        bDescDiffuseMap = m_PSReflectPass2.GetBindDescByName("Aen_DiffuseMap");
        bDescNormalMap = m_PSReflectPass2.GetBindDescByName("Aen_NormalMap");
        bDescEmissionMap = m_PSReflectPass2.GetBindDescByName("Aen_EmissionMap");
        bDescOpacityMap = m_PSReflectPass2.GetBindDescByName("Aen_OpacityMap");

        m_slotsPass2[10] = (bDescDiffuseMap) ? bDescDiffuseMap.value().BindPoint : UINT_MAX;
        m_slotsPass2[11] = (bDescNormalMap) ? bDescNormalMap.value().BindPoint : UINT_MAX;
        m_slotsPass2[12] = (bDescEmissionMap) ? bDescEmissionMap.value().BindPoint : UINT_MAX;
        m_slotsPass2[13] = (bDescOpacityMap) ? bDescOpacityMap.value().BindPoint : UINT_MAX;

        bDescDBufferVS = m_VSReflectPass2.GetBindDescByName(m_bufferName);
        bDescDBufferPS = m_PSReflectPass2.GetBindDescByName(m_bufferName);

        m_slotsPass2[14] = (bDescDBufferVS) ? bDescDBufferVS.value().BindPoint : UINT_MAX;
        m_slotsPass2[15] = (bDescDBufferPS) ? bDescDBufferPS.value().BindPoint : UINT_MAX;

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