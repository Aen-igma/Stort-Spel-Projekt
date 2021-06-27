#pragma once
#include"DX11Core.h"

namespace Aen {
	struct BackBufferTarget : public GCore {
		ComRenderTargetView rtv;
		~BackBufferTarget() = default;
		const bool Initialize();
	};
}
