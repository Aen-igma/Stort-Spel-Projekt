#include "DepthStencil.h"

namespace Aen {

    const bool DepthStencil::Initialize(const Window& window) {

        ComTexture2D dsBuffer;
        D3D11_TEXTURE2D_DESC texDesc;
        ZeroMemory(&texDesc, sizeof(D3D11_TEXTURE2D_DESC));

        texDesc.Width = window.width;
        texDesc.Height = window.height;
        texDesc.MipLevels = 1;
        texDesc.ArraySize = 1;
        texDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        texDesc.SampleDesc.Count = 1;
        texDesc.SampleDesc.Quality = 0;
        texDesc.Usage = D3D11_USAGE_DEFAULT;
        texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
        texDesc.CPUAccessFlags = 0;
        texDesc.MiscFlags = 0;

        if(FAILED(device->CreateTexture2D(&texDesc, nullptr, &dsBuffer)))
            return false;

        if(FAILED(device->CreateDepthStencilView(dsBuffer.Get(), nullptr, &dsView)))
            return false;

        D3D11_DEPTH_STENCIL_DESC dsDesc;
        ZeroMemory(&dsDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));

        dsDesc.DepthEnable = true;
        dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

        if(FAILED(device->CreateDepthStencilState(&dsDesc, &dsState)))
            return false;

        return true;
    }
}