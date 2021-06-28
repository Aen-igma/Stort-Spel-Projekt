#pragma once
#include"Shader.h"
#include<vector>

namespace Aen {

	class ILayout : public GCore {
		public:

		ILayout();

		void Create(VShader& shader);
		void SetILayout();

		private:
		ComInputLayout m_layout;
		std::vector<D3D11_INPUT_ELEMENT_DESC> m_inputDesc;
	};
}
