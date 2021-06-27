#include "Sampler.h"

namespace Aen {

	const bool Sampler::Initialize(const D3D11_TEXTURE_ADDRESS_MODE& adress) {

		D3D11_SAMPLER_DESC sDesc;
		ZeroMemory(&sDesc, sizeof(D3D11_SAMPLER_DESC));

		sDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		sDesc.AddressU = adress;
		sDesc.AddressV = adress;
		sDesc.AddressW = adress;
		sDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;

		for(int i = 0; i < 4; i++)
			sDesc.BorderColor[i] = 1.f;

		sDesc.MinLOD = 0;
		sDesc.MaxLOD = D3D11_FLOAT32_MAX;

		return SUCCEEDED(device->CreateSamplerState(&sDesc, &sState));
	}
}