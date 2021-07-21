#pragma once
#include"DX11Core.h"

namespace Aen {

	enum class FillMode {
		Wireframe = 2,
		Solid = 3,
	};

	enum class CullMode {
		None = 1,
		Front = 2,
		Back = 3
	};

	class RState : public GCore {
		public:
		~RState();
		RState() = delete;
		RState(const FillMode& fill, const CullMode& cull);

		friend class RenderSystem;

		private:
		ComRasterizerState m_rState;
	};
}