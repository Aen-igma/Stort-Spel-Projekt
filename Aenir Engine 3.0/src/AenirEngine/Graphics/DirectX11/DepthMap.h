#pragma once
#include"DX11Core.h"

namespace Aen {
	struct DepthMap : public GCore {
		ComDepthStencilView dsView;
		ComShaderResourceView srv;
		D3D11_VIEWPORT viewPort;

		~DepthMap() = default;

		const bool Initialize(const UINT& dimensions);
	};
}