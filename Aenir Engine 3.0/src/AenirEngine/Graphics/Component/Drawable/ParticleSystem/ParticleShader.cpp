//#include "PCH.h"
//#include "ParticleShader.h"
//
//Aen::ParticleShaderComponent::ParticleShaderComponent(const size_t& id)
//	:Drawable(id)
//{
//	this->m_layout = 0;
//	this->m_matrixBuffer = 0;
//	this->m_samplerState = 0;
//	this->m_vertexShader = 0;
//	this->m_geometryShader = 0;
//	this->m_computeShader = 0;
//	this->m_pixelShader = 0;
//}
//
//
//Aen::ParticleShaderComponent::~ParticleShaderComponent()
//{
//}
//
//bool Aen::ParticleShaderComponent::Initialize()
//{
//	if (!InitializeShader())
//		throw;
//}
//
//void Aen::ParticleShaderComponent::Shutdown()
//{
//	ShutdownShader();
//	return;
//}
//
//bool Aen::ParticleShaderComponent::InitializeShader()
//{
//	//HRESULT rs;
//	//unsigned int numElements;
//	//D3D11_BUFFER_DESC matrixBufferDesc;
//	//D3D11_SAMPLER_DESC samplerDesc;
//	//HRESULT result;
//	/*D3D11_INPUT_ELEMENT_DESC polyLayout[] =
//	{
//		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,0,0, D3D11_INPUT_PER_VERTEX_DATA,0},
//		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
//		{"UV", 0, DXGI_FORMAT_R32G32_FLOAT,0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,0},
//		
//	};
//	numElements = sizeof(polyLayout) / sizeof(polyLayout[0]);
//	result = m_device.Get()->CreateInputLayout(polyLayout,numElements,m_vertexShader->m_byteCode->GetBufferPointer()
//		,m_vertexShader->m_byteCode->GetBufferSize(), &this->m_layout );
//	if (FAILED(result))
//	{
//		return false;
//	}
//	this->m_vertexShader->m_byteCode.Get()->Release();
//	this->m_geometryShader->m_byteCode.Get()->Release();
//	this->m_computeShader->m_byteCode.Get()->Release();
//	this->m_computeShader->m_byteCode.Get()->Release();*/
//	/*matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
//	matrixBufferDesc.ByteWidth = sizeof(m_MatrixBufferType);
//	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
//	matrixBufferDesc.CPUAccessFlags = D3D11_BIND_CONSTANT_BUFFER;
//	matrixBufferDesc.MiscFlags = 0;
//	matrixBufferDesc.StructureByteStride = 0;
//	result = m_device.Get()->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);
//	if (FAILED(result))
//	{
//		std::cout << "Failed to create Buffer" << std::endl;
//		return false;
//	}
//	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
//	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
//	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
//	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
//	samplerDesc.MipLODBias = 0.0f;
//	samplerDesc.MaxAnisotropy = 1;
//	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
//	samplerDesc.BorderColor[0] = 0;
//	samplerDesc.BorderColor[1] = 0;
//	samplerDesc.BorderColor[2] = 0;
//	samplerDesc.BorderColor[3] = 0;
//	samplerDesc.MinLOD = 0;
//	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
//	result = m_device.Get()->CreateSamplerState(&samplerDesc, &m_samplerState);
//	if (FAILED(result))
//	{
//		return false;
//	}*/
//	return true;
//}
//
//void Aen::ParticleShaderComponent::ShutdownShader()
//{
//	if (this->m_samplerState)
//	{
//		this->m_samplerState->Release();
//		this->m_samplerState = 0;
//	}
//	if (this->m_matrixBuffer)
//	{
//		this->m_matrixBuffer->Release();
//		this->m_matrixBuffer = 0;
//	}
//	if (this->m_layout)
//	{
//		this->m_layout->Release();
//		this->m_layout = 0;
//	}
//
//	if (m_vertexShader)
//		delete m_vertexShader;
//	if (m_pixelShader)
//		delete m_pixelShader;
//	if (m_geometryShader)
//		delete m_geometryShader;
//	if (m_computeShader)
//		delete m_computeShader;
//	return;
//}
//
////bool Aen::ParticleShaderComponent::SetShaderParameters(ParticleSystem& ps, float runtime, 
////	const DirectX::XMMATRIX& worldMatrix, const DirectX::XMMATRIX& viewMatrix,
////	const DirectX::XMMATRIX& projectionMatrix)
////{
////	HRESULT result;
////	D3D11_MAPPED_SUBRESOURCE mappedResource;
////	result = m_dContext.Get()->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
////	if (FAILED(result))
////	{
////		return false;
////	}
////	m_MatrixBufferType* dataPtr = (m_MatrixBufferType*)mappedResource.pData;
////
////	dataPtr->world = worldMatrix;
////	dataPtr->view = viewMatrix;
////	dataPtr->projection = projectionMatrix;
////	m_dContext.Get()->Unmap(m_matrixBuffer, 0);
////
////	
////	//____________________________ CS RUNTIME ____________________________//
////	CSInputBuffer inputData;
////	inputData.m_runtime = { runtime, 0.0f, 0.0f, 0.0f };
////	inputData.m_particleCount = { (float)ps.GetParticleCount(), 0.0f, 0.0f, 0.0f };
////	D3D11_MAPPED_SUBRESOURCE MS_DeltaTime;
////	m_dContext.Get()->Map(ps.GetConstantRunTimeBufferPtr(), 0, D3D11_MAP_WRITE_DISCARD, 0, &MS_DeltaTime);
////	CSInputBuffer* lInputDataPtr = (CSInputBuffer*)MS_DeltaTime.pData;
////	lInputDataPtr->m_runtime = inputData.m_runtime;
////	lInputDataPtr->m_particleCount = inputData.m_particleCount;
////	m_dContext.Get()->Unmap(ps.GetConstantRunTimeBufferPtr(), 0);
////	return true;
////}
////
////bool Aen::ParticleShaderComponent::CSBufferUpdate(ParticleSystem& ps,float runtime)
////{
////	CSInputBuffer inputData;
////	inputData.m_runtime = { runtime, 0.0f, 0.0f, 0.0f };
////	inputData.m_particleCount = { (float)ps.GetParticleCount(), 0.0f, 0.0f, 0.0f };
////	D3D11_MAPPED_SUBRESOURCE MS_DeltaTime;
////	m_dContext.Get()->Map(ps.GetConstantRunTimeBufferPtr(), 0, D3D11_MAP_WRITE_DISCARD, 0, &MS_DeltaTime);
////	CSInputBuffer* lInputDataPtr = (CSInputBuffer*)MS_DeltaTime.pData;
////	lInputDataPtr->m_runtime = inputData.m_runtime;
////	lInputDataPtr->m_particleCount = inputData.m_particleCount;
////	m_dContext.Get()->Unmap(ps.GetConstantRunTimeBufferPtr(), 0);
////	return true;
////}
//
//void Aen::ParticleShaderComponent::RenderShader()
//{
//	m_dContext.Get()->VSSetShader(this->m_vertexShader->m_shader.Get(), NULL, 0);
//	m_dContext.Get()->GSSetShader(this->m_geometryShader->m_shader.Get(), NULL, 0);
//	m_dContext.Get()->PSSetShader(this->m_pixelShader->m_shader.Get(),NULL,0);
//	return;
//}
//
//void Aen::ParticleShaderComponent::Draw(Renderer& renderer, const uint32_t& layer)
//{
//}
//
//void Aen::ParticleShaderComponent::DepthDraw(Renderer& renderer, const uint32_t& layer)
//{
//}
//
//bool Aen::ParticleShaderComponent::Render( 
//	const DirectX::XMMATRIX& worldMatrix, const DirectX::XMMATRIX& viewMatrix, 
//	const DirectX::XMMATRIX& projectionMatrix, float runtime, ParticleSystem& ps)
//{
//	SetShaderParameters(ps, ps.GetRunTime(), worldMatrix, viewMatrix, projectionMatrix);
//	m_dContext.Get()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
//	RenderShader();
//
//	//m_dContext.Get()->VSSetConstantBuffers(4, 1, &m_matrixBuffer);
//	//m_dContext.Get()->VSSetShaderResources(0, 1, ps.GetOutputSRV());
//	//m_dContext.Get()->GSSetConstantBuffers(0, 1, &m_matrixBuffer);
//	//m_dContext.Get()->PSSetSamplers(0, 1, &m_samplerState);
//	//m_dContext.Get()->PSSetShaderResources(0, 1, &texture);
//
//	//Borde stämma med antal vertiser, måste verfiera det senare
//	m_dContext.Get()->Draw(ps.GetParticleCount(), 0);
//	m_dContext.Get()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//
//	//Unbind
//	ID3D11ShaderResourceView* unboundSRV[] = { NULL };
//	m_dContext.Get()->VSSetShaderResources(0, 1, unboundSRV);
//	m_dContext.Get()->GSSetShader(NULL, NULL, 0);
//	return true;
//}
//
//void Aen::ParticleShaderComponent::UpdateComputeShader(ParticleSystem* particleSystem)
//{
//	m_dContext.Get()->CSSetShader(this->m_computeShader->m_shader.Get(),NULL,0);
//	m_dContext.Get()->CSSetConstantBuffers(0,1, particleSystem->GetConstantRunTimeBufferReference());
//	m_dContext.Get()->CSSetUnorderedAccessViews(0,1,particleSystem->GetOutputUAV(), NULL);
//	m_dContext.Get()->Dispatch((int)(particleSystem->GetMaxParticleCount() / 64.0f) + (particleSystem->GetMaxParticleCount() % 64 != 0), 1, 1);
//
//	//Unbinding
//	m_dContext.Get()->CSSetShader(NULL,NULL,0);
//	ID3D11UnorderedAccessView* unboundUAV[] = { NULL };
//	m_dContext.Get()->CSSetUnorderedAccessViews(0, 1, unboundUAV, NULL);
//}