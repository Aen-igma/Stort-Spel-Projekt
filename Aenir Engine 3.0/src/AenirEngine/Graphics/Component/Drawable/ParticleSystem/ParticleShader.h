#pragma once
#include "PSComponent.h"
namespace Aen
{
	class ParticleShaderComponent
	{
	public:
		ParticleShaderComponent();
		virtual ~ParticleShaderComponent();
		void UpdateComputeShader(ComDeviceContext*& deviceContext, ParticleSystem& ps);
		bool Initialize(ComDevice*& device);
		void Shutdown();
		bool Render(ComDeviceContext*& deviceContext, const DirectX::XMMATRIX& worldMatrix, const DirectX::XMMATRIX& viewMatrix,
			const DirectX::XMMATRIX& projectionMatrix, float runtime, ID3D11ShaderResourceView*& texture, ParticleSystem& ps);

	protected:

	private:
		struct m_MatrixBufferType
		{
			DirectX::XMMATRIX world = DirectX::XMMatrixIdentity();
			DirectX::XMMATRIX view = DirectX::XMMatrixIdentity();
			DirectX::XMMATRIX projection = DirectX::XMMatrixIdentity();
		};
		//ID3D11VertexShader* m_vertexShader;
		//ID3D11PixelShader* m_pixelShader;
		//ID3D11GeometryShader* m_geometryShader;
		//ID3D11ComputeShader* m_computeShader;
		ID3D11InputLayout* m_layout;
		ID3D11Buffer* m_matrixBuffer;
		ID3D11SamplerState* m_samplerState;
		VShader* m_vertexShader;
		GShader* m_geometryShader;
		CShader* m_computeShader;
		PShader* m_pixelShader;
		
		bool InitializeShader(ComDevice*& device);
		void ShutdownShader();
		void OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename);
		bool SetShaderParameters(ComDeviceContext*& deviceContext, ParticleSystem& ps,
			float runtime, const DirectX::XMMATRIX& worldMatrix, const DirectX::XMMATRIX& viewMatrix,
			const DirectX::XMMATRIX& projectionMatrix, ID3D11ShaderResourceView*& texture);
		void RenderShader(ComDeviceContext*& deviceContext);


	};
}