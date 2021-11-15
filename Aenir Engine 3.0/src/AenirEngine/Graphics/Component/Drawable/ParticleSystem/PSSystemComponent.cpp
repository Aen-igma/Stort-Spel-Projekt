#include "PCH.h"
#include "PSSystemComponent.h"
#include "Core/Renderer.h"

namespace Aen 
{
	Aen::PSSystemcomponent::PSSystemcomponent(const size_t& id)
		:Drawable(id)
	{

	}

	Aen::PSSystemcomponent::~PSSystemcomponent()
	{

	}

	bool Aen::PSSystemcomponent::Initialize(std::string fileName)
	{
		if (!m_PSComponent->Initialize(fileName))
			throw;
		if (!m_PSShader->Initialize())
			throw;

	}

	//void Aen::PSSystemcomponent::RenderParticlesBuffer(ComDeviceContext*& deviceContext)
	//{
	//
	//}

	void Aen::PSSystemcomponent::RenderFrame(float frameTime)
	{
		m_PSComponent->Frame(frameTime);
	
	}

	//void Aen::PSSystemcomponent::RenderParticlesBuffer(ComDeviceContext*& deviceContext)
	//{
	//	m_PSComponent.Render(deviceContext);
	//}

	void Aen::PSSystemcomponent::UpdateCSShader()
	{
		/*m_PSShader->UpdateComputeShader();*/
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
		//for (auto &a : )
		//{

		//}
	}
	void PSSystemcomponent::DepthDraw(Renderer& renderer, const uint32_t& layer)
	{
		
	}
}
