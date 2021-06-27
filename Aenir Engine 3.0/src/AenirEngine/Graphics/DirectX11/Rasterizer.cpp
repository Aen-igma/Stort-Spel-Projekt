#include "Rasterizer.h"

namespace Aen {

	const bool Rasterizer::Initialize(const D3D11_FILL_MODE& fill, const D3D11_CULL_MODE& cull) {
		D3D11_RASTERIZER_DESC rDesc;
		ZeroMemory(&rDesc, sizeof(D3D11_RASTERIZER_DESC));

		rDesc.FillMode = fill;
		rDesc.CullMode = cull;
		rDesc.AntialiasedLineEnable = FALSE;
		rDesc.MultisampleEnable = FALSE;

		return SUCCEEDED(device->CreateRasterizerState(&rDesc, &rState));
	}
}