#pragma once
#include"DX11Core.h"

namespace Aen {

	class RWTexture2D : public GCore {
		public:
		~RWTexture2D();
		RWTexture2D();

		void Create(const UINT& width, const UINT& height, const DXGI_FORMAT& format);
		void Create(const Vec2i& size, const DXGI_FORMAT& format);

		friend class RenderSystem;

		private:
		ComUnorderedAccessView m_uav;
		ComShaderResourceView m_srv;
	};
}