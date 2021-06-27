#pragma once
#include"DX11Core.h"

namespace Aen {
	struct Sampler : public GCore {
		ComSamplerState sState;

		~Sampler() = default;

		const bool Initialize(const D3D11_TEXTURE_ADDRESS_MODE& adress);
	};
}