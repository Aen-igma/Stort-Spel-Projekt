#include "BackBufferTarget.h"

namespace Aen {

    const bool BackBufferTarget::Initialize() {

        ComTexture2D backBuffer;
        if(FAILED(sChain->GetBuffer(0, __uuidof(ID3D11Resource), &backBuffer)))
            return false;

        return SUCCEEDED(device->CreateRenderTargetView(backBuffer.Get(), nullptr, &rtv));
    }
}