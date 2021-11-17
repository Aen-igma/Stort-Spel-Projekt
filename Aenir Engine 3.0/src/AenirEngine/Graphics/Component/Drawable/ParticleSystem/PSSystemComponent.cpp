#include "PCH.h"
#include "PSSystemComponent.h"
#include "Core/Renderer.h"
#include"Importer/ObjImporter.h"

namespace Aen 
{
	Aen::PSSystemcomponent::PSSystemcomponent(const size_t& id)
		:Drawable(id),m_UAView(sizeof(Particle),1024)
	{
		m_CSInputBuffer.m_particleCount = 100;
		m_CSInputBuffer.m_vel.x = 1;
		m_CSInputBuffer.m_vel.y = 2;
		m_CSInputBuffer.m_vel.z = 1;
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

	void PSSystemcomponent::Draw(Renderer& renderer, const uint32_t& layer)
	{
		//Binding everything
		//Here is dispatch and CS part
		renderer.m_PSInputBuffer.GetData() = m_CSInputBuffer;
		renderer.m_PSInputBuffer.UpdateBuffer();
		renderer.m_PSInputBuffer.BindBuffer<CShader>(0);
		RenderSystem::BindUnOrderedAccessView(0,m_UAView);
		RenderSystem::BindShader(renderer.m_PSCShader);

		RenderSystem::Dispatch(16,1,1);

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
