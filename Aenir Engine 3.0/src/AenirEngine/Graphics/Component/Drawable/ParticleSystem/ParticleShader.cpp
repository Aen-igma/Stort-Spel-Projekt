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



bool Aen::ParticleShaderComponent::Initialize(ID3D11Device*& device)
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

bool Aen::ParticleShaderComponent::Render(ID3D11DeviceContext*& deviceContext, const DirectX::XMMATRIX& worldMatrix, const DirectX::XMMATRIX& viewMatrix, const DirectX::XMMATRIX& projectionMatrix, float runtime, ID3D11ShaderResourceView*& texture, ParticleSystemComponent& particleSystem)
{
	return false;
}

bool Aen::ParticleShaderComponent::InitializeShader(ID3D11Device*& device)
{
	return false;
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

bool Aen::ParticleShaderComponent::SetShaderParameters(ID3D11DeviceContext*& deviceContext, ParticleSystemComponent& particleSystem, float runtime, const DirectX::XMMATRIX& worldMatrix, const DirectX::XMMATRIX& viewMatrix, const DirectX::XMMATRIX& projectionMatrix, ID3D11ShaderResourceView*& texture)
{
	return false;
}

void Aen::ParticleShaderComponent::RenderShader(ID3D11DeviceContext*& deviceContext)
{
	deviceContext->VSSetShader(this->m_vertexShader, NULL, 0);
	deviceContext->GSSetShader(this->m_geometryShader, NULL, 0);
	deviceContext->PSSetShader(this->m_pixelShader,NULL,0);
	return;
}

void Aen::ParticleShaderComponent::UpdateComputeShader(ID3D11DeviceContext*& deviceContext, ParticleSystemComponent& particleSystem)
{
	deviceContext->CSSetShader(this->m_computeShader,NULL,0);
	deviceContext->CSSetConstantBuffers(0,1, particleSystem.GetConstantRunTimeBufferReference());
	deviceContext->CSSetUnorderedAccessViews(0,1,particleSystem.GetOutputUAV(), NULL);
	
	deviceContext->Dispatch((int)(particleSystem.GetMaxParticleCount() / 64.0f) + (particleSystem.GetMaxParticleCount() % 64 != 0), 1, 1);

	deviceContext->CSSetShader(NULL,NULL,0);
	ID3D11UnorderedAccessView* unboundUAV[] = { NULL };
	deviceContext->CSSetUnorderedAccessViews(0, 1, unboundUAV, NULL);
}