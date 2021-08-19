#pragma once
#include<vector>
#include<d3dcompiler.h>
#include<dxgi1_6.h>
#include<d3d11.h>
#include<wrl.h>

using ComDevice = Microsoft::WRL::ComPtr<ID3D11Device>;
using ComDeviceContext = Microsoft::WRL::ComPtr<ID3D11DeviceContext>;
using ComSwapChain = Microsoft::WRL::ComPtr<IDXGISwapChain>;
using ComFactory1 = Microsoft::WRL::ComPtr<IDXGIFactory1>;
using ComFactory2 = Microsoft::WRL::ComPtr<IDXGIFactory2>;
using ComFactory6 = Microsoft::WRL::ComPtr<IDXGIFactory6>;
using ComAdapter1 = Microsoft::WRL::ComPtr<IDXGIAdapter1>;

using ComTexture2D = Microsoft::WRL::ComPtr<ID3D11Texture2D>;
using ComDepthStencilView = Microsoft::WRL::ComPtr<ID3D11DepthStencilView>;
using ComDepthStencilState = Microsoft::WRL::ComPtr<ID3D11DepthStencilState>;
using ComShaderResourceView = Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>;
using ComBuffer = Microsoft::WRL::ComPtr<ID3D11Buffer>;
using ComInputLayout = Microsoft::WRL::ComPtr<ID3D11InputLayout>;
using ComRasterizerState = Microsoft::WRL::ComPtr<ID3D11RasterizerState>;
using ComRenderTargetView = Microsoft::WRL::ComPtr<ID3D11RenderTargetView>;
using ComSamplerState = Microsoft::WRL::ComPtr<ID3D11SamplerState>;
using ComReflection = Microsoft::WRL::ComPtr<ID3D11ShaderReflection>;
using ComBlob = Microsoft::WRL::ComPtr<ID3DBlob>;