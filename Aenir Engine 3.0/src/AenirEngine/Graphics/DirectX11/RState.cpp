#include"PCH.h"
#include "RState.h"

namespace Aen {

	RState::~RState() {
		m_rState.Reset();
	}

	RState::RState(const FillMode& fill, const CullMode& cull)
		:m_rState(NULL) {

		D3D11_RASTERIZER_DESC rDesc;
		ZeroMemory(&rDesc, sizeof(D3D11_RASTERIZER_DESC));

		rDesc.AntialiasedLineEnable = false;
		rDesc.CullMode = static_cast<D3D11_CULL_MODE>(cull);
		rDesc.DepthBias = 0;
		rDesc.DepthBiasClamp = 0.f;
		rDesc.DepthClipEnable = true;
		rDesc.FillMode = static_cast<D3D11_FILL_MODE>(fill);
		rDesc.FrontCounterClockwise = false;
		rDesc.MultisampleEnable = false;
		rDesc.ScissorEnable = false;
		rDesc.SlopeScaledDepthBias = 0.f;

		if(FAILED(m_device->CreateRasterizerState(&rDesc, m_rState.GetAddressOf())))
			throw;
	}
}