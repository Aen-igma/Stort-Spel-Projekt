#pragma once
#include"DX11Core.h"

namespace Aen {

	class BBuffer : public GCore {
		public:
		~BBuffer();
		BBuffer();

		friend class RenderSystem;

		private:
		ComRenderTargetView m_rtv;
	};
}
