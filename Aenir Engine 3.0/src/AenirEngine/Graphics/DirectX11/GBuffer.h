#pragma once
#include"DX11Core.h"

namespace Aen {

	class GBuffer : public GCore {
		public:
		~GBuffer();
		GBuffer() = default;
		GBuffer(Window& window, const uint32_t& count = 1u);

		void Create(Window& window, const uint32_t& count = 1u);
		const uint32_t GetCount();

		ComRenderTargetView GetRtv(const uint32_t& index);
		ComShaderResourceView GetSrv(const uint32_t& index);

		friend class RenderSystem;
		friend class GBufferTextures;

		private:
		std::vector<ID3D11RenderTargetView*> m_rtvs;
		std::vector<ID3D11ShaderResourceView*> m_srvs;
		std::vector<ComTexture2D> m_texture;
	};
	class GBufferTextures : public GCore {
	public:
		~GBufferTextures();
		GBufferTextures() = default;

		void CreateFromGBuffer(GBuffer& gbuffer, Window window);
		void c();

		ComShaderResourceView GetSrv(const uint32_t& index);

		friend class RenderSystem;

	private:
		std::vector<ID3D11ShaderResourceView*> m_srvs;
	};
}