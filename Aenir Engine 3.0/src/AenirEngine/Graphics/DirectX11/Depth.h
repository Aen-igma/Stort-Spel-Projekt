#pragma once
#include"DX11Core.h"

namespace Aen {

	class Depth : public GCore {
		public:
		~Depth();
		Depth();
		Depth(const Window& window);

		void Create(const Window& window);
		friend class RenderSystem;

		private:
		ComDepthStencilView m_dsView;
	};
}
