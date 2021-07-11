#pragma once
#include"DX11Core.h"

namespace Aen {

	class GBuffer : public GCore {
		public:
		GBuffer();

		void Create(const uint32_t& count, Window& window);
		
		friend class RenderSystem;

		private:
		std::vector<ID3D11RenderTargetView*> m_rtvs;
		std::vector<ID3D11ShaderResourceView*> m_srvs;
	};
}