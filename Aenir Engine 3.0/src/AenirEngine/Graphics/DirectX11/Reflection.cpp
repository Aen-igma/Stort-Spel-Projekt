#include "PCH.h"
#include "Reflection.h"

namespace Aen {

	Reflection::~Reflection() {
		m_reflection.Reset();
	}

	Reflection::Reflection()
		:m_reflection(NULL) {}

	const D3D11_SHADER_DESC& Reflection::GetShaderDesc() const {
		return m_shaderDesc;
	}
}