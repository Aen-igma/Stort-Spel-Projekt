#include "PCH.h"
#include "UAView.h"

namespace Aen {

	UAView::~UAView() {
		m_buffer.Reset();
		m_uav.Reset();
		m_srv.Reset();
	}

	UAView::UAView() 
		:m_buffer(NULL), m_uav(NULL), m_srv(NULL) {}

	UAView::UAView(const uint32_t& stride, const uint32_t& size) : m_buffer(NULL), m_uav(NULL), m_srv(NULL) {

		D3D11_BUFFER_DESC sDesc;
		ZeroMemory(&sDesc, sizeof(D3D11_BUFFER_DESC));

		sDesc.Usage = D3D11_USAGE_DEFAULT;
		sDesc.ByteWidth = size * stride;
		sDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
		sDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		sDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		sDesc.StructureByteStride = stride;

		if(FAILED(m_device->CreateBuffer(&sDesc, NULL, m_buffer.GetAddressOf())))
			throw;

		D3D11_UNORDERED_ACCESS_VIEW_DESC uADesc;
		ZeroMemory(&uADesc, sizeof(D3D11_UNORDERED_ACCESS_VIEW_DESC));

		uADesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		uADesc.Buffer.FirstElement = 0;
		uADesc.Format = DXGI_FORMAT_UNKNOWN;      
		uADesc.Buffer.NumElements = size; 

		if(FAILED(m_device->CreateUnorderedAccessView(m_buffer.Get(), &uADesc, m_uav.GetAddressOf())))
			throw;

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));

		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
		srvDesc.Buffer.FirstElement = 0;
		srvDesc.Format = DXGI_FORMAT_UNKNOWN;
		srvDesc.Buffer.NumElements = size;

		if(FAILED(m_device->CreateShaderResourceView(m_buffer.Get(), &srvDesc, m_srv.GetAddressOf())))
			throw;
	}

	void UAView::Create(const uint32_t& stride, const uint32_t& size) {

		D3D11_BUFFER_DESC sDesc;
		ZeroMemory(&sDesc, sizeof(D3D11_BUFFER_DESC));

		sDesc.Usage = D3D11_USAGE_DEFAULT;
		sDesc.ByteWidth = size * stride;
		sDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
		sDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		sDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		sDesc.StructureByteStride = stride;

		if(FAILED(m_device->CreateBuffer(&sDesc, NULL, m_buffer.GetAddressOf())))
			throw;

		D3D11_UNORDERED_ACCESS_VIEW_DESC uADesc;
		ZeroMemory(&uADesc, sizeof(D3D11_UNORDERED_ACCESS_VIEW_DESC));

		uADesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		uADesc.Buffer.FirstElement = 0;
		uADesc.Format = DXGI_FORMAT_UNKNOWN;  
		uADesc.Buffer.NumElements = size;

		if(FAILED(m_device->CreateUnorderedAccessView(m_buffer.Get(), &uADesc, m_uav.GetAddressOf())))
			throw;

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));

		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
		srvDesc.Buffer.FirstElement = 0;
		srvDesc.Format = DXGI_FORMAT_UNKNOWN;
		srvDesc.Buffer.NumElements = size;

		if(FAILED(m_device->CreateShaderResourceView(m_buffer.Get(), &srvDesc, m_srv.GetAddressOf())))
			throw;
	}

}