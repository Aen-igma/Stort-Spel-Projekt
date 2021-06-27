#pragma once
#include"Shader.h"
#include<vector>

namespace Aen {
	struct ILayout : public GCore {
		ComInputLayout layout;
		std::vector<D3D11_INPUT_ELEMENT_DESC> inputDesc;

		~ILayout() = default;

		const bool Initialize(const VShader& shader);
	};
}
