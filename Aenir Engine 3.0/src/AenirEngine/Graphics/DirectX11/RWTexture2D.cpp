#include "PCH.h"
#include "RWTexture2D.h"

namespace Aen {
	
	RWTexture2D::~RWTexture2D() {
		m_uav.Reset();
		m_srv.Reset();
	}

	RWTexture2D::RWTexture2D()
		:m_uav(NULL), m_srv(NULL) {}
	
	void RWTexture2D::Create(const UINT& width, const UINT& height) {
		
		ComTexture2D tex = NULL;
		D3D11_TEXTURE2D_DESC textureDesc;
		ZeroMemory(&textureDesc, sizeof(D3D11_TEXTURE2D_DESC));
		textureDesc.Width = width;
		textureDesc.Height = height;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_R32_UINT;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;

		if(FAILED(m_device->CreateTexture2D(&textureDesc, 0, tex.GetAddressOf())))
			throw;
		
		D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
		ZeroMemory(&uavDesc, sizeof(D3D11_UNORDERED_ACCESS_VIEW_DESC));
		uavDesc.Format = DXGI_FORMAT_UNKNOWN;
		uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
		uavDesc.Texture2D.MipSlice = 0;

		if(FAILED(m_device->CreateUnorderedAccessView(tex.Get(), &uavDesc, &m_uav)))
			throw;

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		srvDesc.Format = textureDesc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = 1;
		if(FAILED(m_device->CreateShaderResourceView(tex.Get(), &srvDesc, m_srv.GetAddressOf())))
			throw;

		tex.Reset();
	}
}