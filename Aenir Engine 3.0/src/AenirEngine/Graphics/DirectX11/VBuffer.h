#pragma once
#include"DX11Core.h"

namespace Aen {
	template<class T>
	struct VBuffer : public GCore {
		ComBuffer buffer;
		std::unique_ptr<UINT> stride;
		UINT bSize;

		VBuffer()
			:bSize(0) {
		}

		~VBuffer() = default;

		const bool Initialize(T* data, const UINT& vCount) {

			bSize = vCount;
			stride = std::make_unique<UINT>(sizeof(T));

			D3D11_BUFFER_DESC vDesc;
			ZeroMemory(&vDesc, sizeof(D3D11_BUFFER_DESC));

			vDesc.Usage = D3D11_USAGE_IMMUTABLE;
			vDesc.ByteWidth = sizeof(T) * vCount;
			vDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			vDesc.CPUAccessFlags = 0;
			vDesc.MiscFlags = 0;
			vDesc.StructureByteStride = 0;

			D3D11_SUBRESOURCE_DATA vData;
			ZeroMemory(&vData, sizeof(D3D11_SUBRESOURCE_DATA));

			vData.pSysMem = data;
			vData.SysMemPitch = 0;
			vData.SysMemSlicePitch = 0;

			return SUCCEEDED(device->CreateBuffer(&vDesc, &vData, &buffer));
		}
	};
}