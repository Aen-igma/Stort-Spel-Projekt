#pragma once
#include"Shader.h"

namespace Aen {

	class ILayout : public GCore {
		public:
		~ILayout();
		ILayout();

		void Create(VShader& shader);

		friend class RenderSystem;

		std::vector<D3D11_INPUT_ELEMENT_DESC> m_inputDesc;

		private:
		ComInputLayout m_layout;
	};
}
