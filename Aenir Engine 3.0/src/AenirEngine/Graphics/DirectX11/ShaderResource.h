#pragma once
#include"DX11Core.h"
#include"..\..\ThirdParty\stb_image.h"
#include<vector>

namespace Aen {

	struct ShaderResource : public GCore {
		public:
		ShaderResource(const std::string& dir);

		private:
		ComShaderResourceView m_srv;
	};
}
