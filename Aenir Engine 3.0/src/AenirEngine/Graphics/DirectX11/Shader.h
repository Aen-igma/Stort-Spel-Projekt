#pragma once
#include"DX11Core.h"

namespace Aen {

	namespace Concealed {

		#define AEN_DEF X(ID3D11VertexShader,	 CreateVertexShader)\
						X(ID3D11HullShader,		 CreateHullShader)\
						X(ID3D11ComputeShader,	 CreateComputeShader)\
						X(ID3D11DomainShader,	 CreateDomainShader)\
						X(ID3D11GeometryShader,  CreateGeometryShader)\
						X(ID3D11PixelShader,	 CreatePixelShader)

		template<typename T>
		class TShader : public GCore {
			public:
			TShader()
				:m_shader(NULL), m_byteCode(NULL) {}

			const bool Create(const std::wstring& dir);

			//private:
			Microsoft::WRL::ComPtr<T> m_shader = nullptr;
			ComBlob m_byteCode = nullptr;

			//friend class ILayout; // this not working
			friend class Reflection;
		};

		#define X(T, func) template<> inline const bool TShader<T>::Create(const std::wstring& dir) {\
			if(FAILED(D3DReadFileToBlob(dir.c_str(), m_byteCode.GetAddressOf())))\
				return false;\
			\
			return SUCCEEDED(m_device->func(m_byteCode->GetBufferPointer(), m_byteCode->GetBufferSize(), NULL, m_shader.GetAddressOf()));\
		}

		AEN_DEF
		#undef X
		#undef AEN_DEF
	}
	
	using VShader = Concealed::TShader<ID3D11VertexShader>;
	using HShader = Concealed::TShader<ID3D11HullShader>;
	using CShader = Concealed::TShader<ID3D11ComputeShader>;
	using DShader = Concealed::TShader<ID3D11DomainShader>;
	using GShader = Concealed::TShader<ID3D11GeometryShader>;
	using PShader = Concealed::TShader<ID3D11PixelShader>;
}