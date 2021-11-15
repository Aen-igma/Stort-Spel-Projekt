#include "PCH.h"
#include "PSSystemComponent.h"
#include "Core/Renderer.h"

namespace Aen 
{
	Aen::PSSystemcomponent::PSSystemcomponent(const size_t& id)
		:Drawable(id)
	{
		m_PSShader = new ParticleShaderComponent(id);
		m_PSComponent = new ParticleSystem(id);
	}

	Aen::PSSystemcomponent::~PSSystemcomponent()
	{

	}

	bool Aen::PSSystemcomponent::Initialize(std::string fileName)
	{
		bool suc = true;
		if (!m_PSShader->Initialize())
			suc = false;
		if (!m_PSComponent->Initialize(fileName))
			suc = false;
		return false;
	}

	void Aen::PSSystemcomponent::RenderFrame(float frameTime)
	{
		m_PSComponent->Frame(frameTime);
	}

	void Aen::PSSystemcomponent::RenderParticlesBuffer()
	{
		m_PSComponent->Render();
	}

	void Aen::PSSystemcomponent::UpdateCSShader()
	{
		m_PSShader->UpdateComputeShader(m_PSComponent);
		m_PSComponent->UpdateBuffers();
	}

	std::vector<PSSystemcomponent> PSSystemcomponent::GetData() const
	{
		return this->PSComponentData;
	}

	void Aen::PSSystemcomponent::Emit()
	{

	}

	void PSSystemcomponent::Draw(Renderer& renderer, const uint32_t& layer)
	{
		for (auto &a : GetData())
		{

		}
		for (auto &b : GetData())
		{

		}
	}
	void PSSystemcomponent::DepthDraw(Renderer& renderer, const uint32_t& layer)
	{
		
	}
}
