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

		//Member vairables that we dont need yet
		void SetNrOfPS(UINT nr);
		void SetVelocity(float x,float y,float z);
		void EmitRandom(float frameTime);
		bool activate();
		void SetEmitPos(float x, float y, float z);
		void Initialize();
		void SetHeightLimit(float height);

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

		//Things we need, used for render an handeling constant buffern
		CSInputBuffer m_CSInputBuffer;
		VertexParticle m_VertexPS;
		Texture* m_texture;
		ShaderModel* m_shader;
		UAView m_UAView;
		Mesh* m_pMesh;
		Material* m_pMaterial;

		void updatePS(const float& framerate);

		int m_maxParticles, m_currentNrPS;
		float m_accumulatedTime, m_runTimes;
		float m_particleSize, m_particlesPerSecond;
		Vec3f m_emitPos;
		Vec3f m_emitDir;
		int m_vertexCount;
		
		// Inherited via Drawable
		virtual void Draw(Renderer& renderer, const uint32_t& layer) override;
		virtual void DepthDraw(Renderer& renderer, const uint32_t& layer) override;
		virtual bool FrustumCull(Renderer& renderer) override;
	};
}