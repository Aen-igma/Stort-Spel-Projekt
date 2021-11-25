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
		/*m_CSInputBuffer.m_particleCount = 10;*/
		/*m_ParticleList.m_velocity = m_CSInputBuffer.m_vel.y;*/
		//m_ParticleList.m_pos[1] = 0.0f;
		//m_ParticleList = 0;

		this->m_CSInputBuffer.m_velocity.x = 1.0f;
		this->m_CSInputBuffer.m_velocity.y = 2.0f;
		this->m_CSInputBuffer.m_velocity.z = 1.0f;
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
		m_material = &Aen::Resource::CreateMaterial(GetNameFromPath(dir));
		m_material->LoadeAndSetDiffuseMap(dir);
	}

	void PSSystemcomponent::SetMaterial(Material& material)
	{
		m_material[0] = material;
	}


	void PSSystemcomponent::SetMaterial(const std::string& name)
	{
		if (!Resource::MaterialExist(name))throw;
		m_material[0] = Resource::GetMaterial(name);
	}

	//void PSSystemcomponent::SetMaterial(const std::string& materialSlotName, Material& material)
	//{
	//	if (m_pMesh->m_meshMaterialName.count(materialSlotName) == 0) throw;
	//	m_material[m_pMesh->m_meshMaterialName.at(materialSlotName)] = &material;
	//}

	//void PSSystemcomponent::SetMaterial(const std::string& materialSlotName, const std::string& materialName)
	//{
	//	if (m_pMesh->m_meshMaterialName.count(materialSlotName) == 0) throw;
	//	if (!Resource::MaterialExist(materialName)) throw;
	//	m_material[m_pMesh->m_meshMaterialName.at(materialSlotName)] = &Resource::GetMaterial(materialName);
	//}

	//void PSSystemcomponent::SetMaterial(const UINT& index, Material& material)
	//{
	//}

	//void MeshInstance::SetMaterial(const UINT& index, Material& material) {
	//	m_material[index] = &material;
	//}

	void PSSystemcomponent::updatePS(const float& framerate)
	{
		m_CSInputBuffer.deltaTime = framerate;
	}

	void PSSystemcomponent::InitParticleVariables()
	{
		//this->particleDeviationX = 100.0f;
		//this->particleDeviationY = 10.0f;
		//this->particleDeviationZ = 100.0f;
		//this->particleVelocity = 10.0f;
		//this->particleVelocityVariation = 1.0f;
		//this->particleSize = 0.12f;
		//this->particlesPerSecond = 2500.0f;
		//this->maxParticles = 1024;
		//this->m_ParticleList = new Particle[maxParticles];
		//if (!m_ParticleList)
		//	throw;
		////Initalizing all particles at start
		////for (i = 0; i < maxParticles; i++)
		////{
		////	m_ParticleList[i].active = true;
		////}
		//this->currentNrPS = 0;
		//this->accumulatedTime = 0.0f;
	}

	void PSSystemcomponent::activatePS()
	{

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
			//m_ParticleList[this->currentNrPS].m_pos[0] = randPosPS.x;
			//m_ParticleList[this->currentNrPS].m_pos[1] = randPosPS.y;
			//m_ParticleList[this->currentNrPS].m_pos[2] = randPosPS.z;
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
		for (int i = 0; i < this->currentNrPS; i++)
		{
			//this->m_ParticleList[i].m_pos.x = x;
			//this->m_ParticleList[i].m_pos.y = y;
			//this->m_ParticleList[i].m_pos.z = z;
		}
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



	void PSSystemcomponent::SetVelo(float x)
	{
		//for (int i = 0; i < this->currentNrPS; i++)
		//{
		//	this->m_ParticleList[i].m_velocity = x;
		//}
	}

	void PSSystemcomponent::SetVel(float dir)
	{

		//this->m_ParticleList[currentNrPS].m_velocity = dir;
		//this->m_CSInputBuffer[currentNrPS].m_vel.y = y;
		//this->m_CSInputBuffer[currentNrPS].m_vel.z = z;
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

		/*RenderSystem::Dispatch((int)(this->maxParticles / 64.f)+ (this->maxParticles % 64 != 0), 1, 1);*/
		RenderSystem::Dispatch(16, 1, 1);
	
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

		RenderSystem::Draw(this->currentNrPS,0);


		RenderSystem::UnBindShader<VShader>();
		RenderSystem::UnBindShader<GShader>();
		RenderSystem::UnBindShader<PShader>();
		RenderSystem::UnBindShaderResources<VShader>(0,1);

	}


	void PSSystemcomponent::DepthDraw(Renderer& renderer, const uint32_t& layer)
	{
		
	}
	bool PSSystemcomponent::FrustumCull(Renderer& renderer)
	{
		return true;
	}
}
