#include"PCH.h"
#include "ImGuiHandler.h"

namespace Aen
{
	ImGuiHandler::ImGuiHandler()
	{
	}

	ImGuiHandler::~ImGuiHandler()
	{
		/*if (m_deleteList.size() > 0)
		{
			for (size_t i = 0; i < m_deleteList.size(); i++)
			{
				delete m_entityList[m_deleteList[i] - static_cast<size_t>(1)];
			}
		}*/
	}

	vector<Aen::Entity*> ImGuiHandler::GetEntityList()
	{
		return m_entityList;
	}

	bool ImGuiHandler::LoadLevel(int index)
	{
		if(index >= m_levelImporter.GetRoomVector().size())
			return false;
		for (size_t i = 0; i < m_levelImporter.GetRoomVector()[index].GetModelVector().size(); i++)
		{
			cout << "Model " << endl;
			AddBase(m_levelImporter.GetRoomVector()[index].GetModelVector()[i], m_levelImporter.GetRoomVector()[index].GetTextureVector()[i]);
		}

		for (size_t i = 0; i < m_levelImporter.GetRoomVector()[index].GetLightVector().size(); i++)
		{
			if (m_levelImporter.GetRoomVector()[index].GetLightVector()[i].type == "Directional light")
			{
				cout << "d Light " << endl;

				AddDirectional(m_levelImporter.GetRoomVector()[index].GetLightVector()[i]);
			}
			else if (m_levelImporter.GetRoomVector()[index].GetLightVector()[i].type == "Spot light")
			{
				cout << "s Light " << endl;

				AddSpotLight(m_levelImporter.GetRoomVector()[index].GetLightVector()[i]);
			}
			else if (m_levelImporter.GetRoomVector()[index].GetLightVector()[i].type == "Point light")
			{
				cout << "p Light " << endl;

				AddPointLight(m_levelImporter.GetRoomVector()[index].GetLightVector()[i]);
			}
		}
		return true;
	}

	bool ImGuiHandler::LoadLevel(int index, Aen::Vec2f offset, float angle)
	{
		if (index >= m_levelImporter.GetRoomVector().size())
			return false;
		for (size_t i = 0; i < m_levelImporter.GetRoomVector()[index].GetModelVector().size(); i++)
		{
			cout << "Model " << endl;
			AddBase(m_levelImporter.GetRoomVector()[index].GetModelVector()[i], m_levelImporter.GetRoomVector()[index].GetTextureVector()[i], offset, angle);
		}

		for (size_t i = 0; i < m_levelImporter.GetRoomVector()[index].GetLightVector().size(); i++)
		{
			if (m_levelImporter.GetRoomVector()[index].GetLightVector()[i].type == "Directional light")
			{
				cout << "d Light " << endl;

				AddDirectional(m_levelImporter.GetRoomVector()[index].GetLightVector()[i]);
			}
			else if (m_levelImporter.GetRoomVector()[index].GetLightVector()[i].type == "Spot light")
			{
				cout << "s Light " << endl;

				AddSpotLight(m_levelImporter.GetRoomVector()[index].GetLightVector()[i], offset, angle);
			}
			else if (m_levelImporter.GetRoomVector()[index].GetLightVector()[i].type == "Point light")
			{
				cout << "p Light " << endl;

				AddPointLight(m_levelImporter.GetRoomVector()[index].GetLightVector()[i], offset, angle);
			}
		}
		return true;
	}

	AenIF::Room ImGuiHandler::GetRoom(size_t index)
	{
		return m_levelImporter.GetRoomVector()[index].GetRoom();
	}

	void ImGuiHandler::AddModel(Aen::Entity* entity)
	{
		m_entityCount++;
		m_entityList.push_back(entity);
	}

	void ImGuiHandler::AddLight(Aen::Entity* entity)
	{
		m_entityList.push_back(entity);
	}

	void ImGuiHandler::AddBase(AenIF::Model& model, AenIF::Texture& texture)
	{
		string imageName = AEN_RESOURCE_DIR(texture.name);
		string matName = "Material" + to_string(m_entityCount);
		string texName = "Texture" + to_string(m_entityCount);

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

		entity->SetPos(model.translation[0], model.translation[1], model.translation[2]);         
		entity->SetRot(model.rotation[0], model.rotation[1], model.rotation[2]);         
		entity->SetScale(model.scale[0], model.scale[1], model.scale[2]);

		AddModel(entity);
	}

	void ImGuiHandler::AddBase(AenIF::Model& model, AenIF::Texture& texture, Aen::Vec2f offset, float angle)
	{
		string imageName = AEN_RESOURCE_DIR(texture.name);
		string matName = "Material" + to_string(m_entityCount);
		string texName = "Texture" + to_string(m_entityCount);

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

		float posX = (model.translation[0] * c) - (model.translation[2] * s);
		float posY = (model.translation[0] * s) + (model.translation[2] * c);

		entity->SetPos(posX + offset.x, model.translation[1], posY + offset.y);
		entity->SetRot(model.rotation[0], model.rotation[1], model.rotation[2]);
		entity->SetScale(model.scale[0], model.scale[1], model.scale[2]);

		AddModel(entity);
	}


	void ImGuiHandler::AddPointLight(AenIF::Light& input)
	{
		Aen::Entity* light = &mp_entityHandlerPtr->CreateEntity();

		light->AddComponent<Aen::PointLight>();
		light->GetComponent<Aen::PointLight>().SetColor(input.color[0], input.color[1], input.color[2], 1);
		light->GetComponent<Aen::PointLight>().SetLightDist(1, 1, 1, 5);
		light->GetComponent<Aen::PointLight>().SetStrength(100);
		light->SetPos(input.translation[0], input.translation[1], input.translation[2]);
		light->SetRot(input.rotation[0], input.rotation[1], input.rotation[2]);

		AddLight(light);
	}

	void ImGuiHandler::AddPointLight(AenIF::Light& input, Aen::Vec2f offset, float angle)
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
		light->SetRot(input.rotation[0], input.rotation[1], input.rotation[2]);

		AddLight(light);
	}
	void ImGuiHandler::AddSpotLight(AenIF::Light& input)
	{
		Aen::Entity* light = &mp_entityHandlerPtr->CreateEntity();

		light->AddComponent<Aen::SpotLight>();
		light->GetComponent<Aen::SpotLight>().SetColor(input.color[0], input.color[1], input.color[2], 1);
		light->GetComponent<Aen::SpotLight>().SetConeSize(input.angle);
		light->GetComponent<Aen::SpotLight>().SetLightDist(input.attenuation[0], input.attenuation[1], input.attenuation[2], input.range);
		light->GetComponent<Aen::SpotLight>().SetStrength(input.intensity);
		light->SetPos(input.translation[0], input.translation[1], input.translation[2]);
		light->SetRot(input.rotation[0], input.rotation[1], input.rotation[2]);

		AddLight(light);
	}
	void ImGuiHandler::AddSpotLight(AenIF::Light& input, Aen::Vec2f offset, float angle)
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
		light->SetRot(input.rotation[0], input.rotation[1], input.rotation[2]);


		AddLight(light);
	}

	void ImGuiHandler::AddDirectional(AenIF::Light& input)
	{
		Aen::Entity* light = &mp_entityHandlerPtr->CreateEntity();

		light->AddComponent<Aen::DirectionalLight>();
		light->GetComponent<Aen::DirectionalLight>().SetColor(input.color[0], input.color[1], input.color[2], 1);
		light->GetComponent<Aen::DirectionalLight>().SetStrength(input.intensity);
		light->SetRot(input.direction[0], input.direction[1], input.direction[2]);

		AddLight(light);
	}

	AenIMP::LevelImporter* ImGuiHandler::GetImporterPtr()
	{
		return &m_levelImporter;
	}

	void ImGuiHandler::ReadAllFilesFromResourceFolder()
	{
		string filePath = "../LevelFolder/";
		string fileName = "";
		string fileType = "";

		
		for (const auto& entry : std::filesystem::directory_iterator(filePath))
		{
			fileName = entry.path().filename().string();
			fileType = fileName.substr(fileName.find_last_of(".") + 1);

			if (fileType == "Level")
			{
				m_levelImporter.ReadFromFile(filePath + fileName);
			}
		}
	}
}


