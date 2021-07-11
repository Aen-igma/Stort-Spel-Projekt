#pragma once
#include"DX11Core.h"

namespace Aen {

	class RState : public GCore {
		public:

		RState() = delete;
		RState(const D3D11_FILL_MODE& fill, const D3D11_CULL_MODE& cull);

		void SetRState();

		friend class RenderSystem;

		private:
		ComRasterizerState m_rState;
	};
}