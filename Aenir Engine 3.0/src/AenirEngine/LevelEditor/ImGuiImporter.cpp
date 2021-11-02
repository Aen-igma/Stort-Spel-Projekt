#include "PCH.h"
#include "ImGuiImporter.h"
namespace Aen
{

	ImGuiImporter::ImGuiImporter(vector<Aen::Entity*>* m_entityList, vector<string>* m_itemList, unordered_map<size_t, IGH::ModelContainer>* m_modelMap, unordered_map<size_t, Aen::Entity*>* m_lightMap)
	{
		this->m_entityList = m_entityList;
		this->m_itemList = m_itemList;
		this->m_modelMap = m_modelMap;
		this->m_lightMap = m_lightMap;

	}


	bool ImGuiImporter::import(AenIMP::LevelImporter& m_levelImporter, string& levelPath, float* translation, float* rotation, float* scale)
	{
		m_levelImporter.ReadFromFile(levelPath);

		for (size_t i = 0; i < m_levelImporter.GetRoomVector()[0].GetModelVector().size(); i++)
		{
			//size_t id = AddBase(m_levelImporter.GetRoomVector()[0].GetModelVector()[i], m_levelImporter.GetRoomVector()[0].GetTextureVector()[i]);
			size_t id = AddBase(m_levelImporter.GetRoomVector()[0].GetModelVector()[i], m_levelImporter.GetRoomVector()[0].GetTextureVector()[i]);

			float tX = 0, tY = 0, tZ = 0;
			float rX = 0, rY = 0, rZ = 0;
			float sX = 0, sY = 0, sZ = 0;

			GetFloatArray(m_levelImporter.GetRoomVector()[0].GetModelVector()[i].translation, tX, tY, tZ);
			GetFloatArray(m_levelImporter.GetRoomVector()[0].GetModelVector()[i].rotation, rX, rY, rZ);
			GetFloatArray(m_levelImporter.GetRoomVector()[0].GetModelVector()[i].scale, sX, sY, sZ);

			mp_entityHandlerPtr->GetEntity(id).SetPos(((tX + translation[0])) * scale[0], ((tY + translation[1])) * scale[1], ((tZ + translation[2])) * scale[2]);
			mp_entityHandlerPtr->GetEntity(id).SetRot(rX + rotation[0], rY + rotation[1], rZ + rotation[2]);
			mp_entityHandlerPtr->GetEntity(id).SetScale(scale[0] * sX, scale[1] * sY, scale[2] * sZ);

		}

		for (size_t i = 0; i < m_levelImporter.GetRoomVector()[0].GetLightVector().size(); i++)
		{
			if (m_levelImporter.GetRoomVector()[0].GetLightVector()[i].type == IGH::DIRECTIONALLIGHT.c_str())
			{
				size_t id = AddDirectional(m_levelImporter.GetRoomVector()[0].GetLightVector()[i]);
			}
			else if (m_levelImporter.GetRoomVector()[0].GetLightVector()[i].type == IGH::SPOTLIGHT.c_str())
			{
				size_t id = AddSpotLight(m_levelImporter.GetRoomVector()[0].GetLightVector()[i]);

				float tX = 0, tY = 0, tZ = 0;
				float rX = 0, rY = 0, rZ = 0;

				GetFloatArray(m_levelImporter.GetRoomVector()[0].GetModelVector()[i].translation, tX, tY, tZ);
				GetFloatArray(m_levelImporter.GetRoomVector()[0].GetModelVector()[i].rotation, rX, rY, rZ);

				mp_entityHandlerPtr->GetEntity(id).SetPos(tX + translation[0], tY + translation[1], tZ + translation[2]);
				mp_entityHandlerPtr->GetEntity(id).SetRot(rX + rotation[0], rY + rotation[1], rZ + rotation[2]);
			}
			else if (m_levelImporter.GetRoomVector()[0].GetLightVector()[i].type == IGH::POINTLIGHT.c_str())
			{
				size_t id = AddPointLight(m_levelImporter.GetRoomVector()[0].GetLightVector()[i]);

				float tX = 0, tY = 0, tZ = 0;

				GetFloatArray(m_levelImporter.GetRoomVector()[0].GetLightVector()[i].translation, tX, tY, tZ);

				mp_entityHandlerPtr->GetEntity(id).SetPos(tX + translation[0], tY + translation[1], tZ + translation[2]);
			}
		}
		m_levelImporter.GetRoomVector().clear();

		return false;
	}

	void ImGuiImporter::GetFloatArray(float* inputArray, float& x, float& y, float& z)
	{
		x = inputArray[0];
		y = inputArray[1];
		z = inputArray[2];
	}

	size_t ImGuiImporter::AddBase(AenIF::Model& model, AenIF::Texture& texture)
	{
		string imageName = AEN_RESOURCE_DIR(texture.name);
		string materialName = IGH::MATERIAL + to_string(m_entityCount);
		string textureName = IGH::TEXTURE + to_string(m_entityCount);

		Aen::Entity* entity = &mp_entityHandlerPtr->CreateEntity();

		Aen::Mesh& mesh = Aen::Resource::CreateMesh(model.name);
		mesh.Load(AEN_RESOURCE_DIR(model.mesh));

		Aen::Texture& materialTexture = Aen::Resource::CreateTexture(textureName);
		materialTexture.LoadTexture(imageName);
		Aen::Material& material = Aen::Resource::CreateMaterial(materialName, true);
		material.SetDiffuseMap(materialTexture);

		entity->AddComponent<Aen::MeshInstance>();
		entity->GetComponent<Aen::MeshInstance>().SetMesh(mesh);

		entity->SetPos(model.translation[0], model.translation[1], model.translation[2]);
		entity->SetRot(model.rotation[0], model.rotation[1], model.rotation[2]);
		entity->SetScale(model.scale[0], model.scale[1], model.scale[2]);

		size_t id = entity->GetID();
		Aen::ComponentHandler::GetMeshInstance(static_cast<size_t>(id)).SetMaterial(material);

		AddEnemy(entity, model);
		AddModel(entity, model.name);
		m_modelMap->insert(std::make_pair(entity->GetID(), IGH::ModelContainer(textureName, materialName, texture.name, model.name, model.mesh, model.type, model.rigidBody, model.rigidBodyType)));

		return id;
	}
	size_t ImGuiImporter::AddBase(const string& meshName, const string& objName)
	{
		string imageName = AEN_RESOURCE_DIR("Missing_Textures.png");
		string materialName = "Material" + to_string(m_entityCount);
		string textureName = "Texture" + to_string(m_entityCount);


		Aen::Entity* entity = &mp_entityHandlerPtr->CreateEntity();
		Aen::Mesh& mesh = Aen::Resource::CreateMesh(meshName + std::to_string(m_entityCount));
		mesh.Load(AEN_RESOURCE_DIR(objName));

		entity->AddComponent<Aen::MeshInstance>();
		entity->GetComponent<Aen::MeshInstance>().SetMesh(mesh);


		//AddModel(entity, meshName);
		AddModel(entity, meshName);

		m_modelMap->insert(std::make_pair(entity->GetID(), IGH::ModelContainer(textureName, materialName, imageName, meshName, objName)));

		size_t id = entity->GetID();

		return id;
	}

	void ImGuiImporter::AddModel(Aen::Entity* entity, string name)
	{
		m_entityList->push_back(entity);
		m_itemList->push_back(name + std::to_string(m_entityCount));
		//m_selected.push_back(false);
		//m_selectedEntities.push_back(-1);
		m_entityCount++;
	}

	void ImGuiImporter::AddModel(Aen::Entity* entity)
	{
		m_entityList->push_back(entity);
		m_itemList->push_back(IGH::MODEL + std::to_string(m_entityCount));
		//m_selected.push_back(false);
		//m_selectedEntities.push_back(-1);
		m_entityCount++;
	}

	void ImGuiImporter::AddEnemy(Aen::Entity* entity, AenIF::Model& model)
	{
		if (model.type == IGH::NORMALENEMY)
		{
			OutputDebugStringA(IGH::NORMALENEMY.c_str());
		}
		else if (model.type == IGH::BOSS.c_str())
		{
			OutputDebugStringA(IGH::NORMALENEMY.c_str());
		}
	}

	void ImGuiImporter::AddLight(Aen::Entity* entity)
	{
		m_entityList->push_back(entity);
		m_itemList->push_back(IGH::LIGHT + std::to_string(m_lightCount));
		m_lightCount++;
	}

	void ImGuiImporter::AddLight(Aen::Entity* entity, string type)
	{
		m_lightMap->insert(std::make_pair(entity->GetID(), entity));
		m_entityList->push_back(entity);
		m_itemList->push_back(type + std::to_string(m_lightCount));
		m_lightCount++;
	}

	size_t ImGuiImporter::AddPointLight(AenIF::Light& input)
	{
		Aen::Entity* light = &mp_entityHandlerPtr->CreateEntity();


		light->AddComponent<Aen::PointLight>();
		light->GetComponent<Aen::PointLight>().SetColor(input.color[0], input.color[1], input.color[2], 1);
		light->GetComponent<Aen::PointLight>().SetLightDist(input.attenuation[0], input.attenuation[1], input.attenuation[2], input.range);
		light->GetComponent<Aen::PointLight>().SetStrength(100);
		light->SetPos(input.translation[0], input.translation[1], input.translation[2]);

		AddLight(light, IGH::POINTLIGHT);

		return light->GetID();
	}

	void ImGuiImporter::AddPointLight()
	{
		Aen::Entity* light = &mp_entityHandlerPtr->CreateEntity();

		light->AddComponent<Aen::PointLight>();
		light->GetComponent<Aen::PointLight>().SetColor(Aen::Color::White);
		light->GetComponent<Aen::PointLight>().SetLightDist(1, 1, 1, 1);
		light->GetComponent<Aen::PointLight>().SetStrength(100);
		light->SetPos(0.0f, 0.0f, 0.0f);

		AddLight(light, IGH::POINTLIGHT);
	}

	size_t ImGuiImporter::AddSpotLight(AenIF::Light& input)
	{
		Aen::Entity* light = &mp_entityHandlerPtr->CreateEntity();
		light->AddComponent<Aen::SpotLight>();
		light->GetComponent<Aen::SpotLight>().SetColor(input.color[0], input.color[1], input.color[2], 1);
		light->GetComponent<Aen::SpotLight>().SetConeSize(input.angle);
		light->GetComponent<Aen::SpotLight>().SetLightDist(input.attenuation[0], input.attenuation[1], input.attenuation[2], input.range);
		light->GetComponent<Aen::SpotLight>().SetStrength(input.intensity);
		light->SetPos(input.translation[0], input.translation[1], input.translation[2]);
		light->SetRot(input.rotation[0], input.rotation[1], input.rotation[2]);
		
		AddLight(light, IGH::SPOTLIGHT);
		return light->GetID();
	}

	void ImGuiImporter::AddSpotLight()
	{
		Aen::Entity* light = &mp_entityHandlerPtr->CreateEntity();

		light->AddComponent<Aen::SpotLight>();
		light->GetComponent<Aen::SpotLight>().SetColor(Aen::Color::White);
		light->GetComponent<Aen::SpotLight>().SetConeSize(1);
		light->GetComponent<Aen::SpotLight>().SetLightDist(1, 1, 1, 1);
		light->GetComponent<Aen::SpotLight>().SetStrength(100);
		light->SetPos(0.0f, 0.0f, 0.0f);
		light->SetRot(0.0f, 0.0f, 0.0f);

		AddLight(light, IGH::SPOTLIGHT);
	}

	size_t ImGuiImporter::AddDirectional(AenIF::Light& input)
	{
		Aen::Entity* light = &mp_entityHandlerPtr->CreateEntity();

		light->AddComponent<Aen::DirectionalLight>();
		light->GetComponent<Aen::DirectionalLight>().SetColor(input.color[0], input.color[1], input.color[2], 1);
		light->GetComponent<Aen::DirectionalLight>().SetStrength(input.intensity);
		light->SetRot(input.direction[0], input.direction[1], input.direction[2]);

		AddLight(light, IGH::DIRECTIONALLIGHT);
		return light->GetID();
	}

	void ImGuiImporter::AddDirectional()
	{
		Aen::Entity* light = &mp_entityHandlerPtr->CreateEntity();

		light->AddComponent<Aen::DirectionalLight>();
		light->GetComponent<Aen::DirectionalLight>().SetColor(Aen::Color::White);
		light->GetComponent<Aen::DirectionalLight>().SetStrength(1);
		light->SetRot(0.0f, 0.0f, 0.0f);

		AddLight(light, IGH::DIRECTIONALLIGHT);
	}

}