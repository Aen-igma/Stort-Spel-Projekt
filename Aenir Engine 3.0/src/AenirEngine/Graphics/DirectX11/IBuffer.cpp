#include"PCH.h"
#include "IBuffer.h"

namespace Aen {

    IBuffer::~IBuffer() {
        m_buffer.Reset();
    }

    IBuffer::IBuffer()
        :m_buffer(NULL), m_bufferSize(0) {}

    bool IBuffer::Create(DWORD* vId, const UINT& bufferSize) {

        m_bufferSize = bufferSize;

        D3D11_BUFFER_DESC bDesc;
        ZeroMemory(&bDesc, sizeof(D3D11_BUFFER_DESC));

        bDesc.Usage = D3D11_USAGE_DEFAULT;
        bDesc.ByteWidth = /*sizeof(DWORD) **/ m_bufferSize;
        bDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
        bDesc.CPUAccessFlags = 0;
        bDesc.MiscFlags = 0;
        bDesc.StructureByteStride = 0;

        D3D11_SUBRESOURCE_DATA srData;
        ZeroMemory(&srData, sizeof(D3D11_SUBRESOURCE_DATA));

        srData.pSysMem = vId;

        if (FAILED(m_device->CreateBuffer(&bDesc, &srData, m_buffer.GetAddressOf()))){
            throw;
            return false;
        }
        return true;
    }

    void IBuffer::BindBuffer() {
        m_dContext->IASetIndexBuffer(m_buffer.Get(), DXGI_FORMAT_R32_UINT, 0);
    }

    void IBuffer::DrawIndexed() {
        m_dContext->DrawIndexed(m_bufferSize, 0, 0);
    }

    const UINT IBuffer::GetBufferSize() {
        return m_bufferSize;
    }
}