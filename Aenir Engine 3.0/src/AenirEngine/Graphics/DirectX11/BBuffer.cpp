#include"PCH.h"
#include "BBuffer.h"

namespace Aen {

    BBuffer::~BBuffer() {
        m_rtv.Reset();
    }

    BBuffer::BBuffer()
        :m_rtv(NULL) {
        ComTexture2D backBuffer;
        if(FAILED(m_sChain->GetBuffer(0, __uuidof(ID3D11Resource), reinterpret_cast<void**>(backBuffer.GetAddressOf()))))
            throw;

        if(FAILED(m_device->CreateRenderTargetView(backBuffer.Get(), nullptr, m_rtv.GetAddressOf())))
            throw;

        backBuffer.Reset();
    }
}