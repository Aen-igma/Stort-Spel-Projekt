#include "PCH.h"
#include "Reflection.h"

namespace Aen {

	Reflection::~Reflection() {
		m_reflection.Reset();
	}

	Reflection::Reflection()
		:m_reflection(NULL) {}
	const D3D11_SHADER_DESC& Reflection::GetShaderDesc() {
		return m_shaderDesc;
	}

	const std::vector<D3D11_SIGNATURE_PARAMETER_DESC>& Reflection::GetInputDesc() {
		return m_inputDesc;
	}

	const std::vector<D3D11_SIGNATURE_PARAMETER_DESC>& Reflection::GetOutputDesc() {
		return m_outputDesc;
	}

	const std::optional<D3D11_SHADER_BUFFER_DESC> Reflection::GetBufferDescByName(const std::string& name) {
		if(m_bufferDesc.count(name) > 0)
			return m_bufferDesc.at(name);

		return {};
	}

	const std::optional<D3D11_SHADER_INPUT_BIND_DESC> Reflection::GetBindDescByName(const std::string& name) {
		if(m_bindDesc.count(name) > 0)
			return m_bindDesc.at(name);

		return {};
	}

	const std::unordered_map<std::string, D3D11_SHADER_BUFFER_DESC>& Reflection::GetBufferDescs() {
		return m_bufferDesc;
	}

	const std::unordered_map<std::string, D3D11_SHADER_INPUT_BIND_DESC>& Reflection::GetBindDescs() {
		return m_bindDesc;
	}
}