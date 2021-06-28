#include "RState.h"

namespace Aen {

	RState::RState(const D3D11_FILL_MODE& fill, const D3D11_CULL_MODE& cull)
		:m_rState(NULL) {

		D3D11_RASTERIZER_DESC rDesc;
		ZeroMemory(&rDesc, sizeof(D3D11_RASTERIZER_DESC));

		rDesc.FillMode = fill;
		rDesc.CullMode = cull;
		rDesc.AntialiasedLineEnable = FALSE;
		rDesc.MultisampleEnable = FALSE;

		if(FAILED(m_device->CreateRasterizerState(&rDesc, &m_rState)))
			throw;
	}

	void RState::SetRState() {
		m_dContext->RSSetState(m_rState.Get());
	}
}