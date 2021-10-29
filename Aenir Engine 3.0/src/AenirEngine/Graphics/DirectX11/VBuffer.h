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

		const bool Create(T* data, const UINT& vCount) {

			m_bufferSize = vCount;
			m_stride = std::make_unique<UINT>(sizeof(T));

			D3D11_BUFFER_DESC bDesc;
			ZeroMemory(&bDesc, sizeof(D3D11_BUFFER_DESC));

			bDesc.Usage = D3D11_USAGE_IMMUTABLE;
			bDesc.ByteWidth = sizeof(T) * m_bufferSize;
			bDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bDesc.CPUAccessFlags = 0;
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

		private:
		ComBuffer m_buffer;
		std::unique_ptr<UINT> m_stride;
		UINT m_bufferSize;
	};
}