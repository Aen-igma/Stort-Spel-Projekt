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

		void SetNrOfPS(UINT nr);
		void SetVelocity(float x,float y,float z);
		void SetAcceleration(float x, float y, float z);
		void SetEmitPos(float x, float y, float z);
		void SetEmitInterval(float xyz);
		void SetMaxParticles(int max);
		void Initialize();
		void SetHeightLimit(float height);
		bool activate();

		//Material
		void SetMaterial(Material& material);
		void SetMaterial(const std::string& materialName);

	private:


		friend class ComponentHandler;
		friend class Renderer;
		friend class Entity;
		friend class GCore;

		CSInputBuffer m_CSInputBuffer;
		VertexParticle m_VertexPS;
		UAView m_UAView;
		Material* m_pMaterial;

		void updatePS(const float& framerate);

		int m_maxParticles, m_currentNrPS;
		int m_vertexCount;
		float m_accumulatedTime, m_runTimes;
		float m_particleSize, m_particlesPerSecond;
		Vec3f m_emitPos;
		Vec3f m_emitDir;

		
		// Inherited via Drawable
		virtual void Draw(Renderer& renderer, const uint32_t& layer) override;
		virtual void DepthDraw(Renderer& renderer, const uint32_t& layer) override;
		virtual bool FrustumCull(Renderer& renderer) override;
	};
}