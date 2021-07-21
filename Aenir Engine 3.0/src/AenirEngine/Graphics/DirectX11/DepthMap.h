#pragma once
#include"Shader.h"

namespace Aen {

	class DepthMap : public GCore {
		public:
		~DepthMap();
		DepthMap() = delete;
		DepthMap(const UINT& dimensions);

		void SetViewPort();
		void SetDSView();

		friend class RenderSystem;

		private:
		ComDepthStencilView m_dsView;
		ComShaderResourceView m_srv;
		D3D11_VIEWPORT m_viewPort;
	};
}