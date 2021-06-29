#include"PCH.h"
#include "DepthMap.h"

namespace Aen {

	DepthMap::DepthMap(const UINT& dimensions)
		:m_dsView(NULL), m_srv(NULL), m_viewPort() {

		ComTexture2D dBuffer;
		D3D11_TEXTURE2D_DESC dtDesc;
		ZeroMemory(&dtDesc, sizeof(D3D11_TEXTURE2D_DESC));

		dtDesc.Width = dimensions;
		dtDesc.Height = dimensions;
		dtDesc.MipLevels = 1;
		dtDesc.ArraySize = 1;
		dtDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
		dtDesc.SampleDesc.Count = 1;
		dtDesc.SampleDesc.Quality = 0;
		dtDesc.Usage = D3D11_USAGE_DEFAULT;
		dtDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
		dtDesc.CPUAccessFlags = 0;
		dtDesc.MiscFlags = 0;

		D3D11_DEPTH_STENCIL_VIEW_DESC dsDesc;
		ZeroMemory(&dsDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));

		dsDesc.Flags = 0;
		dsDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		dsDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		dsDesc.Texture2D.MipSlice = 0;

		D3D11_SHADER_RESOURCE_VIEW_DESC srDesc;
		ZeroMemory(&srDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));

		srDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		srDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srDesc.Texture2D.MipLevels = dtDesc.MipLevels;
		srDesc.Texture2D.MostDetailedMip = 0;

		if(FAILED(m_device->CreateTexture2D(&dtDesc, nullptr, &dBuffer)))
			throw;

		if(FAILED(m_device->CreateDepthStencilView(dBuffer.Get(), &dsDesc, &m_dsView)))
			throw;

		if(FAILED(m_device->CreateShaderResourceView(dBuffer.Get(), &srDesc, &m_srv)))
			throw;

		ZeroMemory(&m_viewPort, sizeof(D3D11_VIEWPORT));

		m_viewPort.TopLeftX = 0;
		m_viewPort.TopLeftY = 0;
		m_viewPort.Width = static_cast<FLOAT>(dimensions);
		m_viewPort.Height = static_cast<FLOAT>(dimensions);
		m_viewPort.MinDepth = 0;
		m_viewPort.MaxDepth = 1;
	}

	void DepthMap::SetViewPort() {
		m_dContext->RSSetViewports(1, &m_viewPort);
	}

	void DepthMap::SetDSView() {
		ComRenderTargetView rtv = NULL;
		m_dContext->OMSetRenderTargets(1, rtv.GetAddressOf(), m_dsView.Get());
	}
}