#pragma once
#include"DX11Core.h"

namespace Aen {

	class GBuffer : public GCore {
		public:
		~GBuffer();
		GBuffer() = default;
		GBuffer(const uint32_t& count, Window& window);

		void Create(const uint32_t& count, Window& window);
		const uint32_t GetCount();

		friend class RenderSystem;

		private:
		std::vector<ID3D11RenderTargetView*> m_rtvs;
		std::vector<ID3D11ShaderResourceView*> m_srvs;
	};
}