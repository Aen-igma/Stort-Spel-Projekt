#include "PCH.h"
#include "PSSystemComponent.h"
#include "Core/Renderer.h"
#include"Importer/ObjImporter.h"

namespace Aen 
{
	Aen::PSSystemcomponent::PSSystemcomponent(const size_t& id)
		:Drawable(id),m_UAView(sizeof(Particle),1024)
	{
		//m_Particle.m_velocity = m_CSInputBuffer.m_vel.y;
		//m_Particle.m_pos.x = 10;
		//m_Particle.m_pos.y = 10;
		//m_Particle.m_pos.z = 10;
		m_ParticleList.m_velocity = m_CSInputBuffer.m_vel.y;
	}

	Aen::PSSystemcomponent::~PSSystemcomponent()
	{
	}

	void PSSystemcomponent::LoadAndSetTexture(const std::string& dir)
	{
		m_texture = &Aen::Resource::CreateTexture(GetNameFromPath(dir));
		m_texture->LoadTexture(dir);
	}

	void PSSystemcomponent::SetTexture(Texture& texture)
	{
		m_texture = &texture;
	}

	void PSSystemcomponent::SetTexture(const std::string& name)
	{
		if (!Aen::Resource::TextureExist(name)) throw;
		m_texture = &Aen::Resource::GetTexture(name);
	}

	void PSSystemcomponent::LoadAndSetMaterial(const std::string& dir)
	{
		m_material->CreateDefault();
		m_material->LoadeAndSetDiffuseMap(dir);
	}

	void PSSystemcomponent::SetMaterial(const std::string& name)
	{
	
	}

	void PSSystemcomponent::updatePS(const float& framerate)
	{
		m_CSInputBuffer.m_runtime = framerate;
	}

	void PSSystemcomponent::EmitRandom(const float& frameRate)
	{
		this->m_CSInputBuffer.m_runtime += frameRate;
		bool emitPS, found;
		int index, i, j;
	}

	void PSSystemcomponent::DeviationAndVelocity()
	{
		this->particleDeviationX = 100.0f;
		this->particleDeviationY = 10.0f;
		this->particleDeviationZ = 100.0f;
		this->particleVelocity = 10.0f;
		this->particleVelocityVariation = 1.0f;
	}

	void PSSystemcomponent::SetNrOfPS(UINT nr)
	{
		this->m_CSInputBuffer.m_particleCount = nr;
	}

	void PSSystemcomponent::SetPos(float x, float y, float z)
	{
		//m_Particle.m_pos.x = x;
		//m_Particle.m_pos.y = y;
		//m_Particle.m_pos.z = z;
	}

	void PSSystemcomponent::respawn(float x, float y, float z)
	{
		//if (this->m_Particle.m_pos.y >= 10)
		//{
		//	this->m_Particle.m_pos.x = x;
		//	this->m_Particle.m_pos.y = y;
		//	this->m_Particle.m_pos.z = z;
		//	this->m_Particle.m_color.x = 1;
		//	this->m_Particle.m_color.y = 1;
		//	this->m_Particle.m_color.z = 1;
		//	this->m_Particle.m_color.w = 1;
		//	this->m_Particle.m_uv.x = 0;
		//	this->m_Particle.m_uv.y = 0;
		//}
	}

	bool PSSystemcomponent::activate()
	{
		//int i;
		//this->maxParticles = 1024;
		//this->m_ParticleList = new Particle[maxParticles];
		//if (!m_ParticleList)
		//	return false;
		//for (int i = 0; i < maxParticles; i++)
		//	m_ParticleList[i].active = true;
		//return true;
		return false;
	}



	void PSSystemcomponent::SetVel(float x, float y, float z)
	{
		this->m_CSInputBuffer.m_vel.x = x;
		this->m_CSInputBuffer.m_vel.y = y;
		this->m_CSInputBuffer.m_vel.z = z;
	}

	//void PSSystemcomponent::EnableBlending()
	//{
	//	float blendFactor[4];
	//	blendFactor[0] = 0.0f;
	//	blendFactor[1] = 0.0f;
	//	blendFactor[2] = 0.0f;
	//	blendFactor[3] = 0.0f;
	//	m_CDevice->Get()->OMSetBlendState(alphaEnableBlendingState, blendFactor, 0xfffffff);
	//	return;
	//}
	//void PSSystemcomponent::DisableBlending()
	//{
	//	float blendFactor[4];
	//	blendFactor[0] = 0.0f;
	//	blendFactor[1] = 0.0f;
	//	blendFactor[2] = 0.0f;
	//	blendFactor[3] = 0.0f;
	//	m_CDevice->Get()->OMSetBlendState(alphaDisableBlendingState,blendFactor,0xfffffff);
	//	return;
	//}

	void PSSystemcomponent::Draw(Renderer& renderer, const uint32_t& layer)
	{
		//Binding everything
		//Here is dispatch and CS part

		renderer.m_PSInputBuffer.GetData() = m_CSInputBuffer;
		renderer.m_PSInputBuffer.UpdateBuffer();
		renderer.m_PSInputBuffer.BindBuffer<CShader>(0);
		RenderSystem::BindUnOrderedAccessView(0,m_UAView);
		RenderSystem::BindShader(renderer.m_PSCShader);
		RenderSystem::Dispatch(int(std::ceil(m_CSInputBuffer.m_particleCount / 64.f)), 1, 1);
	
	
		RenderSystem::UnBindShader<CShader>();
		RenderSystem::UnBindUnOrderedAccessViews(0,1);

		//Here is draw part of PS
		RenderSystem::SetPrimitiveTopology(Topology::POINTLIST);
		RenderSystem::SetInputLayout(renderer.m_PSLayout);
		renderer.m_cbTransform.GetData().m_mdlMat = EntityHandler::GetEntity(m_id).GetTransformation();
		renderer.m_cbTransform.UpdateBuffer();
		renderer.m_cbTransform.BindBuffer<VShader>(0);
	
		RenderSystem::BindShaderResourceView<VShader>(0, m_UAView);

		renderer.m_cbTransform.BindBuffer<GShader>(0);
		RenderSystem::BindShaderResourceView<PShader>(0,m_texture->m_shaderResource);
		RenderSystem::BindSamplers<PShader>(0,renderer.m_wrapSampler);

		RenderSystem::BindRenderTargetView(renderer.m_backBuffer);
		RenderSystem::SetRasteriserState(renderer.m_rasterizerState);

		RenderSystem::BindShader(renderer.m_PSVShader);
		RenderSystem::BindShader(renderer.m_PSGShader);
		RenderSystem::BindShader(renderer.m_PSPShader);

		RenderSystem::Draw(m_CSInputBuffer.m_particleCount,0);


		RenderSystem::UnBindShader<VShader>();
		RenderSystem::UnBindShader<GShader>();
		RenderSystem::UnBindShader<PShader>();
		RenderSystem::UnBindShaderResources<VShader>(0,1);

	}


	void PSSystemcomponent::DepthDraw(Renderer& renderer, const uint32_t& layer)
	{
		
	}
}
