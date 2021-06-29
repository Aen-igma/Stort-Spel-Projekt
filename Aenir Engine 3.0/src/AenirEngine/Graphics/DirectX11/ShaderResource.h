#pragma once
#include"DX11Core.h"

namespace Aen {

	struct AEN_DECLSPEC ShaderResource : public GCore {
		public:
		ShaderResource();

		void LoadResource(const std::string& dir);

		private:
		ComShaderResourceView m_srv;
	};
}
