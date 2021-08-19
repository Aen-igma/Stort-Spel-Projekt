#include"PCH.h"
#include "ILayout.h"

namespace Aen {

    ILayout::~ILayout() {
        m_layout.Reset();
    }

    ILayout::ILayout()
        :m_layout(NULL), m_inputDesc() {}
    
    void ILayout::Create(VShader& shader) {

        Reflection reflection(shader);

        m_inputDesc.reserve(reflection.GetShaderDesc().InputParameters);
        m_inputDesc.resize(reflection.GetShaderDesc().InputParameters);
        for(uint32_t i = 0; i < reflection.GetShaderDesc().InputParameters; i++) {
            D3D11_SIGNATURE_PARAMETER_DESC sDesc = reflection.GetInputDesc()[i];
            m_inputDesc[i] = {sDesc.SemanticName, sDesc.SemanticIndex, GetFormat(sDesc.ComponentType, sDesc.Mask), 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0};
        }
        
        if(FAILED(m_device->CreateInputLayout(
            m_inputDesc.data(), 
            (UINT)m_inputDesc.size(), 
            shader.m_byteCode->GetBufferPointer(), 
            shader.m_byteCode->GetBufferSize(), 
            &m_layout)))
            throw;
    }

    const DXGI_FORMAT ILayout::GetFormat(const D3D_REGISTER_COMPONENT_TYPE& type, const BYTE& mask) {
        if(mask == 1) {
            switch(type) {
                case D3D_REGISTER_COMPONENT_UINT32: return DXGI_FORMAT_R32_UINT;
                case D3D_REGISTER_COMPONENT_SINT32: return DXGI_FORMAT_R32_SINT;
                case D3D_REGISTER_COMPONENT_FLOAT32: return DXGI_FORMAT_R32_FLOAT;
            }
        } else if(mask <= 3) {
            switch(type) {
                case D3D_REGISTER_COMPONENT_UINT32: return DXGI_FORMAT_R32G32_UINT;
                case D3D_REGISTER_COMPONENT_SINT32: return DXGI_FORMAT_R32G32_SINT;
                case D3D_REGISTER_COMPONENT_FLOAT32: return DXGI_FORMAT_R32G32_FLOAT;
            }
        } else if (mask <= 7) {
            switch(type) {
                case D3D_REGISTER_COMPONENT_UINT32: return DXGI_FORMAT_R32G32B32_UINT;
                case D3D_REGISTER_COMPONENT_SINT32: return DXGI_FORMAT_R32G32B32_SINT;
                case D3D_REGISTER_COMPONENT_FLOAT32: return DXGI_FORMAT_R32G32B32_FLOAT;
            }
        } else if (mask <= 15) {
            switch(type) {
                case D3D_REGISTER_COMPONENT_UINT32: return DXGI_FORMAT_R32G32B32A32_UINT;
                case D3D_REGISTER_COMPONENT_SINT32: return DXGI_FORMAT_R32G32B32A32_SINT;
                case D3D_REGISTER_COMPONENT_FLOAT32: return DXGI_FORMAT_R32G32B32A32_FLOAT;
            }
        }
    }
}