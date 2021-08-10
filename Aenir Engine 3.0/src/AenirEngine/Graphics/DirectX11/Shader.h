#pragma once
#include"DX11Core.h"

namespace Aen {

	namespace Concealed {

		template<typename T>
		class TShader : public GCore {
			public:
			TShader()
				:m_shader(NULL), m_byteCode(nullptr) {}

			const bool Create(const std::wstring& dir);

			//private:
			Microsoft::WRL::ComPtr<T> m_shader;
			ID3D10Blob* m_byteCode;

			//friend class ILayout; // this not working
			friend class Reflection; // this not working
		};

		template<>
		inline const bool TShader<ID3D11VertexShader>::Create(const std::wstring& dir) {
			if (FAILED(D3DReadFileToBlob(dir.c_str(), &m_byteCode)))
				return false;
			return SUCCEEDED(m_device->CreateVertexShader(m_byteCode->GetBufferPointer(), m_byteCode->GetBufferSize(), nullptr, &m_shader));
		}

		template<>
		inline const bool TShader<ID3D11HullShader>::Create(const std::wstring& dir) {
			if (FAILED(D3DReadFileToBlob(dir.c_str(), &m_byteCode)))
				return false;
			return SUCCEEDED(m_device->CreateHullShader(m_byteCode->GetBufferPointer(), m_byteCode->GetBufferSize(), nullptr, &m_shader));
		}

		template<>
		inline const bool TShader<ID3D11ComputeShader>::Create(const std::wstring& dir) {
			if (FAILED(D3DReadFileToBlob(dir.c_str(), &m_byteCode)))
				return false;
			return SUCCEEDED(m_device->CreateComputeShader(m_byteCode->GetBufferPointer(), m_byteCode->GetBufferSize(), nullptr, &m_shader));
		}

		template<>
		inline const bool TShader<ID3D11DomainShader>::Create(const std::wstring& dir) {
			if (FAILED(D3DReadFileToBlob(dir.c_str(), &m_byteCode)))
				return false;
			return SUCCEEDED(m_device->CreateDomainShader(m_byteCode->GetBufferPointer(), m_byteCode->GetBufferSize(), nullptr, &m_shader));
		}

		template<>
		inline const bool TShader<ID3D11GeometryShader>::Create(const std::wstring& dir) {
			if (FAILED(D3DReadFileToBlob(dir.c_str(), &m_byteCode)))
				return false;
			return SUCCEEDED(m_device->CreateGeometryShader(m_byteCode->GetBufferPointer(), m_byteCode->GetBufferSize(), nullptr, &m_shader));
		}

		template<>
		inline const bool TShader<ID3D11PixelShader>::Create(const std::wstring& dir) {
			if (FAILED(D3DReadFileToBlob(dir.c_str(), &m_byteCode)))
				return false;
			return SUCCEEDED(m_device->CreatePixelShader(m_byteCode->GetBufferPointer(), m_byteCode->GetBufferSize(), nullptr, &m_shader));
		}
	}
	
	using VShader = Concealed::TShader<ID3D11VertexShader>;
	using HShader = Concealed::TShader<ID3D11HullShader>;
	using CShader = Concealed::TShader<ID3D11ComputeShader>;
	using DShader = Concealed::TShader<ID3D11DomainShader>;
	using GShader = Concealed::TShader<ID3D11GeometryShader>;
	using PShader = Concealed::TShader<ID3D11PixelShader>;
}