#pragma once
#include"Shader.h"

namespace Aen {

	template<class T>
	class CBuffer : public GCore {
		public:

		CBuffer()
			:m_buffer(NULL), m_data() {

			D3D11_BUFFER_DESC bDesc;
			ZeroMemory(&bDesc, sizeof(D3D11_BUFFER_DESC));

			int mod = sizeof(T) % 16;
			UINT size = (mod == 0) ? static_cast<UINT>(sizeof(T)) : static_cast<UINT>(sizeof(T) + (16 - mod));

			bDesc.Usage = D3D11_USAGE_DYNAMIC;
			bDesc.ByteWidth = size;
			bDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			bDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			bDesc.MiscFlags = 0;
			bDesc.StructureByteStride = 0;

			if(FAILED(m_device->CreateBuffer(&bDesc, nullptr, &m_buffer)))
				throw;
		}

		void UpdateBuffer() {
			D3D11_MAPPED_SUBRESOURCE mResource;
			m_dContext->Map(m_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mResource);

			CopyMemory(mResource.pData, &m_data, sizeof(T));
			m_dContext->Unmap(m_buffer.Get(), 0);
		}

		template<class T1>
		void SetBuffer(const UINT& slot);

		private:
		ComBuffer m_buffer;
		T m_data;
	};
	
	namespace Concealed1 {
		template<class T>
		inline void SB(ID3D11DeviceContext*& dc, ComBuffer& b, const UINT& s);

		template<>
		inline void SB<VShader>(ID3D11DeviceContext*& dc, ComBuffer& b, const UINT& s) {
			dc->VSSetConstantBuffers(s, 1, b.GetAddressOf());
		}

		template<>
		inline void SB<HShader>(ID3D11DeviceContext*& dc, ComBuffer& b, const UINT& s) {
			dc->HSSetConstantBuffers(s, 1, b.GetAddressOf());
		}

		template<>
		inline void SB<CShader>(ID3D11DeviceContext*& dc, ComBuffer& b, const UINT& s) {
			dc->CSSetConstantBuffers(s, 1, b.GetAddressOf());
		}

		template<>
		inline void SB<DShader>(ID3D11DeviceContext*& dc, ComBuffer& b, const UINT& s) {
			dc->DSSetConstantBuffers(s, 1, b.GetAddressOf());
		}

		template<>
		inline void SB<GShader>(ID3D11DeviceContext*& dc, ComBuffer& b, const UINT& s) {
			dc->GSSetConstantBuffers(s, 1, b.GetAddressOf());
		}

		template<>
		inline void SB<PShader>(ID3D11DeviceContext*& dc, ComBuffer& b, const UINT& s) {
			dc->PSSetConstantBuffers(s, 1, b.GetAddressOf());
		}
	}

	template<class T>
	template<class T1>
	inline void CBuffer<T>::SetBuffer(const UINT& slot) {
		Concealed1::SB<T1>(m_dContext, m_buffer, slot);
	}

}