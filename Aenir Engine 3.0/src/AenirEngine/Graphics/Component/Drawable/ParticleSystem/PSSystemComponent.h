#pragma once
#include "..\Drawable.h"
#include "..\Mesh\Material.h"
#include "..\Mesh\Mesh.h"


namespace Aen 
{
	struct VertexParticle
	{
		Vec3f Pos;
		float Alive;
		Vec3f Velocity;
		float Age;
		Color Color;
		Vec2f UV;
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

		//Material
		void SetMaterial(Material& material);
		void SetMaterial(const std::string& materialName);




	private:

		//Friend class 
		friend class ComponentHandler;
		friend class Renderer;
		friend class Entity;
		friend class GCore;

		void updatePS(const float& framerate);

		//Things we need, used for render an handeling constant buffern
		CSInputBuffer m_CSInputBuffer;
		VertexParticle m_VertexPS;
		ShaderModel* m_shader;
		UAView m_UAView;
		Material* m_pMaterial;

		int m_maxParticles, m_currentNrPS, m_vertexCount;
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