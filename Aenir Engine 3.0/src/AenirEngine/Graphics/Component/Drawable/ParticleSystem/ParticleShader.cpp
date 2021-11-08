#include "PCH.h"
#include "ParticleShader.h"

Aen::ParticleShaderComponent::ParticleShaderComponent()
{
	this->m_layout = 0;
	this->m_matrixBuffer = 0;
	this->m_samplerState = 0;
}

Aen::ParticleShaderComponent::ParticleShaderComponent(const ParticleShaderComponent& other)
{
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

bool Aen::ParticleShaderComponent::Render(ComDeviceContext*& deviceContext, const DirectX::XMMATRIX& worldMatrix, const DirectX::XMMATRIX& viewMatrix, const DirectX::XMMATRIX& projectionMatrix, float runtime, ID3D11ShaderResourceView*& texture, ParticleSystemComponent& particleSystem)
{
	return false;
}

bool Aen::ParticleShaderComponent::InitializeShader(ComDevice*& device)
{
	return false;
}

void Aen::ParticleShaderComponent::ShutdownShader()
{
	if (this->m_samplerState)
	{
		this->m_samplerState->Get()->Release();
		this->m_samplerState = 0;
	}
	if (this->m_matrixBuffer)
	{
		this->m_matrixBuffer->Get()->Release();
		this->m_matrixBuffer = 0;
	}
	if (this->m_layout)
	{
		this->m_layout->Get()->Release();
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

bool Aen::ParticleShaderComponent::SetShaderParameters(ComDeviceContext*& deviceContext, ParticleSystemComponent& particleSystem, float runtime, const DirectX::XMMATRIX& worldMatrix, const DirectX::XMMATRIX& viewMatrix, const DirectX::XMMATRIX& projectionMatrix, ID3D11ShaderResourceView*& texture)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	result = deviceContext->Get()->Map(m_matrixBuffer->Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}
	m_MatrixBufferType* dataPtr = (m_MatrixBufferType*)mappedResource.pData;

	dataPtr->world = worldMatrix;
	dataPtr->view = viewMatrix;
	dataPtr->projection = projectionMatrix;
	deviceContext->Get()->Unmap(m_matrixBuffer->Get(), 0);

	//____________________________ CS RUNTIME ____________________________//
	CSInputBuffer inputData;
	inputData.m_runtime = { runtime, 0.0f, 0.0f, 0.0f };
	inputData.m_particleCount = { (float)particleSystem.GetParticleCount(), 0.0f, 0.0f, 0.0f };
	D3D11_MAPPED_SUBRESOURCE MS_DeltaTime;
	deviceContext->Get()->Map(particleSystem.GetConstantRunTimeBufferPtr(), 0, D3D11_MAP_WRITE_DISCARD, 0, &MS_DeltaTime);
	CSInputBuffer* lInputDataPtr = (CSInputBuffer*)MS_DeltaTime.pData;
	lInputDataPtr->m_runtime = inputData.m_runtime;
	lInputDataPtr->m_particleCount = inputData.m_particleCount;
	deviceContext->Get()->Unmap(particleSystem.GetConstantRunTimeBufferPtr(), 0);
	return true;
}

void Aen::ParticleShaderComponent::RenderShader(ComDeviceContext*& deviceContext)
{
	deviceContext->Get()->VSSetShader(this->m_vertexShader, NULL, 0);
	deviceContext->Get()->GSSetShader(this->m_geometryShader, NULL, 0);
	deviceContext->Get()->PSSetShader(this->m_pixelShader,NULL,0);
	return;
}

void Aen::ParticleShaderComponent::UpdateComputeShader(ComDeviceContext*& deviceContext, ParticleSystemComponent& particleSystem)
{
	deviceContext->Get()->CSSetShader(this->m_computeShader,NULL,0);
	deviceContext->Get()->CSSetConstantBuffers(0,1, particleSystem.GetConstantRunTimeBufferReference());
	deviceContext->Get()->CSSetUnorderedAccessViews(0,1,particleSystem.GetOutputUAV(), NULL);
	
	deviceContext->Get()->Dispatch((int)(particleSystem.GetMaxParticleCount() / 64.0f) + (particleSystem.GetMaxParticleCount() % 64 != 0), 1, 1);

	deviceContext->Get()->CSSetShader(NULL,NULL,0);
	ID3D11UnorderedAccessView* unboundUAV[] = { NULL };
	deviceContext->Get()->CSSetUnorderedAccessViews(0, 1, unboundUAV, NULL);
}