#pragma once
#include"DX11Core.h"

namespace Aen {

	class RWTexture2D : public GCore {
		public:
		~RWTexture2D();
		RWTexture2D();

		void Create(const UINT& width, const UINT& height);

		friend class RenderSystem;

		private:
		ComUnorderedAccessView m_uav;
		ComShaderResourceView m_srv;
	};
}