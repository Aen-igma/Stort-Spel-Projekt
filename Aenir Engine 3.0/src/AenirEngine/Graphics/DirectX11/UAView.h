#pragma once
#include"BBuffer.h"

namespace Aen {

	class UAView : public GCore {
		public:
		~UAView();
		UAView();
		UAView(const uint32_t& stride, const uint32_t& size);

		void Create(const uint32_t& stride, const uint32_t& size);
		void Create(BBuffer& bBuffer);

		private:
		ComBuffer m_buffer;
		ComUnorderedAccessView m_uav;
		ComShaderResourceView m_srv;

		friend class RenderSystem;
	};
}