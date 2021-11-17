//#pragma once
//#include "ParticleSystem.h"
//
//namespace Aen
//{
//	class ParticleShaderComponent: public Drawable, GCore
//	{
//	public:
//		ParticleShaderComponent(const size_t& id);
//		virtual ~ParticleShaderComponent();
//		void UpdateComputeShader(ParticleSystem* ps);
//		bool Initialize();
//		void Shutdown();
//		bool Render(const DirectX::XMMATRIX& worldMatrix, const DirectX::XMMATRIX& viewMatrix,
//			const DirectX::XMMATRIX& projectionMatrix, float runtime, ParticleSystem& ps);
//		bool CSBufferUpdate(ParticleSystem& ps, float runtime);
//	protected:
//
//	private:
//		struct m_MatrixBufferType
//		{
//			DirectX::XMMATRIX world = DirectX::XMMatrixIdentity();
//			DirectX::XMMATRIX view = DirectX::XMMatrixIdentity();
//			DirectX::XMMATRIX projection = DirectX::XMMatrixIdentity();
//		};
//		//ID3D11VertexShader* m_vertexShader;
//		//ID3D11PixelShader* m_pixelShader;
//		//ID3D11GeometryShader* m_geometryShader;
//		//ID3D11ComputeShader* m_computeShader;
//		
//		ID3D11InputLayout* m_layout;
//		ID3D11Buffer* m_matrixBuffer;
//		ID3D11SamplerState* m_samplerState;
//		VShader* m_vertexShader;
//		GShader* m_geometryShader;
//		CShader* m_computeShader;
//		PShader* m_pixelShader;
//		
//
//		bool InitializeShader();
//		void ShutdownShader();
//		bool SetShaderParameters(ParticleSystem& ps,
//			float runtime, const DirectX::XMMATRIX& worldMatrix, const DirectX::XMMATRIX& viewMatrix,
//			const DirectX::XMMATRIX& projectionMatrix);
//
//
//		void RenderShader();
//
//	;
//
//
//
//		// Inherited via Drawable
//		virtual void Draw(Renderer& renderer, const uint32_t& layer) override;
//
//		virtual void DepthDraw(Renderer& renderer, const uint32_t& layer) override;
//
//	};
//}