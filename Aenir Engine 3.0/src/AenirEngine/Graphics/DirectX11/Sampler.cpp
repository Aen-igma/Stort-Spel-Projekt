#include"PCH.h"
#include "Sampler.h"

namespace Aen {

	Sampler::Sampler(const SType& adress)
		:m_sState(NULL) {
		D3D11_SAMPLER_DESC sDesc;
		ZeroMemory(&sDesc, sizeof(D3D11_SAMPLER_DESC));

		sDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		sDesc.AddressU = static_cast<D3D11_TEXTURE_ADDRESS_MODE>(adress);
		sDesc.AddressV = static_cast<D3D11_TEXTURE_ADDRESS_MODE>(adress);
		sDesc.AddressW = static_cast<D3D11_TEXTURE_ADDRESS_MODE>(adress);
		sDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;

		for(int i = 0; i < 4; i++)
			sDesc.BorderColor[i] = 1.f;

		sDesc.MinLOD = 0;
		sDesc.MaxLOD = D3D11_FLOAT32_MAX;

		if(FAILED(m_device->CreateSamplerState(&sDesc, m_sState.GetAddressOf())))
			throw;
	}
}