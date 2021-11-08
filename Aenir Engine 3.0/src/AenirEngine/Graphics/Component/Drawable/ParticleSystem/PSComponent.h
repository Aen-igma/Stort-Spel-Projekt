#pragma once
#include "AenCore.h"
#include "AenGraphics.h"

struct m_Particle
{
	float m_pos[3];
	float m_velocity;
	float m_color[4];
	float m_uv[2];
	bool m_active;
};

struct CSInputBuffer
{
	DirectX::XMFLOAT4 m_runtime;
	DirectX::XMFLOAT4 m_particleCount;
};

namespace Aen
{
	class ParticleSystemComponent
	{
	public:
		ParticleSystemComponent();
		virtual ~ParticleSystemComponent();

		bool Initialize(ID3D11Device*& device, std::wstring textureFilename);
		void Shutdown();
		bool Frame(float frameTime, ID3D11DeviceContext*& deviceContext);
		void Render(ID3D11DeviceContext*& deviceContext);
		void Move(float x, float y, float z);
		int GetVertexCount();
		int GetParticleCount();
		int GetMaxParticleCount();
		float GetRunTime();
		bool UpdateBuffers(ID3D11DeviceContext*& deviceContext);


		ID3D11Buffer* GetConstantRunTimeBufferPtr()const;
		ID3D11Buffer* const* GetConstantRunTimeBufferReference()const;
		ID3D11ShaderResourceView* const* GetOutputSRV() const;
		ID3D11UnorderedAccessView* const* GetOutputUAV() const;
	protected:

	private:
		float m_particleDeviationX, m_particleDeviationY, m_particleDeviationZ;
		float m_particleVelocity, m_particleVelocityVariation;
		float m_particleSize, m_particlesPerSecond;
		int m_maxParticles;
		int m_currentParticleCount;
		float m_accumulatedTime;
		float m_runTime;
		DirectX::XMFLOAT3 m_systemPos;
		m_Particle* m_particleList;
		int m_vertexCount, m_indexCount;
		ID3D11Buffer* m_vertexBuffer, * m_SRVBuffer;
		ID3D11Buffer* cRuntimeBuffer;
		ID3D11Buffer* m_directOutputBuffer;
		ID3D11ShaderResourceView* m_outputSRV;
		ID3D11UnorderedAccessView* m_UAV;

		bool LoadTexture(ID3D11Device*& device, std::wstring filename);
		bool InitializeParticleSystem();
		void ShutdownParticleSystem();
		bool InitializeBuffers(ID3D11Device*& device);
		void ShutdownBuffers();
		void EmitParticles(float frameTime);
		void UpdateParticles(float frameTime);
		void RenderBuffers(ID3D11DeviceContext*& deviceContext);
	};

}