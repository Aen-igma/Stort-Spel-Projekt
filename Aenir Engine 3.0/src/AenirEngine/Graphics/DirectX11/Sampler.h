#pragma once
#include"Shader.h"

namespace Aen {

	enum class SamplerType : int {
		WRAP = 1,
		MIRROR = 2,
		CLAMP = 3,
		BORDER = 4,
		MIRROR_ONCE = 5,
		NONE = 6
	};

	class Sampler : public GCore {
		public:
		~Sampler();
		Sampler();
		Sampler(const SamplerType& type);

		void Create(const SamplerType& type);

		friend class RenderSystem;

		private:
		ComSamplerState m_sState;
	};
}