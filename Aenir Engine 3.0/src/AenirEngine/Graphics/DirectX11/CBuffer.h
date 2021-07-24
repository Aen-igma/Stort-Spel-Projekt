#pragma once
#include"Shader.h"

namespace Aen {

	template<class T>
	class CBuffer : public GCore {
		public:

		~CBuffer() {
			m_buffer.Reset();
		}

		CBuffer()
			:m_buffer(NULL), m_data() {

			D3D11_BUFFER_DESC bDesc;
			ZeroMemory(&bDesc, sizeof(D3D11_BUFFER_DESC));

			UINT mod = sizeof(m_data) % 16u;
			UINT size = (mod == 0) ? static_cast<UINT>(sizeof(m_data)) : static_cast<UINT>(sizeof(m_data) + (16u - mod));

			bDesc.Usage = D3D11_USAGE_DYNAMIC;
			bDesc.ByteWidth = size;
			bDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			bDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			bDesc.MiscFlags = 0;
			bDesc.StructureByteStride = 0;

			if(FAILED(m_device->CreateBuffer(&bDesc, nullptr, &m_buffer)))
				throw;
		}

		T& GetData() {
			return m_data;
		}

		void UpdateBuffer() {
			D3D11_MAPPED_SUBRESOURCE mResource;
			m_dContext->Map(m_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mResource);

			CopyMemory(mResource.pData, &m_data, sizeof(m_data));
			m_dContext->Unmap(m_buffer.Get(), 0);
		}

		template<class T1>
		void BindBuffer(const UINT& slot);

		private:
		ComBuffer m_buffer;
		T m_data;
	};
	
	namespace Concealed1 {
		template<class T>
		inline void BB(const ComDeviceContext& dc, ComBuffer& b, const UINT& s);

		template<>
		inline void BB<VShader>(const ComDeviceContext& dc, ComBuffer& b, const UINT& s) {
			dc->VSSetConstantBuffers(s, 1, b.GetAddressOf());
		}

		template<>
		inline void BB<HShader>(const ComDeviceContext& dc, ComBuffer& b, const UINT& s) {
			dc->HSSetConstantBuffers(s, 1, b.GetAddressOf());
		}

		template<>
		inline void BB<CShader>(const ComDeviceContext& dc, ComBuffer& b, const UINT& s) {
			dc->CSSetConstantBuffers(s, 1, b.GetAddressOf());
		}

		template<>
		inline void BB<DShader>(const ComDeviceContext& dc, ComBuffer& b, const UINT& s) {
			dc->DSSetConstantBuffers(s, 1, b.GetAddressOf());
		}

		template<>
		inline void BB<GShader>(const ComDeviceContext& dc, ComBuffer& b, const UINT& s) {
			dc->GSSetConstantBuffers(s, 1, b.GetAddressOf());
		}

		template<>
		inline void BB<PShader>(const ComDeviceContext& dc, ComBuffer& b, const UINT& s) {
			dc->PSSetConstantBuffers(s, 1, b.GetAddressOf());
		}
	}

	template<class T>
	template<class T1>
	inline void CBuffer<T>::BindBuffer(const UINT& slot) {
		Concealed1::BB<T1>(m_dContext, m_buffer, slot);
	}
}