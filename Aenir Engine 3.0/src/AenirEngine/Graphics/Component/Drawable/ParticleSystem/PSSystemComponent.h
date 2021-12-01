#pragma once
#include "..\Drawable.h"
#include "..\Mesh\Material.h"
#include "..\Mesh\Mesh.h"


namespace Aen 
{
	struct VertexParticle
	{
		Vec3f m_Pos;
		float Alive;
		Vec3f m_Velocity;
		float Age;
		Color m_Color;
		Vec2f m_UV;
		Vec2f padding;
	};
	//struct CSInputBuffer
	//{
	//	float m_runtime;
	//	UINT m_particleCount;
	//	double padding;
	//	Vec3f m_vel;
	//	float padding2;
	//};

	struct CSInputBuffer
	{
		Vec3f velocity;
		float lifeTime;
		Vec3f acceleration;
		float deltaTime;
		Vec3f initalPos;
		float emitInterval;
		int maxParticles;
		int emitCount;

	};

	class AEN_DECLSPEC PSSystemcomponent : public Drawable
	{
	public:
		PSSystemcomponent(const size_t& id);
		virtual ~PSSystemcomponent();

		//Member vairables that we dont need yet
		void SetNrOfPS(UINT nr);
		void SetPos(float x, float y, float z);
		void SetVelocity(float x,float y,float z);
		void EmitRandom(float frameTime);
		bool activate();
		void SetEmitPos(float x, float y, float z);
		void Initialize();
		void SetRespawnHeight(float height);
		

		void Reset();

		//Texture
		void LoadAndSetTexture(const std::string& dir);
		void SetTexture(Texture& texture);
		void SetTexture(const std::string& name);

		//Material
		void SetMaterial(Material& material);
		void SetMaterial(const std::string& materialName);
		void SetMaterial(const std::string& materialSlotName, Material& material);
		void SetMaterial(const std::string& materialSlotName, const std::string& materialName);
		void SetMaterial(const UINT& index, Material& material);

		//Friend class 
		friend class ComponentHandler;
		friend class Renderer;
		friend class Entity;
		friend class GCore;



	private:


		friend class ComponentHandler;
		friend class Renderer;
		friend class Entity;
		friend class GCore;


	
		//Particle* m_ParticleList;
		CSInputBuffer m_CSInputBuffer;
		VertexParticle m_VertexPS;
		Texture* m_texture;
		VBuffer<VertexParticle>* m_pVbuffer;
		ShaderModel* m_shader;
		UAView m_UAView;

		Mesh* m_pMesh;
		Material* m_pMaterial;

		void updatePS(const float& framerate);
		
		void DeviationAndVelocity();
		float particleDeviationX, particleDeviationY, particleDeviationZ;
		float particleVelocity, particleVelocityVariation;


		ID3D11BlendState* alphaEnableBlendingState = NULL;
		ID3D11BlendState* alphaDisableBlendingState = NULL;

		int maxParticles, currentNrPS;
		float accumulatedTime, runTimes;
		float particleSize, particlesPerSecond;
		Vec3f m_emitPos;
		Vec3f m_emitDir;
		int m_vertexCount;

		UINT m_MaxParticles;
		bool m_firstRun;
		float m_GameTime;
		float m_TimeStep;
		float m_Age;
		Vec3f m_EyePosW;
		Vec3f m_EmitPosW;
		Vec3f m_EmitDirW;


		
		
		// Inherited via Drawable
		virtual void Draw(Renderer& renderer, const uint32_t& layer) override;
		virtual void DepthDraw(Renderer& renderer, const uint32_t& layer) override;
		virtual bool FrustumCull(Renderer& renderer) override;
	};
}