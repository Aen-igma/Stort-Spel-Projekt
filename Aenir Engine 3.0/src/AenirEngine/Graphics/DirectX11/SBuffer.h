#pragma once
#include"Shader.h"
#include<vector>

namespace Aen {

	template<class T>
	class SBuffer : public GCore {
		public:

		SBuffer() = delete;
		SBuffer(const uint32_t& size)
			:m_srv(NULL), m_buffer(NULL), m_data(size), m_size(size) {

			D3D11_BUFFER_DESC sDesc;
			ZeroMemory(&sDesc, sizeof(D3D11_BUFFER_DESC));

			int mod = sizeof(T) % 16;
			UINT byteSize = (mod == 0) ? static_cast<UINT>(sizeof(T)) : static_cast<UINT>(sizeof(T) + (16 - mod));

			sDesc.Usage = D3D11_USAGE_DYNAMIC;
			sDesc.ByteWidth = byteSize * size;
			sDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
			sDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			sDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
			sDesc.StructureByteStride = byteSize;

			if(FAILED(m_device->CreateBuffer(&sDesc, NULL, &m_buffer)))
				return false;

			D3D11_SHADER_RESOURCE_VIEW_DESC desc;
			ZeroMemory(&desc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));

			desc.Format = DXGI_FORMAT_UNKNOWN;
			desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
			desc.Buffer.FirstElement = 0;
			desc.Buffer.NumElements = m_size;

			if(FAILED(m_device->CreateShaderResourceView(m_buffer.Get(), &desc, &m_srv)))
				throw;
		}

		void UpdateBuffer() {
			D3D11_MAPPED_SUBRESOURCE mResource;
			m_dContext->Map(m_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mResource);

			CopyMemory(mResource.pData, m_data.data(), sizeof(T) * m_size);
			m_dContext->Unmap(m_buffer.Get(), 0);
		}

		template<class T1>
		void SetSRV(const UINT& slot);

		private:
		ComShaderResourceView m_srv;
		ComBuffer m_buffer;
		std::vector<T> m_data;
		uint32_t m_size;
	};

	namespace Concealed1 {
	
		template<class T>
		inline void SSRV(ID3D11DeviceContext*& dc, ComShaderResourceView& srv, const UINT& s);

		template<>
		inline void SSRV<VShader>(ID3D11DeviceContext*& dc, ComShaderResourceView& srv, const UINT& s) {
			dc->VSSetShaderResources(s, 1, srv.GetAddressOf());
		}

		template<>
		inline void SSRV<HShader>(ID3D11DeviceContext*& dc, ComShaderResourceView& srv, const UINT& s) {
			dc->HSSetShaderResources(s, 1, srv.GetAddressOf());
		}

		template<>
		inline void SSRV<CShader>(ID3D11DeviceContext*& dc, ComShaderResourceView& srv, const UINT& s) {
			dc->CSSetShaderResources(s, 1, srv.GetAddressOf());
		}

		template<>
		inline void SSRV<DShader>(ID3D11DeviceContext*& dc, ComShaderResourceView& srv, const UINT& s) {
			dc->DSSetShaderResources(s, 1, srv.GetAddressOf());
		}

		template<>
		inline void SSRV<GShader>(ID3D11DeviceContext*& dc, ComShaderResourceView& srv, const UINT& s) {
			dc->GSSetShaderResources(s, 1, srv.GetAddressOf());
		}

		template<>
		inline void SSRV<PShader>(ID3D11DeviceContext*& dc, ComShaderResourceView& srv, const UINT& s) {
			dc->PSSetShaderResources(s, 1, srv.GetAddressOf());
		}
	}
	
	template<class T>
	template<class T1>
	inline void SBuffer<T>::SetSRV(const UINT& slot) {
		Concealed1::SSRV<T1>(m_dContext, m_srv, slot);
	}

}