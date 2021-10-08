#pragma once
#include"DX11Core.h"

namespace Aen {

	class BBuffer : public GCore {
		public:
		~BBuffer();
		BBuffer();

		friend class RenderSystem;
		friend class UAView;

		private:
		ComRenderTargetView m_rtv;
		ComTexture2D m_backBuffer;
	};
}
