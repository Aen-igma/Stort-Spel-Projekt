#include "ILayout.h"

namespace Aen {

    const bool ILayout::Initialize(const VShader& shader) {
        return SUCCEEDED(device->CreateInputLayout(inputDesc.data(), (UINT)inputDesc.size(), shader.byteCode->GetBufferPointer(), shader.byteCode->GetBufferSize(), &layout));
    }
}