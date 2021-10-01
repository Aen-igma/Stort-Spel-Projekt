#pragma once
#include"Shader.h"

namespace Aen {

	class DepthMap : public GCore {
		public:
		~DepthMap();
		DepthMap();
		DepthMap(const UINT& dimensions);
		DepthMap(const UINT& width, const UINT& height);
		DepthMap(const Window& window);

		void Create(const UINT& dimensions);
		void Create(const UINT& width, const UINT& height);
		void Create(const Window& window);
		void SetViewPort();

		friend class RenderSystem;

		private:
		ComDepthStencilView m_dsView;
		ComShaderResourceView m_srv;
		D3D11_VIEWPORT m_viewPort;
	};
}