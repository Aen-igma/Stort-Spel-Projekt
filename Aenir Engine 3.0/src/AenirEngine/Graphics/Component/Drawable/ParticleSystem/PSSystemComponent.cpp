#include "PCH.h"
#include "PSSystemComponent.h"
#include "Core/Renderer.h"
#include"Importer/ObjImporter.h"
#include "../Mesh/MeshInstance.h"
#include"Core/GlobalSettings.h"


namespace Aen 
{
	Aen::PSSystemcomponent::PSSystemcomponent(const size_t& id)
		:Drawable(id),m_UAView(sizeof(VertexParticle),1024) {
		m_pMaterial = 0;	
	}

	Aen::PSSystemcomponent::~PSSystemcomponent() {
	}


	void PSSystemcomponent::SetMaterial(Material& material)
	{
		m_pMaterial = &material;
	}

	void PSSystemcomponent::SetMaterial(const std::string& materialName)
	{
		if (!Resource::MaterialExist(materialName))throw;
		m_pMaterial = &Resource::GetMaterial(materialName);
	}

	void PSSystemcomponent::updatePS(const float& framerate)
	{
		m_CSInputBuffer.deltaTime = framerate;
	}

	void PSSystemcomponent::SetNrOfPS(UINT nr)
	{
		this->m_CSInputBuffer.emitCount = nr;
		this->m_currentNrPS = this->m_CSInputBuffer.emitCount;
	}

	void PSSystemcomponent::SetVelocity(float x, float y, float z)
	{
		this->m_CSInputBuffer.velocity.x = x;
		this->m_CSInputBuffer.velocity.y = y;
		this->m_CSInputBuffer.velocity.z = z;
	}

	void PSSystemcomponent::SetAcceleration(float x, float y, float z)
	{
		this->m_CSInputBuffer.acceleration.x = x;
		this->m_CSInputBuffer.acceleration.y = y;
		this->m_CSInputBuffer.acceleration.z = z;
	}

	bool PSSystemcomponent::activate()
	{
		return true;
	}

	void PSSystemcomponent::SetEmitPos(float x, float y, float z)
	{
		this->m_CSInputBuffer.initalPos.x = x;
		this->m_CSInputBuffer.initalPos.y = y;
		this->m_CSInputBuffer.initalPos.z = z;
	}

	void PSSystemcomponent::SetEmitInterval(float xyz)
	{
		this->m_CSInputBuffer.emitInterval = xyz;
	}

	void PSSystemcomponent::SetMaxParticles(int max)
	{
		this->m_maxParticles = max;
	}

	void PSSystemcomponent::Initialize()
	{
		this->m_CSInputBuffer.maxParticles = 1024;
		this->m_CSInputBuffer.acceleration = {0.0f, 7.8f, 0.0f};
		this->m_CSInputBuffer.lifeTime = 0.0f;
		this->m_CSInputBuffer.deltaTime = 0.0f;
		this->m_CSInputBuffer.velocity = { 0.0f,0.0f,0.0f };
		this->m_vertexCount = 0;
		this->m_particlesPerSecond = 1.0f;
	}
	void PSSystemcomponent::SetHeightLimit(float height)
	{
		this->m_CSInputBuffer.lifeTime = height;
	}
	void PSSystemcomponent::Draw(Renderer& renderer, const uint32_t& layer) {
		

		//Dispatch from depth
		renderer.m_PSInputBuffer.GetData() = m_CSInputBuffer;
		renderer.m_PSInputBuffer.UpdateBuffer();
		renderer.m_PSInputBuffer.BindBuffer<CShader>(0);
		RenderSystem::BindUnOrderedAccessView(0, m_UAView);
		RenderSystem::BindShader(renderer.m_PSCShader);

		RenderSystem::Dispatch(16, 1, 1);

		RenderSystem::UnBindShader<CShader>();
		RenderSystem::UnBindUnOrderedAccessViews(0, 1);

		//First Pass
		RenderSystem::ClearRenderTargetView(renderer.m_particleOut, Color(0.f, 0.f, 0.f, 0.f));
		RenderSystem::SetPrimitiveTopology(Topology::POINTLIST);
		RenderSystem::SetInputLayout(renderer.m_PSLayout);
		renderer.m_cbTransform.GetData().m_mdlMat = EntityHandler::GetEntity(m_id).GetTransformation();
		renderer.m_cbTransform.UpdateBuffer();
		renderer.m_cbTransform.BindBuffer<VShader>(0);

		RenderSystem::BindShaderResourceView<VShader>(0, m_UAView);

		renderer.m_cbTransform.BindBuffer<GShader>(0);
		RenderSystem::BindSamplers<PShader>(0,renderer.m_wrapSampler);

		if(m_pMaterial) {

			RenderSystem::UnBindShaderResources<PShader>(0u, 3u);
			if(m_pMaterial->m_textures[0]) {
				RenderSystem::BindShaderResourceView<PShader>(0, m_pMaterial->m_textures[0]->m_shaderResource);
				renderer.m_cbUseTexture.GetData()[0] = (int)true;
			}

			if(m_pMaterial->m_textures[2]) {
				RenderSystem::BindShaderResourceView<PShader>(1, m_pMaterial->m_textures[2]->m_shaderResource);
				renderer.m_cbUseTexture.GetData()[2] = (int)true;
			}

			if(m_pMaterial->m_textures[3]) {
				RenderSystem::BindShaderResourceView<PShader>(2, m_pMaterial->m_textures[3]->m_shaderResource);
				renderer.m_cbUseTexture.GetData()[3] = (int)true;
			}

			renderer.m_cbUseTexture.UpdateBuffer();
		}

		renderer.m_cbUseTexture.BindBuffer<PShader>(0u);

		RenderSystem::BindRenderTargetView(renderer.m_particleOut, renderer.m_depthMap);
		//RenderSystem::SetRasteriserState(renderer.m_rasterizerState);
		
		//Uncomment if you want wireframe state
		//RenderSystem::SetRasteriserState(renderer.m_wireFrameState);


		RenderSystem::BindShader(renderer.m_PSVShader);
		RenderSystem::BindShader(renderer.m_PSGShader);
		RenderSystem::BindShader(renderer.m_PSPShader);

		RenderSystem::Draw(this->m_currentNrPS,0);


		// Second Pass
		RenderSystem::UnBindShader<VShader>();
		RenderSystem::UnBindShader<GShader>();
		RenderSystem::UnBindShader<PShader>();
		RenderSystem::UnBindShaderResources<VShader>(0,1);
		RenderSystem::UnBindShaderResources<PShader>(0,3);
		RenderSystem::UnBindRenderTargets(renderer.m_particleOut.GetCount());

		renderer.m_cbBGColor.BindBuffer<CShader>(0);
		RenderSystem::BindShader<CShader>(renderer.m_PostPatricleCS);
		RenderSystem::BindUnOrderedAccessView(0u, renderer.m_UAVBackBuffer);
		RenderSystem::BindShaderResourceView<CShader>(0u, renderer.m_particleOut);

		RenderSystem::Dispatch(renderer.m_dispatchGroups, 1u);

		RenderSystem::UnBindShader<CShader>();
		RenderSystem::UnBindUnOrderedAccessViews(0u, 1u);
		RenderSystem::UnBindShaderResources<CShader>(0u, renderer.m_particleOut.GetCount());
	}

	void PSSystemcomponent::DepthDraw(Renderer& renderer, const uint32_t& layer) {
	}

	bool PSSystemcomponent::FrustumCull(Renderer& renderer) 
	{		
		if (GlobalSettings::GetMainCamera())
		{
			DirectX::XMFLOAT3 tempFloat = DirectX::XMFLOAT3(m_CSInputBuffer.initalPos.x, m_CSInputBuffer.initalPos.y, m_CSInputBuffer.initalPos.z);
			DirectX::XMVECTOR tempVec = DirectX::XMLoadFloat3(&tempFloat);
			if (GlobalSettings::GetMainCamera()->GetComponent<Camera>().GetFrustum().Contains(tempVec))
			{
				return true;
			}
		}
		return false;
	}
}
