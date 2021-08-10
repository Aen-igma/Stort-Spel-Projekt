#pragma once
#include"Shader.h"

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

		const D3D11_SHADER_DESC& GetShaderDesc() const;

		private:
		ComReflection m_reflection;
		D3D11_SHADER_DESC m_shaderDesc;
	};
	
	#define X(T) template<> inline Reflection::Reflection(T& shader) {\
		if(FAILED(D3DReflect(shader.m_byteCode->GetBufferPointer(), shader.m_byteCode->GetBufferSize(), __uuidof(ID3D11ShaderReflection), reinterpret_cast<void**>(m_reflection.GetAddressOf()))))\
			throw;\
		\
		if(FAILED(m_reflection->GetDesc(&m_shaderDesc)))\
			throw;\
	}

	AEN_DEF
	#undef X

	#define X(T) template<> inline void Reflection::Create(T& shader) {\
		if(FAILED(D3DReflect(shader.m_byteCode->GetBufferPointer(), shader.m_byteCode->GetBufferSize(), __uuidof(ID3D11ShaderReflection), reinterpret_cast<void**>(m_reflection.GetAddressOf()))))\
			throw;\
	\
		if(FAILED(m_reflection->GetDesc(&m_shaderDesc)))\
			throw;\
	}

	AEN_DEF
	#undef X
	#undef AEN_DEF
}