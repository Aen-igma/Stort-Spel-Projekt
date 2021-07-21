#include"PCH.h"
#include "BBuffer.h"

namespace Aen {

    BBuffer::~BBuffer() {
        m_rtv.Reset();
    }

    BBuffer::BBuffer()
        :m_rtv(NULL) {
        ComTexture2D backBuffer;
        if(FAILED(m_sChain->GetBuffer(0, __uuidof(ID3D11Resource), &backBuffer)))
            throw;

        if(FAILED(m_device->CreateRenderTargetView(backBuffer.Get(), nullptr, &m_rtv)))
            throw;
    }

    void BBuffer::SetRTV(DepthStencil& ds) {
        m_dContext->OMSetRenderTargets(1, &m_rtv, ds.m_dsView.Get());
    }
}