#include "PCH.h"
#include "ImGuiImporter.h"
namespace Aen
{

	ImGuiImporter::ImGuiImporter()
	{
		this->m_entityList = new vector<Aen::Entity*>;
		this->m_itemList = new vector<string>;
		this->m_modelMap = new unordered_map< size_t, IGH::ModelContainer>;
		this->m_lightMap = new unordered_map< size_t, Aen::Entity*>;
		this->m_levelImporter = new AenIMP::LevelImporter();
		m_standAlone = true;
	}

	ImGuiImporter::ImGuiImporter(vector<Aen::Entity*>* m_entityList, vector<string>* m_itemList, unordered_map<size_t, IGH::ModelContainer>* m_modelMap, unordered_map<size_t, Aen::Entity*>* m_lightMap, AenIMP::LevelImporter* m_levelImporter)
	{
		this->m_entityList = m_entityList;
		this->m_itemList = m_itemList;
		this->m_modelMap = m_modelMap;
		this->m_lightMap = m_lightMap;
		this->m_levelImporter = m_levelImporter;
		m_standAlone = false;
	}

	ImGuiImporter::~ImGuiImporter()
	{
		if (m_standAlone == true)
		{
			if (m_levelImporter != nullptr)
			{
				delete m_levelImporter;
			}

			if (m_itemList != nullptr)
			{
				delete m_itemList;
			}

			if (m_entityList != nullptr)
			{
				delete m_entityList;
			}
			if (m_modelMap != nullptr)
			{
				delete m_modelMap;
			}
			if (m_lightMap != nullptr)
			{
				delete m_lightMap;
			}
		}
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

	void ImGuiImporter::ReadAllFilesFromResourceFolder()
	{
		string filePath = IGH::LEVELPATH;
		string fileName = "";
		string fileType = "";

		for (const auto& entry : std::filesystem::directory_iterator(filePath))
		{
			fileName = entry.path().filename().string();
			fileType = fileName.substr(fileName.find_last_of(".") + 1);

			if (fileType == "Level")
			{
				m_levelImporter->ReadFromFile(filePath + fileName);
			}
		}
	}

	AenIMP::LevelImporter* ImGuiImporter::GetImporterPtr()
	{
		return m_levelImporter;
	}

	AenIF::Room ImGuiImporter::GetRoom(size_t index)
	{
		return m_levelImporter->GetRoomVector()[index].GetRoom();
	}

	vector<Aen::Entity*>* ImGuiImporter::GetEntityList()
	{
		return m_entityList;
	}

	bool ImGuiImporter::LoadLevel(int index)
	{
		if (index >= m_levelImporter->GetRoomVector().size())
			return false;
		for (size_t i = 0; i < m_levelImporter->GetRoomVector()[index].GetModelVector().size(); i++)
		{
			AddBase(m_levelImporter->GetRoomVector()[index].GetModelVector()[i], m_levelImporter->GetRoomVector()[index].GetTextureVector()[i]);
		}

		for (size_t i = 0; i < m_levelImporter->GetRoomVector()[index].GetLightVector().size(); i++)
		{
			if (m_levelImporter->GetRoomVector()[index].GetLightVector()[i].type == "Directional light")
			{

				AddDirectional(m_levelImporter->GetRoomVector()[index].GetLightVector()[i]);
			}
			else if (m_levelImporter->GetRoomVector()[index].GetLightVector()[i].type == "Spot light")
			{
				cout << "s Light " << endl;

				AddSpotLight(m_levelImporter->GetRoomVector()[index].GetLightVector()[i]);
			}
			else if (m_levelImporter->GetRoomVector()[index].GetLightVector()[i].type == "Point light")
			{
				cout << "p Light " << endl;

				AddPointLight(m_levelImporter->GetRoomVector()[index].GetLightVector()[i]);
			}
		}
		return true;
	}

	bool ImGuiImporter::LoadLevel(AenIMP::CompleteRoom* roomPtr, Aen::Vec2f offset, float angle)
	{
		//if (index >= m_levelImporter.GetRoomVector().size())
		//	return false;
		for (size_t i = 0; i < roomPtr->GetModelVector().size(); i++)
		{
			cout << "Model " << endl;
			AddBase(roomPtr->GetModelVector()[i], roomPtr->GetTextureVector()[i], offset, angle);
		}

		for (size_t i = 0; i < roomPtr->GetLightVector().size(); i++)
		{
			if (roomPtr->GetLightVector()[i].type == "Directional light")
			{
				cout << "d Light " << endl;

				AddDirectional(roomPtr->GetLightVector()[i]);
			}
			else if (roomPtr->GetLightVector()[i].type == "Spot light")
			{
				cout << "s Light " << endl;

				AddSpotLight(roomPtr->GetLightVector()[i], offset, angle);
			}
			else if (roomPtr->GetLightVector()[i].type == "Point light")
			{
				cout << "p Light " << endl;

				AddPointLight(roomPtr->GetLightVector()[i], offset, angle);
			}
		}
		return true;
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

	void ImGuiImporter::AddBase(AenIF::Model& model, AenIF::Texture& texture, Aen::Vec2f offset, float angle)
	{
		string imageName = AEN_RESOURCE_DIR(texture.name);
		string matName = IGH::MATERIAL + to_string(m_entityCount);
		string texName = IGH::TEXTURE + to_string(m_entityCount);

		Aen::Entity* entity = &mp_entityHandlerPtr->CreateEntity();
		Aen::Mesh& mesh = Aen::Resource::CreateMesh(model.name + std::to_string(m_entityCount));
		mesh.Load(AEN_RESOURCE_DIR(model.mesh));

		Aen::Texture& matTexture = Aen::Resource::CreateTexture(texName);
		matTexture.LoadTexture(imageName);
		Aen::Material& mat = Aen::Resource::CreateMaterial(matName, true);
		mat.SetDiffuseMap(matTexture);

		entity->AddComponent<Aen::MeshInstance>();
		entity->GetComponent<Aen::MeshInstance>().SetMesh(mesh);

		size_t id = entity->GetID();
		Aen::ComponentHandler::GetMeshInstance(static_cast<uint32_t>(id)).SetMaterial(mat);

		float s = sin(angle);
		float c = cos(angle);

		float posX = ((model.translation[0]) * c) - ((model.translation[2]) * s);
		float posZ = ((model.translation[0]) * s) + ((model.translation[2]) * c);

		entity->SetPos(posX + offset.x, model.translation[1], posZ + offset.y);
		entity->SetRot(model.rotation[0], model.rotation[1] + (angle * 57.2957795), model.rotation[2]);
		entity->SetScale(model.scale[0], model.scale[1], model.scale[2]);

		AddModel(entity);
	}

	void ImGuiImporter::AddPointLight(AenIF::Light& input, Aen::Vec2f offset, float angle)
	{
		Aen::Entity* light = &mp_entityHandlerPtr->CreateEntity();

		light->AddComponent<Aen::PointLight>();
		light->GetComponent<Aen::PointLight>().SetColor(input.color[0], input.color[1], input.color[2], 1);
		light->GetComponent<Aen::PointLight>().SetLightDist(1, 1, 1, 5);
		light->GetComponent<Aen::PointLight>().SetStrength(100);

		float s = sin(angle);
		float c = cos(angle);

		float posX = (input.translation[0] * c) - (input.translation[2] * s);
		float posY = (input.translation[0] * s) + (input.translation[2] * c);

		light->SetPos(posX + offset.x, input.translation[1], posY + offset.y);
		light->SetRot(input.rotation[0], input.rotation[1] + (angle * 57.2957795), input.rotation[2]);

		AddLight(light);
	}

	void ImGuiImporter::AddSpotLight(AenIF::Light& input, Aen::Vec2f offset, float angle)
	{
		Aen::Entity* light = &mp_entityHandlerPtr->CreateEntity();

		light->AddComponent<Aen::SpotLight>();
		light->GetComponent<Aen::SpotLight>().SetColor(input.color[0], input.color[1], input.color[2], 1);
		light->GetComponent<Aen::SpotLight>().SetConeSize(input.angle);
		light->GetComponent<Aen::SpotLight>().SetLightDist(input.attenuation[0], input.attenuation[1], input.attenuation[2], input.range);
		light->GetComponent<Aen::SpotLight>().SetStrength(input.intensity);

		float s = sin(angle);
		float c = cos(angle);

		float posX = (input.translation[0] * c) - (input.translation[2] * s);
		float posY = (input.translation[0] * s) + (input.translation[2] * c);

		light->SetPos(posX + offset.x, input.translation[1], posY + offset.y);
		light->SetRot(input.rotation[0], input.rotation[1] + (angle * 57.2957795), input.rotation[2]);

		AddLight(light);
	
	}

}