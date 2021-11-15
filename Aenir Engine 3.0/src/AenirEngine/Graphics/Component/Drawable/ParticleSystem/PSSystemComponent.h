#pragma once
#include "ParticleShader.h"

namespace Aen 
{
	class  AEN_DECLSPEC PSSystemcomponent : public Drawable, GCore
	{
	public:
		PSSystemcomponent(const size_t& id);
		virtual ~PSSystemcomponent();

		//För att inita allting
		bool Initialize(std::string fileName);
		void RenderFrame(float frameTime);
		void RenderParticlesBuffer();
		void UpdateCSShader();

		std::vector<PSSystemcomponent> GetData()const;

		//This will change on the CPU???
		void Emit();

		friend class ComponentHandler;
		friend class Renderer;
		friend class Entity;
		friend class GCore;

	protected:

	private:
		ParticleShaderComponent* m_PSShader;
		ParticleSystem* m_PSComponent;
		/*ComDevice* m_Device;*/
		/*ComDevice* m_Device;*/
		friend class ComponentHandler;
		friend class Renderer;
		friend class Entity;
		friend class GCore;

		
		std::vector<PSSystemcomponent> PSComponentData;
		std::vector<ParticleShaderComponent> m_PSShaderData;
		std::vector<ParticleSystem> m_PSSystemData;
		// Inherited via Drawable
		virtual void Draw(Renderer& renderer, const uint32_t& layer) override;
		virtual void DepthDraw(Renderer& renderer, const uint32_t& layer) override;
	};

}