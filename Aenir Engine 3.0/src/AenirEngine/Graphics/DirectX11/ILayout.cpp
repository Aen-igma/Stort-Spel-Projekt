#include"PCH.h"
#include "ILayout.h"

namespace Aen {
    
    ILayout::ILayout()
        :m_layout(NULL), m_inputDesc() {}
    
    void ILayout::Create(VShader& shader) {

        if(FAILED(m_device->CreateInputLayout(
            m_inputDesc.data(), 
            (UINT)m_inputDesc.size(), 
            shader.m_byteCode->GetBufferPointer(), 
            shader.m_byteCode->GetBufferSize(), 
            &m_layout)))
            throw;
    }

    void ILayout::SetILayout() {
        m_dContext->IASetInputLayout(m_layout.Get());
    }
}