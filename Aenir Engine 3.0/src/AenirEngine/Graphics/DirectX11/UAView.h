#pragma once
#include"DX11Core.h"

namespace Aen {

	class UAView : public GCore {
		public:
		~UAView();
		UAView();
		UAView(const uint32_t& stride, const uint32_t& size);

		void Create(const uint32_t& stride, const uint32_t& size);

		private:
		ComBuffer m_buffer;
		ComUnorderedAccessView m_uav;
		ComShaderResourceView m_srv;

		friend class RenderSystem;
	};
}