#pragma once
#include"DX11Core.h"
#include<vector>

namespace Aen {
	template<class T>
	struct SBuffer : public GCore {
		ComShaderResourceView srv;
		ComBuffer buffer;
		uint32_t count;
		std::vector<T> data;

		SBuffer() = delete;
		SBuffer(const uint32_t& count)
			:count(count) {
			data.resize(count, T());
		}

		~SBuffer() = default;

		const bool Initialize() {

			D3D11_BUFFER_DESC sDesc;
			ZeroMemory(&sDesc, sizeof(D3D11_BUFFER_DESC));
			
			int mod = sizeof(T) % 16;
			UINT size = (mod == 0) ? static_cast<UINT>(sizeof(T)) : static_cast<UINT>(sizeof(T) + (16 - mod));

			sDesc.Usage = D3D11_USAGE_DYNAMIC;
			sDesc.ByteWidth = size * count;
			sDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
			sDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			sDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
			sDesc.StructureByteStride = size;

			if(FAILED(device->CreateBuffer(&sDesc, NULL, &buffer)))
				return false;

			D3D11_SHADER_RESOURCE_VIEW_DESC desc;
			ZeroMemory(&desc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));

			desc.Format = DXGI_FORMAT_UNKNOWN;
			desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
			desc.Buffer.FirstElement = 0;
			desc.Buffer.NumElements = count;

			return SUCCEEDED(device->CreateShaderResourceView(buffer.Get(), &desc, &srv));
		}

		void UpdateBuffer() {
			D3D11_MAPPED_SUBRESOURCE mResource;
			dContext->Map(buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mResource);

			CopyMemory(mResource.pData, data.data(), sizeof(T) * count);
			dContext->Unmap(buffer.Get(), 0);
		}
	};
}