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

		//Material and texture
		void LoadAndSetTexture(const std::string& dir);
		void SetTexture(Texture& texture);
		void SetTexture(const std::string& name);
		void LoadAndSetMaterial(const std::string& dir);
		void SetMaterial(const std::string& name);

		//Member vairables that we dont need yet
		//void SetVelocity(float velocity);
		//void SetParticleCount(int nrOfPS);
		//void SetPos(Vec3f pos);

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
	

		CSInputBuffer m_CSInputBuffer;
		Texture* m_texture;
		Material* m_material;
		ShaderModel* m_shader;
		UAView m_UAView;

		void updatePS(const float& framerate);


		// Inherited via Drawable
		virtual void Draw(Renderer& renderer, const uint32_t& layer) override;
		virtual void DepthDraw(Renderer& renderer, const uint32_t& layer) override;
	};

}