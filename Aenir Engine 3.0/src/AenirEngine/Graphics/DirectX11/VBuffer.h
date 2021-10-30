#pragma once
#include"DX11Core.h"

namespace Aen {

	template<class T>
	class VBuffer : public GCore {
		public:

		~VBuffer() {
			m_buffer.Reset();
		}

		VBuffer()
			:m_buffer(NULL), m_stride(nullptr), m_bufferSize(0) {}

		const bool Create(T* data, const UINT& vCount, const D3D11_USAGE& usage = D3D11_USAGE_IMMUTABLE) {

			m_bufferSize = vCount;
			m_stride = std::make_unique<UINT>(sizeof(T));

			D3D11_BUFFER_DESC bDesc;
			ZeroMemory(&bDesc, sizeof(D3D11_BUFFER_DESC));

			bDesc.Usage = usage;
			bDesc.ByteWidth = sizeof(T) * m_bufferSize;
			bDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

			if(usage == D3D11_USAGE_IMMUTABLE) bDesc.CPUAccessFlags = 0;
			else if(usage == D3D11_USAGE_DYNAMIC) bDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

			bDesc.MiscFlags = 0;
			bDesc.StructureByteStride = 0;

			D3D11_SUBRESOURCE_DATA srData;
			ZeroMemory(&srData, sizeof(D3D11_SUBRESOURCE_DATA));

			srData.pSysMem = data;
			srData.SysMemPitch = 0;
			srData.SysMemSlicePitch = 0;

			if (FAILED(m_device->CreateBuffer(&bDesc, &srData, &m_buffer))) return false;
			return true;
		}

		void BindBuffer() {
			UINT offset = 0;
			m_dContext->IASetVertexBuffers(0, 1, m_buffer.GetAddressOf(), m_stride.get(), &offset);
		}

		void Draw() {
			m_dContext->Draw(m_bufferSize, 0);
		}
		
		void Draw(const UINT& count, const UINT& offset) {
			m_dContext->Draw(count, offset);
		}

		const UINT GetVertCount() const
		{
			return m_bufferSize;
		}

		const UINT GetStride() const
		{
			return *m_stride.get();
		}

		void UpdateBuffer(T* data, const UINT& size) {
			D3D11_MAPPED_SUBRESOURCE mResource;
			m_dContext->Map(m_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mResource);

			CopyMemory(mResource.pData, data, sizeof(T) * size);
			m_dContext->Unmap(m_buffer.Get(), 0);
		}

		private:
		ComBuffer m_buffer;
		std::unique_ptr<UINT> m_stride;
		UINT m_bufferSize;
	};
}