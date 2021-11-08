#pragma once
#include "Graphics/Component/Drawable/ParticleSystem/PSComponent.h"


namespace Aen
{
	class ParticleShaderComponent
	{
	public:
		ParticleShaderComponent();
		ParticleShaderComponent(const ParticleShaderComponent& other);
		virtual ~ParticleShaderComponent();
		void UpdateComputeShader(ComDeviceContext*& deviceContext, ParticleSystemComponent& particleSystem);

		bool Initialize(ComDevice*& device);
		void Shutdown();
		bool Render(ComDeviceContext*& deviceContext, const DirectX::XMMATRIX& worldMatrix, const DirectX::XMMATRIX& viewMatrix,
			const DirectX::XMMATRIX& projectionMatrix, float runtime, ID3D11ShaderResourceView*& texture, ParticleSystemComponent& particleSystem);

	protected:

	private:
		struct m_MatrixBufferType
		{
			DirectX::XMMATRIX world = DirectX::XMMatrixIdentity();
			DirectX::XMMATRIX view = DirectX::XMMatrixIdentity();
			DirectX::XMMATRIX projection = DirectX::XMMatrixIdentity();
		};
		ID3D11VertexShader* m_vertexShader;
		ID3D11PixelShader* m_pixelShader;
		ID3D11GeometryShader* m_geometryShader;
		ID3D11ComputeShader* m_computeShader;
		ID3D11InputLayout* m_layout;
		ID3D11Buffer* m_matrixBuffer;
		ID3D11SamplerState* m_samplerState;

		bool InitializeShader(ComDevice*& device);
		void ShutdownShader();
		void OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename);
		bool SetShaderParameters(ComDeviceContext*& deviceContext, ParticleSystemComponent& particleSystem, float runtime, const DirectX::XMMATRIX& worldMatrix, const DirectX::XMMATRIX& viewMatrix,
			const DirectX::XMMATRIX& projectionMatrix, ID3D11ShaderResourceView*& texture);
		void RenderShader(ComDeviceContext*& deviceContext);


	};
}