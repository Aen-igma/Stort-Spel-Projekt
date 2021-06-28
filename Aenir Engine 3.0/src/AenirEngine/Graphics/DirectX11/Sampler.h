#pragma once
#include"Shader.h"

namespace Aen {

	enum class SType : int {
		WRAP = 1,
		MIRROR = 2,
		CLAMP = 3,
		BORDER = 4,
		MIRROR_ONCE = 5
	};

	class Sampler : public GCore {
		public:
		Sampler() = delete;
		Sampler(const SType& adress);

		template<class T>
		void SetSampler(const UINT& slot);

		private:
		ComSamplerState m_sState;
	};

	template<>
	inline void Sampler::SetSampler<VShader>(const UINT& slot) {
		m_dContext->VSSetSamplers(slot, 1, m_sState.GetAddressOf());
	}

	template<>
	inline void Sampler::SetSampler<HShader>(const UINT& slot) {
		m_dContext->HSSetSamplers(slot, 1, m_sState.GetAddressOf());
	}

	template<>
	inline void Sampler::SetSampler<CShader>(const UINT& slot) {
		m_dContext->CSSetSamplers(slot, 1, m_sState.GetAddressOf());
	}

	template<>
	inline void Sampler::SetSampler<DShader>(const UINT& slot) {
		m_dContext->DSSetSamplers(slot, 1, m_sState.GetAddressOf());
	}

	template<>
	inline void Sampler::SetSampler<GShader>(const UINT& slot) {
		m_dContext->GSSetSamplers(slot, 1, m_sState.GetAddressOf());
	}

	template<>
	inline void Sampler::SetSampler<PShader>(const UINT& slot) {
		m_dContext->PSSetSamplers(slot, 1, m_sState.GetAddressOf());
	}
}