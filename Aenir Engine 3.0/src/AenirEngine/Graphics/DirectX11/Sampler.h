#pragma once
#include"Shader.h"

namespace Aen {

	enum class SamplerType : int {
		WRAP = 1,
		MIRROR = 2,
		CLAMP = 3,
		BORDER = 4,
		MIRROR_ONCE = 5
	};

	class Sampler : public GCore {
		public:
		Sampler() = delete;
		Sampler(const SamplerType& adress);

		template<class T>
		void BindSampler(const UINT& slot);

		friend class RenderSystem;

		private:
		ComSamplerState m_sState;
	};

	template<>
	inline void Sampler::BindSampler<VShader>(const UINT& slot) {
		m_dContext->VSSetSamplers(slot, 1, m_sState.GetAddressOf());
	}

	template<>
	inline void Sampler::BindSampler<HShader>(const UINT& slot) {
		m_dContext->HSSetSamplers(slot, 1, m_sState.GetAddressOf());
	}

	template<>
	inline void Sampler::BindSampler<CShader>(const UINT& slot) {
		m_dContext->CSSetSamplers(slot, 1, m_sState.GetAddressOf());
	}

	template<>
	inline void Sampler::BindSampler<DShader>(const UINT& slot) {
		m_dContext->DSSetSamplers(slot, 1, m_sState.GetAddressOf());
	}

	template<>
	inline void Sampler::BindSampler<GShader>(const UINT& slot) {
		m_dContext->GSSetSamplers(slot, 1, m_sState.GetAddressOf());
	}

	template<>
	inline void Sampler::BindSampler<PShader>(const UINT& slot) {
		m_dContext->PSSetSamplers(slot, 1, m_sState.GetAddressOf());
	}
}