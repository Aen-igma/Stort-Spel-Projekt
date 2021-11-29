#include "PCH.h"
#include "PSSystemComponent.h"
#include "Core/Renderer.h"
#include"Importer/ObjImporter.h"
#include "../Mesh/MeshInstance.h"
#include"Core/GlobalSettings.h"


namespace Aen 
{
	Aen::PSSystemcomponent::PSSystemcomponent(const size_t& id)
		:Drawable(id),m_UAView(sizeof(VertexParticle),1024)
	{
		//m_Particle.m_velocity = m_CSInputBuffer.m_vel.y;
		//m_Particle.m_pos.x = 10;
		//m_Particle.m_pos.y = 10;
		//m_Particle.m_pos.z = 10;
		//m_CSInputBuffer.m_particleCount = 10;
		//m_ParticleList.m_velocity = m_CSInputBuffer.m_vel.y;
		//m_ParticleList.m_pos[1] = 0.0f;
		//m_ParticleList = 0;

		//this->m_CSInputBuffer.m_velocity.x = 1.0f;
		//this->m_CSInputBuffer.m_velocity.y = 2.0f;
		//this->m_CSInputBuffer.m_velocity.z = 1.0f;

	}

	Aen::PSSystemcomponent::~PSSystemcomponent()
	{
		delete m_texture;
		delete m_pMesh;
		delete m_shader;
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

	void PSSystemcomponent::SetMaterial(Material& material)
	{
		m_pMaterial = &material;
	}

	void PSSystemcomponent::SetMaterial(const std::string& materialName)
	{
		if (!Resource::MaterialExist(materialName))throw;
		m_pMaterial = &Resource::GetMaterial(materialName);
	}


	void PSSystemcomponent::SetMaterial(const std::string& materialSlotName, Material& material)
	{
		if (m_pMesh->m_meshMaterialName.count(materialSlotName) == 0)throw;
		m_pMaterial[m_pMesh->m_meshMaterialName.at(materialSlotName) == 0] = &material;
	}

	void PSSystemcomponent::SetMaterial(const std::string& materialSlotName, const std::string& materialName)
	{
		if (m_pMesh->m_meshMaterialName.count(materialSlotName) == 0)throw;
		if (!Resource::MaterialExist(materialName))throw;
		m_pMaterial[m_pMesh->m_meshMaterialName.at(materialSlotName)] = &Resource::GetMaterial(materialName);
	}

	void PSSystemcomponent::SetMaterial(const UINT& index, Material& material)
	{
		m_pMaterial[index] = &material;
	}

	void PSSystemcomponent::updatePS(const float& framerate)
	{
		m_CSInputBuffer.deltaTime = framerate;
		
	}

	void PSSystemcomponent::EmitRandom(float frameTime)
	{
		
		this->runTimes += frameTime;
		this->accumulatedTime += frameTime;
		bool emitPS, found;
		emitPS = false;
		int index, i, j;
		Vec3f pos;
		float velocity;
		if (this->accumulatedTime > 1.0f/ this->particlesPerSecond)
		{
			emitPS = true;
			this->accumulatedTime = 0.0f;
		}
		if (emitPS && this->currentNrPS < (this->maxParticles - 1))
		{
			this->currentNrPS++;
			DirectX::XMFLOAT3 randPosPS
			{
				(((float)rand() - (float)rand()) / RAND_MAX) + 0.5f,
				(((float)rand() - (float)rand()) / RAND_MAX) + 0.5f,
				(((float)rand() - (float)rand()) / RAND_MAX) + 0.5f
			};
			float randVelocity = this->particleVelocity + (((float)rand() 
				- (float)rand())/ RAND_MAX) * this->particleVelocityVariation;
			m_VertexPS.m_Pos.x = randPosPS.x;
			m_VertexPS.m_Pos.y = randPosPS.y;
			m_VertexPS.m_Pos.z = randPosPS.z;
			found = false;
		}
		return;
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
		this->m_CSInputBuffer.m_emitCount = nr;
		this->currentNrPS = this->m_CSInputBuffer.m_emitCount;
		
	}

	void PSSystemcomponent::SetPos(float x, float y, float z)
	{
		//this->m_CSInputBuffer.m_InitalPos.x = x;
		//this->m_CSInputBuffer.m_InitalPos.y = y;
		//this->m_CSInputBuffer.m_InitalPos.z = z;
		m_VertexPS.m_Pos.x = x;
		m_VertexPS.m_Pos.y = y;
		m_VertexPS.m_Pos.z = z;
		
	}

	void PSSystemcomponent::SetVelocity(float x, float y, float z)
	{
		this->m_VertexPS.m_Velocity.x = x;
		this->m_VertexPS.m_Velocity.y = y;
		this->m_VertexPS.m_Velocity.z = z;
	}



	bool PSSystemcomponent::activate()
	{
		/*this->m_pVbuffer->Create(m_VertexPS, sizeof(VertexParticle) * this->maxParticles,D3D11_USAGE_IMMUTABLE );*/
		return true;
	}

	void PSSystemcomponent::SetEmitPos(float x, float y, float z)
	{
		this->m_emitPos.x = x;
		this->m_emitPos.y = y;
		this->m_emitPos.z = z;
	}

	void PSSystemcomponent::Initialize()
	{
		this->m_vertexCount = this->maxParticles;
		/*this->m_pVbuffer->Create(&m_VertexPS,this->m_vertexCount,D3D11_USAGE_IMMUTABLE);*/
	}

	void PSSystemcomponent::Draw(Renderer& renderer, const uint32_t& layer) {

		// First Pass
		// 
		renderer.m_PSInputBuffer.GetData().m_InitalPos.x = this->m_emitPos.x;
		renderer.m_PSInputBuffer.GetData().m_InitalPos.y = this->m_emitPos.y;
		renderer.m_PSInputBuffer.GetData().m_InitalPos.z = this->m_emitPos.z;
		renderer.m_PSInputBuffer.GetData().m_velocity.x = this->m_emitDir.x;
		renderer.m_PSInputBuffer.GetData().m_velocity.y = this->m_emitDir.y;
		renderer.m_PSInputBuffer.GetData().m_velocity.z = this->m_emitDir.z;


		RenderSystem::ClearRenderTargetView(renderer.m_particleOut, Color(0.f, 0.f, 0.f, 0.f));
		RenderSystem::SetPrimitiveTopology(Topology::POINTLIST);
		RenderSystem::SetInputLayout(renderer.m_PSLayout);
		renderer.m_cbTransform.GetData().m_mdlMat = EntityHandler::GetEntity(m_id).GetTransformation();
		renderer.m_cbTransform.UpdateBuffer();
		renderer.m_cbTransform.BindBuffer<VShader>(0);
	
	/*	renderer.m_VertexBuffer.Create(&m_PS,sizeof(VertexParticle)*this->maxParticles);
		renderer.m_VertexBuffer.UpdateBuffer(&m_PS,0);
		renderer.m_VertexBuffer.BindBuffer();*/



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
		RenderSystem::SetRasteriserState(renderer.m_rasterizerState);
		//RenderSystem::SetRasteriserState(renderer.m_wireFrameState);


		RenderSystem::BindShader(renderer.m_PSVShader);
		RenderSystem::BindShader(renderer.m_PSGShader);
		RenderSystem::BindShader(renderer.m_PSPShader);

		RenderSystem::Draw(this->currentNrPS,0);

		// Second Pass

		RenderSystem::UnBindShader<VShader>();
		RenderSystem::UnBindShader<GShader>();
		RenderSystem::UnBindShader<PShader>();
		RenderSystem::UnBindShaderResources<VShader>(0,1);
		RenderSystem::UnBindShaderResources<PShader>(0,2);
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

		renderer.m_PSInputBuffer.GetData() = m_CSInputBuffer;
		renderer.m_PSInputBuffer.UpdateBuffer();
		renderer.m_PSInputBuffer.BindBuffer<CShader>(0);
		RenderSystem::BindUnOrderedAccessView(0,m_UAView);
		RenderSystem::BindShader(renderer.m_PSCShader);

		RenderSystem::Dispatch(16, 1, 1);

		RenderSystem::UnBindShader<CShader>();
		RenderSystem::UnBindUnOrderedAccessViews(0,1);

		RenderSystem::SetPrimitiveTopology(Topology::POINTLIST);
		RenderSystem::SetInputLayout(renderer.m_PSLayout);
		renderer.m_cbTransform.GetData().m_mdlMat = EntityHandler::GetEntity(m_id).GetTransformation();
		renderer.m_cbTransform.UpdateBuffer();
		renderer.m_cbTransform.BindBuffer<VShader>(0);

		RenderSystem::BindShaderResourceView<VShader>(0, m_UAView);

		renderer.m_cbTransform.BindBuffer<GShader>(0);

		RenderSystem::SetRasteriserState(renderer.m_rasterizerState);

		RenderSystem::BindShader(renderer.m_PSVShader);
		RenderSystem::BindShader(renderer.m_PSGShader);
		RenderSystem::BindShader(renderer.m_transparencyPS);

		RenderSystem::Draw(this->currentNrPS, 0);

		RenderSystem::UnBindShader<VShader>();
		RenderSystem::UnBindShader<GShader>();
		RenderSystem::UnBindShader<PShader>();
		RenderSystem::UnBindShaderResources<VShader>(0,1);
	}
	bool PSSystemcomponent::FrustumCull(Renderer& renderer) 
	{
		return true;
	}
}
