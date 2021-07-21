#include"PCH.h"
#include "DepthStencil.h"

namespace Aen {

    DepthStencil::~DepthStencil() {
        m_dsView.Reset();
        m_dsState.Reset();
    }

    DepthStencil::DepthStencil(const Window& window)
        :m_dsView(NULL), m_dsState(NULL) {
        ComTexture2D dsBuffer;
        D3D11_TEXTURE2D_DESC texDesc;
        ZeroMemory(&texDesc, sizeof(D3D11_TEXTURE2D_DESC));

        texDesc.Width = window.GetSize().x;
        texDesc.Height = window.GetSize().y;
        texDesc.MipLevels = 1;
        texDesc.ArraySize = 1;
        texDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        texDesc.SampleDesc.Count = 1;
        texDesc.SampleDesc.Quality = 0;
        texDesc.Usage = D3D11_USAGE_DEFAULT;
        texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
        texDesc.CPUAccessFlags = 0;
        texDesc.MiscFlags = 0;

        if(FAILED(m_device->CreateTexture2D(&texDesc, nullptr, &dsBuffer)))
            throw;

        if(FAILED(m_device->CreateDepthStencilView(dsBuffer.Get(), nullptr, &m_dsView)))
            throw;

        D3D11_DEPTH_STENCIL_DESC dsDesc;
        ZeroMemory(&dsDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));

        dsDesc.DepthEnable = true;
        dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

        if(FAILED(m_device->CreateDepthStencilState(&dsDesc, &m_dsState)))
            throw;
    }

    void DepthStencil::Create(const Window& window) {
        ComTexture2D dsBuffer;
        D3D11_TEXTURE2D_DESC texDesc;
        ZeroMemory(&texDesc, sizeof(D3D11_TEXTURE2D_DESC));

        texDesc.Width = window.GetSize().x;
        texDesc.Height = window.GetSize().y;
        texDesc.MipLevels = 1;
        texDesc.ArraySize = 1;
        texDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        texDesc.SampleDesc.Count = 1;
        texDesc.SampleDesc.Quality = 0;
        texDesc.Usage = D3D11_USAGE_DEFAULT;
        texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
        texDesc.CPUAccessFlags = 0;
        texDesc.MiscFlags = 0;

        if(FAILED(m_device->CreateTexture2D(&texDesc, nullptr, &dsBuffer)))
            throw;

        if(FAILED(m_device->CreateDepthStencilView(dsBuffer.Get(), nullptr, &m_dsView)))
            throw;

        D3D11_DEPTH_STENCIL_DESC dsDesc;
        ZeroMemory(&dsDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));

        dsDesc.DepthEnable = true;
        dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

        if(FAILED(m_device->CreateDepthStencilState(&dsDesc, &m_dsState)))
            throw;
    }

}