#pragma once
#include"Shader.h"
#include<optional>

namespace Aen {

	#define AEN_DEF X(VShader)\
					X(HShader)\
					X(CShader)\
					X(GShader)\
					X(PShader)

	class Reflection : public GCore {
		public:
		~Reflection();
		Reflection();

		template<class T>
		Reflection(T& shader);

		template<class T>
		void Create(T& shader);

		const D3D11_SHADER_DESC& GetShaderDesc();
		const std::vector<D3D11_SIGNATURE_PARAMETER_DESC>& GetInputDesc();
		const std::vector<D3D11_SIGNATURE_PARAMETER_DESC>& GetOutputDesc();
		const std::optional<D3D11_SHADER_BUFFER_DESC> GetBufferDescByName(const std::string& name);
		const std::optional<D3D11_SHADER_INPUT_BIND_DESC> GetBindDescByName(const std::string& name);
		const std::unordered_map<std::string, D3D11_SHADER_BUFFER_DESC>& GetBufferDescs();
		const std::unordered_map<std::string, D3D11_SHADER_INPUT_BIND_DESC>& GetBindDescs();

		private:
		ComReflection m_reflection;
		D3D11_SHADER_DESC m_shaderDesc;
		std::vector<D3D11_SIGNATURE_PARAMETER_DESC> m_inputDesc;
		std::vector<D3D11_SIGNATURE_PARAMETER_DESC> m_outputDesc;
		std::unordered_map<std::string, D3D11_SHADER_BUFFER_DESC> m_bufferDesc;
		std::unordered_map<std::string, D3D11_SHADER_INPUT_BIND_DESC> m_bindDesc;
	};
	
	#define X(T) template<> inline Reflection::Reflection(T& shader) {\
		if(FAILED(D3DReflect(shader.m_byteCode->GetBufferPointer(), shader.m_byteCode->GetBufferSize(), IID_ID3D11ShaderReflection, reinterpret_cast<void**>(m_reflection.GetAddressOf()))))\
			throw;\
		\
		if(FAILED(m_reflection->GetDesc(&m_shaderDesc)))\
			throw;\
		\
		ID3D11ShaderReflectionConstantBuffer* CBR = nullptr;\
		for(uint32_t i = 0; i < m_shaderDesc.ConstantBuffers; i++) {\
			D3D11_SHADER_BUFFER_DESC bDTemp;\
			CBR = m_reflection->GetConstantBufferByIndex(i);\
			CBR->GetDesc(&bDTemp);\
			m_bufferDesc.emplace(bDTemp.Name, bDTemp);\
		}\
		\
		for(uint32_t i = 0; i < m_shaderDesc.BoundResources; i++) {\
			D3D11_SHADER_INPUT_BIND_DESC bDTemp;\
			m_reflection->GetResourceBindingDesc(i, &bDTemp);\
			m_bindDesc.emplace(bDTemp.Name, bDTemp);\
		}\
		\
		m_inputDesc.reserve(m_shaderDesc.InputParameters);\
		m_inputDesc.resize(m_shaderDesc.InputParameters);\
		for(uint32_t i = 0; i < m_shaderDesc.InputParameters; i++)\
			m_reflection->GetInputParameterDesc (i, &m_inputDesc[i]);\
		\
		m_outputDesc.reserve(m_shaderDesc.OutputParameters);\
		m_outputDesc.resize(m_shaderDesc.OutputParameters);\
		for(uint32_t i = 0; i < m_shaderDesc.OutputParameters; i++)\
			m_reflection->GetOutputParameterDesc (i, &m_outputDesc[i]);\
	}

	AEN_DEF
	#undef X

	#define X(T) template<> inline void Reflection::Create(T& shader) {\
		if(FAILED(D3DReflect(shader.m_byteCode->GetBufferPointer(), shader.m_byteCode->GetBufferSize(), IID_ID3D11ShaderReflection, reinterpret_cast<void**>(m_reflection.GetAddressOf()))))\
			throw;\
		\
		if(FAILED(m_reflection->GetDesc(&m_shaderDesc)))\
			throw;\
		\
		ID3D11ShaderReflectionConstantBuffer* CBR = nullptr;\
		for(uint32_t i = 0; i < m_shaderDesc.ConstantBuffers; i++) {\
			D3D11_SHADER_BUFFER_DESC bDTemp;\
			CBR = m_reflection->GetConstantBufferByIndex(i);\
			CBR->GetDesc(&bDTemp);\
			m_bufferDesc.emplace(bDTemp.Name, bDTemp);\
		}\
		\
		for(uint32_t i = 0; i < m_shaderDesc.BoundResources; i++) {\
			D3D11_SHADER_INPUT_BIND_DESC bDTemp;\
			m_reflection->GetResourceBindingDesc(i, &bDTemp);\
			m_bindDesc.emplace(bDTemp.Name, bDTemp);\
		}\
		\
		m_inputDesc.reserve(m_shaderDesc.InputParameters);\
		m_inputDesc.resize(m_shaderDesc.InputParameters);\
		for(uint32_t i = 0; i < m_shaderDesc.InputParameters; i++)\
			m_reflection->GetInputParameterDesc (i, &m_inputDesc[i]);\
		\
		m_outputDesc.reserve(m_shaderDesc.OutputParameters);\
		m_outputDesc.resize(m_shaderDesc.OutputParameters);\
		for(uint32_t i = 0; i < m_shaderDesc.OutputParameters; i++)\
			m_reflection->GetOutputParameterDesc (i, &m_outputDesc[i]);\
	}

	AEN_DEF
	#undef X
	#undef AEN_DEF
}