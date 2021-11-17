//#pragma once
//#include "..\Drawable.h"
//#include "..\Mesh\Texture.h"
////#include "Graphics/Component/Resource.h"
//namespace Aen
//{
//	struct m_Particle
//	{
//		float m_pos[3];
//		float m_velocity;
//		float m_color[4];
//		float m_uv[2];
//		bool m_active;
//	};
//	
//	class ParticleSystem
//	{
//	public:
//		ParticleSystem();
//		~ParticleSystem();
//		
//		bool Initialize(std::string textureFilename);
//		void Shutdown();
//		bool Emit(float frameTime);
//		void Render();
//		void Move(float x, float y, float z);
//		int GetVertexCount();
//		int GetParticleCount();
//		int GetMaxParticleCount();
//		float GetRunTime();
//		bool UpdateBuffers();
//
//		ID3D11Buffer* GetConstantRunTimeBufferPtr()const;
//		ID3D11Buffer* const* GetConstantRunTimeBufferReference()const;
//		ID3D11ShaderResourceView* const* GetOutputSRV() const;
//		ID3D11UnorderedAccessView* const* GetOutputUAV() const;
//		ID3D11ShaderResourceView* GetTexture();
//
//
//	protected:
//
//	private:
//		int m_maxParticles;
//		int m_currentParticleCount;
//		int m_vertexCount, m_indexCount;
//
//		float m_particleDeviationX, m_particleDeviationY, m_particleDeviationZ;
//		float m_particleVelocity, m_particleVelocityVariation;
//		float m_particleSize, m_particlesPerSecond;
//		float m_accumulatedTime;
//		float m_runTime;
//
//		DirectX::XMFLOAT3 m_systemPos;
//		m_Particle* m_particleList;
//		ID3D11Buffer* m_vertexBuffer, * m_SRVBuffer;
//		ID3D11Buffer* cRuntimeBuffer;
//		ID3D11Buffer* m_directOutputBuffer;
//		ID3D11ShaderResourceView* m_outputSRV;
//		ID3D11UnorderedAccessView* m_UAV;
//
//		
//		
//
//
//
//
//		void Draw(m_Particle& PSdata);
//
//		//Texture
//		Texture* m_Texture;
//		bool LoadTexture(std::string fileName);
//		void ReleaseTexture();
//
//
//		bool InitializeParticleSystem();
//		void ShutdownParticleSystem();
//		bool InitializeBuffers();
//		void ShutdownBuffers();
//		void EmitParticles(float frameTime);
//		void UpdateParticles(float frameTime);
//		void RenderBuffers();
//
//
//	};
//
//}