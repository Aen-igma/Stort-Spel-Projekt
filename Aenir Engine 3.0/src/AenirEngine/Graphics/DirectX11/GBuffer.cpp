#include "PCH.h"
#include "GBuffer.h"

namespace Aen {

	GBuffer::GBuffer(const uint32_t& count, Window& window) {

		m_rtvs.resize(count);
		m_srvs.resize(count);

		D3D11_TEXTURE2D_DESC tDesc;
		ZeroMemory(&tDesc, sizeof(D3D11_TEXTURE2D_DESC));
		tDesc.Width = window.GetSize().x;
		tDesc.Height = window.GetSize().y;
		tDesc.MipLevels = 1;
		tDesc.ArraySize = 1;
		tDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		tDesc.SampleDesc.Count = 1;
		tDesc.SampleDesc.Quality = 0;
		tDesc.Usage = D3D11_USAGE_DEFAULT;
		tDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		tDesc.CPUAccessFlags = 0;
		tDesc.MiscFlags = 0;


		D3D11_RENDER_TARGET_VIEW_DESC rDesc;
		ZeroMemory(&rDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
		rDesc.Format = tDesc.Format;
		rDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		rDesc.Texture2D.MipSlice = 0;


		D3D11_SHADER_RESOURCE_VIEW_DESC sDesc;
		ZeroMemory(&sDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
		sDesc.Format = tDesc.Format;
		sDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		sDesc.Texture2D.MostDetailedMip = 0;
		sDesc.Texture2D.MipLevels = 1;

		std::vector<ComTexture2D> texture(count, nullptr);

		for(UINT i = 0; i < count; i++) {
			if(FAILED(m_device->CreateTexture2D(&tDesc, NULL, &texture[i])))
				throw;

			if(FAILED(m_device->CreateShaderResourceView(texture[i].Get(), &sDesc, &m_srvs[i])))
				throw;

			if(FAILED(m_device->CreateRenderTargetView(texture[i].Get(), &rDesc, &m_rtvs[i])))
				throw;
		}
	}

    void GBuffer::Create(const uint32_t& count, Window& window) {
		
        m_rtvs.resize(count);
        m_srvs.resize(count);

		D3D11_TEXTURE2D_DESC tDesc;
		ZeroMemory(&tDesc, sizeof(D3D11_TEXTURE2D_DESC));
		tDesc.Width = window.GetSize().x;
		tDesc.Height = window.GetSize().y;
		tDesc.MipLevels = 1;
		tDesc.ArraySize = 1;
		tDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		tDesc.SampleDesc.Count = 1;
		tDesc.SampleDesc.Quality = 0;
		tDesc.Usage = D3D11_USAGE_DEFAULT;
		tDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		tDesc.CPUAccessFlags = 0;
		tDesc.MiscFlags = 0;


		D3D11_RENDER_TARGET_VIEW_DESC rDesc;
		ZeroMemory(&rDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
		rDesc.Format = tDesc.Format;
		rDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		rDesc.Texture2D.MipSlice = 0;


		D3D11_SHADER_RESOURCE_VIEW_DESC sDesc;
		ZeroMemory(&sDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
		sDesc.Format = tDesc.Format;
		sDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		sDesc.Texture2D.MostDetailedMip = 0;
		sDesc.Texture2D.MipLevels = 1;

		std::vector<ComTexture2D> texture(count, nullptr);

		for(UINT i = 0; i < count; i++) {
			if(FAILED(m_device->CreateTexture2D(&tDesc, NULL, &texture[i])))
				throw;

			if(FAILED(m_device->CreateShaderResourceView(texture[i].Get(), &sDesc, &m_srvs[i])))
				throw;

			if(FAILED(m_device->CreateRenderTargetView(texture[i].Get(), &rDesc, &m_rtvs[i])))
				throw;
		}
    }

	const uint32_t GBuffer::GetCount() {
		return m_rtvs.size();
	}

}