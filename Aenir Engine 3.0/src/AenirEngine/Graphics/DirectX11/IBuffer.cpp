#include "IBuffer.h"

namespace Aen {

    bool IBuffer::Initialize(DWORD* vId, UINT iCount) {

        iSize = iCount;

        D3D11_BUFFER_DESC iDesc;
        ZeroMemory(&iDesc, sizeof(D3D11_BUFFER_DESC));

        DWORD p[36];
        memcpy(p, vId, sizeof(DWORD) * 36);

        iDesc.Usage = D3D11_USAGE_IMMUTABLE;
        iDesc.ByteWidth = sizeof(DWORD) * iCount;
        iDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
        iDesc.CPUAccessFlags = 0;
        iDesc.MiscFlags = 0;
        iDesc.StructureByteStride = 0;

        D3D11_SUBRESOURCE_DATA iData;
        ZeroMemory(&iData, sizeof(D3D11_SUBRESOURCE_DATA));

        iData.pSysMem = vId;

        return SUCCEEDED(device->CreateBuffer(&iDesc, &iData, &buffer));
    }
}