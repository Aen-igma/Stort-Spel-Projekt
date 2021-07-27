#include"PCH.h"
#include "Depth.h"

namespace Aen {

    Depth::~Depth() {
        m_dsView.Reset();
    }

    Depth::Depth()
        :m_dsView(NULL) {}

    Depth::Depth(const Window& window)
        :m_dsView(NULL){

        ComTexture2D dsBuffer;
        D3D11_TEXTURE2D_DESC texDesc;
        ZeroMemory(&texDesc, sizeof(D3D11_TEXTURE2D_DESC));

        texDesc.Width = (UINT)window.GetSize().x;
        texDesc.Height = (UINT)window.GetSize().y;
        texDesc.MipLevels = 1;
        texDesc.ArraySize = 1;
        texDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        texDesc.SampleDesc.Count = 1;
        texDesc.SampleDesc.Quality = 0;
        texDesc.Usage = D3D11_USAGE_DEFAULT;
        texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
        texDesc.CPUAccessFlags = 0;
        texDesc.MiscFlags = 0;

        if(FAILED(m_device->CreateTexture2D(&texDesc, NULL, dsBuffer.GetAddressOf())))
            throw;

        if(FAILED(m_device->CreateDepthStencilView(dsBuffer.Get(), NULL, m_dsView.GetAddressOf())))
            throw;

        dsBuffer.Reset();
    }

    void Depth::Create(const Window& window) {
        ComTexture2D dsBuffer;
        D3D11_TEXTURE2D_DESC texDesc;
        ZeroMemory(&texDesc, sizeof(D3D11_TEXTURE2D_DESC));

        texDesc.Width = (UINT)window.GetSize().x;
        texDesc.Height = (UINT)window.GetSize().y;
        texDesc.MipLevels = 1;
        texDesc.ArraySize = 1;
        texDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        texDesc.SampleDesc.Count = 1;
        texDesc.SampleDesc.Quality = 0;
        texDesc.Usage = D3D11_USAGE_DEFAULT;
        texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
        texDesc.CPUAccessFlags = 0;
        texDesc.MiscFlags = 0;

        if(FAILED(m_device->CreateTexture2D(&texDesc, NULL, dsBuffer.GetAddressOf())))
            throw;

        if(FAILED(m_device->CreateDepthStencilView(dsBuffer.Get(), NULL, m_dsView.GetAddressOf())))
            throw;

        dsBuffer.Reset();
    }

}