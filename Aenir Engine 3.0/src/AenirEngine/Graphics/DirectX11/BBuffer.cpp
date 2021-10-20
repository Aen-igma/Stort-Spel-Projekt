#include"PCH.h"
#include "BBuffer.h"

namespace Aen {

    BBuffer::~BBuffer() {
        m_rtv.Reset();
        m_bbTexture.Reset();
    }

    BBuffer::BBuffer()
        :m_rtv(NULL), m_bbTexture(NULL) {

        if(FAILED(m_sChain->GetBuffer(0, IID_ID3D11Resource, reinterpret_cast<void**>(m_bbTexture.GetAddressOf()))))
            throw;

        if(FAILED(m_device->CreateRenderTargetView(m_bbTexture.Get(), nullptr, m_rtv.GetAddressOf())))
            throw;
    }
}