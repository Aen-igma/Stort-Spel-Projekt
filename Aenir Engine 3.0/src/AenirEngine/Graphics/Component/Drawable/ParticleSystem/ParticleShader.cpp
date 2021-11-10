#include "PCH.h"
#include "ParticleShader.h"

Aen::ParticleShaderComponent::ParticleShaderComponent()
{
	this->m_layout = 0;
	this->m_matrixBuffer = 0;
	this->m_samplerState = 0;
}


Aen::ParticleShaderComponent::~ParticleShaderComponent()
{
}

bool Aen::ParticleShaderComponent::Initialize(ComDevice*& device)
{
	bool result;
	result = InitializeShader(device);
	if (!result)
	{
		return false;
	}
	return true;
}

void Aen::ParticleShaderComponent::Shutdown()
{
	ShutdownShader();
	return;
}



bool Aen::ParticleShaderComponent::InitializeShader(ComDevice*& device)
{
	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_SAMPLER_DESC samplerDesc;
	HRESULT result;
	if (m_vertexShader->Create(AEN_OUTPUT_DIR_WSTR(L"VertexParticleShader.cso")))
	{
		if (!m_vertexShader->Create(L"VertexParticleShader.cso"))
		{
			throw;
		}
	}
	if (m_geometryShader->Create(AEN_OUTPUT_DIR_WSTR(L"GeometryParticleShader.cso")))
	{
		if (!m_geometryShader->Create(AEN_OUTPUT_DIR_WSTR(L"GeometryParticleShader.cso")))
		{
			throw;
		}
	}
	if (m_pixelShader->Create(AEN_OUTPUT_DIR_WSTR(L"PixelParticleShader.cso")))
	{
		if (!m_pixelShader->Create(L"PixelParticleShader.cso"))
		{
			throw;
		}
	}
	D3D11_INPUT_ELEMENT_DESC polyLayout[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,0,0, D3D11_INPUT_PER_VERTEX_DATA,0},
		{"UV", 0, DXGI_FORMAT_R32G32_FLOAT,0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	numElements = sizeof(polyLayout) / sizeof(polyLayout[0]);
	result = device->Get()->CreateInputLayout(polyLayout,numElements,m_vertexShader->m_byteCode->GetBufferPointer()
		,m_vertexShader->m_byteCode->GetBufferSize()/*,m_layout->GetAddressOf()*/, &this->m_layout );
	if (FAILED(result))
	{
		return false;
	}
	this->m_vertexShader->m_byteCode.Get()->Release();
	this->m_geometryShader->m_byteCode.Get()->Release();
	this->m_computeShader->m_byteCode.Get()->Release();
	this->m_computeShader->m_byteCode.Get()->Release();

	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(m_MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;
	result = device->Get()->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);
	if (FAILED(result))
	{
		std::cout << "Failed to create Buffer" << std::endl;
		return false;
	}
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	result = device->Get()->CreateSamplerState(&samplerDesc, &m_samplerState);
	if (FAILED(result))
	{
		return false;
	}
	return true;
}

void Aen::ParticleShaderComponent::ShutdownShader()
{
	if (this->m_samplerState)
	{
		this->m_samplerState->Release();
		this->m_samplerState = 0;
	}
	if (this->m_matrixBuffer)
	{
		this->m_matrixBuffer->Release();
		this->m_matrixBuffer = 0;
	}
	if (this->m_layout)
	{
		this->m_layout->Release();
		this->m_layout = 0;
	}
	if (m_vertexShader)
	{
		delete m_vertexShader;
	}
	if (m_pixelShader)
	{
		delete m_pixelShader;
	}
	if (m_geometryShader)
	{
		delete m_geometryShader;
	}
	if (m_computeShader)
	{
		delete m_computeShader;
	}
	return;
}

void Aen::ParticleShaderComponent::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename)
{
}

bool Aen::ParticleShaderComponent::SetShaderParameters(ComDeviceContext*& deviceContext, ParticleSystem& ps, float runtime, const DirectX::XMMATRIX& worldMatrix, const DirectX::XMMATRIX& viewMatrix, const DirectX::XMMATRIX& projectionMatrix, ID3D11ShaderResourceView*& texture)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	result = deviceContext->Get()->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}
	m_MatrixBufferType* dataPtr = (m_MatrixBufferType*)mappedResource.pData;

	dataPtr->world = worldMatrix;
	dataPtr->view = viewMatrix;
	dataPtr->projection = projectionMatrix;
	deviceContext->Get()->Unmap(m_matrixBuffer, 0);

	//____________________________ CS RUNTIME ____________________________//
	CSInputBuffer inputData;
	inputData.m_runtime = { runtime, 0.0f, 0.0f, 0.0f };
	inputData.m_particleCount = { (float)ps.GetParticleCount(), 0.0f, 0.0f, 0.0f };
	D3D11_MAPPED_SUBRESOURCE MS_DeltaTime;
	deviceContext->Get()->Map(ps.GetConstantRunTimeBufferPtr(), 0, D3D11_MAP_WRITE_DISCARD, 0, &MS_DeltaTime);
	CSInputBuffer* lInputDataPtr = (CSInputBuffer*)MS_DeltaTime.pData;
	lInputDataPtr->m_runtime = inputData.m_runtime;
	lInputDataPtr->m_particleCount = inputData.m_particleCount;
	deviceContext->Get()->Unmap(ps.GetConstantRunTimeBufferPtr(), 0);
	return true;
}

void Aen::ParticleShaderComponent::RenderShader(ComDeviceContext*& deviceContext)
{
	deviceContext->Get()->VSSetShader(this->m_vertexShader->m_shader.Get(), NULL, 0);
	deviceContext->Get()->GSSetShader(this->m_geometryShader->m_shader.Get(), NULL, 0);
	deviceContext->Get()->PSSetShader(this->m_pixelShader->m_shader.Get(),NULL,0);
	return;
}

bool Aen::ParticleShaderComponent::Render(ComDeviceContext*& deviceContext, 
	const DirectX::XMMATRIX& worldMatrix, const DirectX::XMMATRIX& viewMatrix, 
	const DirectX::XMMATRIX& projectionMatrix, float runtime, ID3D11ShaderResourceView*& texture, ParticleSystem& ps)
{
	SetShaderParameters(deviceContext, ps, ps.GetRunTime(), worldMatrix, viewMatrix, projectionMatrix, texture);
	deviceContext->Get()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	RenderShader(deviceContext);

	deviceContext->Get()->VSSetConstantBuffers(4, 1, &m_matrixBuffer);
	deviceContext->Get()->VSSetShaderResources(0, 1, ps.GetOutputSRV());
	deviceContext->Get()->GSSetConstantBuffers(0, 1, &m_matrixBuffer);
	deviceContext->Get()->PSSetSamplers(0, 1, &m_samplerState);
	deviceContext->Get()->PSSetShaderResources(0, 1, &texture);

	//Borde stämma med antal vertiser, måste verfiera det senare
	deviceContext->Get()->Draw(ps.GetParticleCount(), 0);
	deviceContext->Get()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//Unbind
	ID3D11ShaderResourceView* unboundSRV[] = { NULL };
	deviceContext->Get()->VSSetShaderResources(0, 1, unboundSRV);
	deviceContext->Get()->GSSetShader(NULL, NULL, 0);

	return true;
}

void Aen::ParticleShaderComponent::UpdateComputeShader(ComDeviceContext*& deviceContext, ParticleSystem& particleSystem)
{
	deviceContext->Get()->CSSetShader(this->m_computeShader->m_shader.Get(),NULL,0);
	deviceContext->Get()->CSSetConstantBuffers(0,1, particleSystem.GetConstantRunTimeBufferReference());
	deviceContext->Get()->CSSetUnorderedAccessViews(0,1,particleSystem.GetOutputUAV(), NULL);
	deviceContext->Get()->Dispatch((int)(particleSystem.GetMaxParticleCount() / 64.0f) + (particleSystem.GetMaxParticleCount() % 64 != 0), 1, 1);

	//Unbinding
	deviceContext->Get()->CSSetShader(NULL,NULL,0);
	ID3D11UnorderedAccessView* unboundUAV[] = { NULL };
	deviceContext->Get()->CSSetUnorderedAccessViews(0, 1, unboundUAV, NULL);
}