#pragma once
#include"DepthStencil.h"

namespace Aen {

	class BBuffer : public GCore {
		public:
		~BBuffer();
		BBuffer();

		void SetRTV(DepthStencil& ds);

		friend class RenderSystem;

		private:
		ComRenderTargetView m_rtv;
	};
}
