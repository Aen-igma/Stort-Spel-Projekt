#pragma once
#include"DX11Core.h"

namespace Aen {
	struct DepthStencil : public GCore {
		ComDepthStencilView dsView;
		ComDepthStencilState dsState;

		~DepthStencil() = default;

		const bool Initialize(const Window& window);
	};
}
