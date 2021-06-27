#pragma once
#include"DX11Core.h"
#include"..\..\ThirdParty\stb_image.h"
#include<vector>

namespace Aen {
	struct ShaderResource : public GCore {
		ComShaderResourceView srv;

		~ShaderResource() = default;

		bool Initialize(const std::string& dir);
	};
}
