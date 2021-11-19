#pragma once
#include "..\Drawable.h"
#include "..\Mesh\Material.h"


namespace Aen 
{
	struct Particle
	{
		Vec3f m_pos;
		float m_velocity;
		Color m_color;
		Vec2f m_uv;
	};
	struct CSInputBuffer
	{
		float m_runtime;
		UINT m_particleCount;
		double padding;
		Vec3f m_vel;
		float padding2;
	};

	class AEN_DECLSPEC PSSystemcomponent : public Drawable
	{
	public:
		PSSystemcomponent(const size_t& id);
		virtual ~PSSystemcomponent();

		//Member vairables that we dont need yet
		void SetVelo(float x);
		void SetVel(float dir);
		void SetNrOfPS(UINT nr);
		void SetPos(float x, float y, float z);
		void respawn(float x, float y, float z);


		void EmitRandom(float frameTime);
		void InitParticleVariables();
		bool activate();
		//void EnableBlending();
		//void DisableBlending();
		

		//Material and texture
		void LoadAndSetTexture(const std::string& dir);
		void SetTexture(Texture& texture);
		void SetTexture(const std::string& name);
		void LoadAndSetMaterial(const std::string& dir);
		void SetMaterial(Material& material);
		void SetMaterial(const std::string& name);
		//void SetMaterial(Material& material);
		//void SetMaterial(const std::string& materialName);
		//void SetMaterial(const std::string& materialSlotName, Material& material);
		//void SetMaterial(const std::string& materialSlotName, const std::string& materialName);
		//void SetMaterial(const UINT& index, Material& material);

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
	
		Particle* m_ParticleList;
		CSInputBuffer m_CSInputBuffer;
		Texture* m_texture;
		Material* m_material;
		ShaderModel* m_shader;
		UAView m_UAView;

		void updatePS(const float& framerate);
		
		void DeviationAndVelocity();
		float particleDeviationX, particleDeviationY, particleDeviationZ;
		float particleVelocity, particleVelocityVariation;


		ID3D11BlendState* alphaEnableBlendingState = NULL;
		ID3D11BlendState* alphaDisableBlendingState = NULL;

		int maxParticles, currentNrPS;
		float accumulatedTime, runTimes;
		float particleSize, particlesPerSecond;
		// Inherited via Drawable
		virtual void Draw(Renderer& renderer, const uint32_t& layer) override;
		virtual void DepthDraw(Renderer& renderer, const uint32_t& layer) override;
	};

}