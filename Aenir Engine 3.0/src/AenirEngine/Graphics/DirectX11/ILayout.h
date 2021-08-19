#pragma once
#include"Reflection.h"

namespace Aen {

	class ILayout : public GCore {
		public:
		~ILayout();
		ILayout();

		void Create(VShader& shader);

		friend class RenderSystem;

		private:
		const DXGI_FORMAT GetFormat(const D3D_REGISTER_COMPONENT_TYPE& type, const BYTE& mask);

		ComInputLayout m_layout;
		std::vector<D3D11_INPUT_ELEMENT_DESC> m_inputDesc;
	};
}
