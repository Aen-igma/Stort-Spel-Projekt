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

        if(FAILED(m_device->CreateTexture2D(&texDesc, nullptr, dsBuffer.GetAddressOf())))
            throw;

        D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
        ZeroMemory(&dsvDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));

        dsvDesc.Format = texDesc.Format;
        dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
        dsvDesc.Texture2D.MipSlice = 0;

        if(FAILED(m_device->CreateDepthStencilView(dsBuffer.Get(), &dsvDesc, m_dsView.GetAddressOf())))
            throw;

        D3D11_DEPTH_STENCIL_DESC dsDesc;
        ZeroMemory(&dsDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));

        dsDesc.DepthEnable = true;
        dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

        dsDesc.StencilEnable = true;
        dsDesc.StencilReadMask = 0xFF;
        dsDesc.StencilWriteMask = 0xFF;

        dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
        dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
        dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
        dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

        dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
        dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
        dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
        dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

        if(FAILED(m_device->CreateDepthStencilState(&dsDesc, m_dsState.GetAddressOf())))
            throw;

        dsBuffer.Reset();
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

        if(FAILED(m_device->CreateTexture2D(&texDesc, nullptr, dsBuffer.GetAddressOf())))
            throw;

        D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
        ZeroMemory(&dsvDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));

        dsvDesc.Format = texDesc.Format;
        dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
        dsvDesc.Texture2D.MipSlice = 0;

        if(FAILED(m_device->CreateDepthStencilView(dsBuffer.Get(), &dsvDesc, m_dsView.GetAddressOf())))
            throw;

        D3D11_DEPTH_STENCIL_DESC dsDesc;
        ZeroMemory(&dsDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));

        dsDesc.DepthEnable = true;
        dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

        dsDesc.StencilEnable = true;
        dsDesc.StencilReadMask = 0xFF;
        dsDesc.StencilWriteMask = 0xFF;

        dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
        dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
        dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
        dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

        dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
        dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
        dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
        dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

        if(FAILED(m_device->CreateDepthStencilState(&dsDesc, m_dsState.GetAddressOf())))
            throw;

        dsBuffer.Reset();
    }

}