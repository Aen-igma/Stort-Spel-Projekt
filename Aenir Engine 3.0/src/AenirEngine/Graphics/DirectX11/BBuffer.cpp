#include"PCH.h"
#include "BBuffer.h"

namespace Aen {

    BBuffer::~BBuffer() {
        m_rtv.Reset();
        m_backBuffer.Reset();
    }

    BBuffer::BBuffer()
        :m_rtv(NULL) {
        if(FAILED(m_sChain->GetBuffer(0, IID_ID3D11Resource, reinterpret_cast<void**>(m_backBuffer.GetAddressOf()))))
            throw;

        if(FAILED(m_device->CreateRenderTargetView(m_backBuffer.Get(), nullptr, m_rtv.GetAddressOf())))
            throw;
    }
}