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

	bool Aen::PSSystemcomponent::Initialize(ComDevice*& device ,std::string fileName)
	{
		if (!m_PSComponent->Initialize(device, fileName))
			throw;
		if (!m_PSShader->Initialize(device))
			throw;

	}

	//void Aen::PSSystemcomponent::RenderParticlesBuffer(ComDeviceContext*& deviceContext)
	//{
	//
	//}

	void Aen::PSSystemcomponent::RenderFrame(float frameTime, ComDeviceContext*& deviceContext)
	{
		m_PSComponent->Frame(frameTime,deviceContext);
	
	}

	//void Aen::PSSystemcomponent::RenderParticlesBuffer(ComDeviceContext*& deviceContext)
	//{
	//	m_PSComponent.Render(deviceContext);
	//}

	void Aen::PSSystemcomponent::UpdateCSShader(ComDeviceContext*& deviceContext)
	{
	//m_PSShader->UpdateComputeShader(deviceContext,m_PSComponent);
	//m_PSComponent->UpdateBuffers(deviceContext);
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
