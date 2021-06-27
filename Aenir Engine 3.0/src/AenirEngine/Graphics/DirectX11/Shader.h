#pragma once
#include"DX11Core.h"

namespace Aen {
	namespace Concealed {
		template<typename T>
		struct TShader : public GCore {
			Microsoft::WRL::ComPtr<T> shader;
			ID3D10Blob* byteCode;

			~TShader() = default;

			const bool Initialize(const std::wstring& dir);
		};

		inline const bool TShader<ID3D11VertexShader>::Initialize(const std::wstring& dir) {
			if (FAILED(D3DReadFileToBlob(dir.c_str(), &byteCode)))
				return false;
			return SUCCEEDED(device->CreateVertexShader(byteCode->GetBufferPointer(), byteCode->GetBufferSize(), nullptr, &shader));
		}

		inline const bool TShader<ID3D11HullShader>::Initialize(const std::wstring& dir) {
			if (FAILED(D3DReadFileToBlob(dir.c_str(), &byteCode)))
				return false;
			return SUCCEEDED(device->CreateHullShader(byteCode->GetBufferPointer(), byteCode->GetBufferSize(), nullptr, &shader));
		}

		inline const bool TShader<ID3D11ComputeShader>::Initialize(const std::wstring& dir) {
			if (FAILED(D3DReadFileToBlob(dir.c_str(), &byteCode)))
				return false;
			return SUCCEEDED(device->CreateComputeShader(byteCode->GetBufferPointer(), byteCode->GetBufferSize(), nullptr, &shader));
		}

		inline const bool TShader<ID3D11DomainShader>::Initialize(const std::wstring& dir) {
			if (FAILED(D3DReadFileToBlob(dir.c_str(), &byteCode)))
				return false;
			return SUCCEEDED(device->CreateDomainShader(byteCode->GetBufferPointer(), byteCode->GetBufferSize(), nullptr, &shader));
		}

		inline const bool TShader<ID3D11GeometryShader>::Initialize(const std::wstring& dir) {
			if (FAILED(D3DReadFileToBlob(dir.c_str(), &byteCode)))
				return false;
			return SUCCEEDED(device->CreateGeometryShader(byteCode->GetBufferPointer(), byteCode->GetBufferSize(), nullptr, &shader));
		}

		inline const bool TShader<ID3D11PixelShader>::Initialize(const std::wstring& dir) {
			if (FAILED(D3DReadFileToBlob(dir.c_str(), &byteCode)))
				return false;
			return SUCCEEDED(device->CreatePixelShader(byteCode->GetBufferPointer(), byteCode->GetBufferSize(), nullptr, &shader));
		}
	}
	
	using VShader = Concealed::TShader<ID3D11VertexShader>;
	using HShader = Concealed::TShader<ID3D11HullShader>;
	using CShader = Concealed::TShader<ID3D11ComputeShader>;
	using DShader = Concealed::TShader<ID3D11DomainShader>;
	using GShader = Concealed::TShader<ID3D11GeometryShader>;
	using PShader = Concealed::TShader<ID3D11PixelShader>;
}