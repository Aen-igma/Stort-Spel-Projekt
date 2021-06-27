#pragma once
#include"DX11Core.h"

namespace Aen {
	template<class T>
	struct CBuffer : public GCore {
		ComBuffer buffer;
		T data;

		CBuffer()
			: data() {
		}

		~CBuffer() = default;

		const bool Initialize() {

			D3D11_BUFFER_DESC cDesc;
			ZeroMemory(&cDesc, sizeof(D3D11_BUFFER_DESC));

			int mod = sizeof(T) % 16;
			UINT size = (mod == 0) ? static_cast<UINT>(sizeof(T)) : static_cast<UINT>(sizeof(T) + (16 - mod));

			cDesc.Usage = D3D11_USAGE_DYNAMIC;
			cDesc.ByteWidth = size;
			cDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			cDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			cDesc.MiscFlags = 0;
			cDesc.StructureByteStride = 0;

			return SUCCEEDED(device->CreateBuffer(&cDesc, nullptr, &buffer));
		}

		void UpdateBuffer() {
			D3D11_MAPPED_SUBRESOURCE mResource;
			dContext->Map(buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mResource);

			CopyMemory(mResource.pData, &data, sizeof(T));
			dContext->Unmap(buffer.Get(), 0);
		}
	};
}