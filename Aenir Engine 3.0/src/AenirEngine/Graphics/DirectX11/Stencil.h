#pragma once
#include"DX11Core.h"

namespace Aen {

	enum class StencilType {
		Write,
		Mask,
		Off
	};

	class Stencil : public GCore {
		public:
		~Stencil();
		Stencil();
		Stencil(const bool& useDepth, const StencilType& type);

		void SetStencil(const bool& useDepth, const StencilType& type);
		friend class RenderSystem;

		private:
		ComDepthStencilState m_dsState;
	};

}