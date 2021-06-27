#pragma once
#include"DX11Core.h"

namespace Aen {
	struct Rasterizer : public GCore {
		ComRasterizerState rState;

		~Rasterizer() = default;

		const bool Initialize(const D3D11_FILL_MODE& fill, const D3D11_CULL_MODE& cull);
	};
}