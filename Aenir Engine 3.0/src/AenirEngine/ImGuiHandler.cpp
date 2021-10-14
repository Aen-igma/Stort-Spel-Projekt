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

	void ImGuiHandler::LoadLevel(int& index)
	{
		for (size_t i = 0; i < m_levelImporter.GetRoomVector()[index].GetModelVector().size(); i++)
		{
			AddBase(m_levelImporter.GetRoomVector()[index].GetModelVector()[i]);
		}

		for (size_t i = 0; i < m_levelImporter.GetRoomVector()[index].GetLightVector().size(); i++)
		{
			if (m_levelImporter.GetRoomVector()[index].GetLightVector()[i].type == "Directional light")
			{
				AddDirectional(m_levelImporter.GetRoomVector()[index].GetLightVector()[i]);
			}
			else if (m_levelImporter.GetRoomVector()[index].GetLightVector()[i].type == "Spot light")
			{
				AddSpotLight(m_levelImporter.GetRoomVector()[index].GetLightVector()[i]);
			}
			else if (m_levelImporter.GetRoomVector()[index].GetLightVector()[i].type == "Point light")
			{
				AddPointLight(m_levelImporter.GetRoomVector()[index].GetLightVector()[i]);
			}
		}
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

	void ImGuiHandler::AddBase(AenIF::Model& model)
	{
		Aen::Entity* entity = AEN_NEW(Aen::Entity);
		Aen::Mesh& mesh = Aen::Resource::CreateMesh(model.name + std::to_string(m_entityCount));
		mesh.Load(AEN_RESOURCE_DIR(model.mesh));

		entity->AddComponent<Aen::MeshInstance>();
		entity->GetComponent<Aen::MeshInstance>().SetMesh(mesh);

		AddModel(entity);
	}


	void ImGuiHandler::AddPointLight(AenIF::Light& input)
	{
		Aen::Entity* light = AEN_NEW(Aen::Entity);

		light->AddComponent<Aen::PointLight>();
		light->GetComponent<Aen::PointLight>().SetColor(input.color[0], input.color[1], input.color[2], 1);
		light->GetComponent<Aen::PointLight>().SetLightDist(1, 1, 1, 5);
		light->GetComponent<Aen::PointLight>().SetStrength(100);
		light->SetPos(input.translation[0], input.translation[1], input.translation[2]);
		light->SetRot(input.rotation[0], input.rotation[1], input.rotation[2]);

		AddLight(light);
	}

	void ImGuiHandler::AddSpotLight(AenIF::Light& input)
	{
		Aen::Entity* light = AEN_NEW(Aen::Entity);

		light->AddComponent<Aen::SpotLight>();
		light->GetComponent<Aen::SpotLight>().SetColor(input.color[0], input.color[1], input.color[2], 1);
		light->GetComponent<Aen::SpotLight>().SetConeSize(input.angle);
		light->GetComponent<Aen::SpotLight>().SetLightDist(input.attenuation[0], input.attenuation[1], input.attenuation[2], input.range);
		light->GetComponent<Aen::SpotLight>().SetStrength(input.intensity);
		light->SetPos(input.translation[0], input.translation[1], input.translation[2]);
		light->SetRot(input.rotation[0], input.rotation[1], input.rotation[2]);

		AddLight(light);
	}

	void ImGuiHandler::AddDirectional(AenIF::Light& input)
	{
		Aen::Entity* light = AEN_NEW(Aen::Entity);

		light->AddComponent<Aen::DirectionalLight>();
		light->GetComponent<Aen::DirectionalLight>().SetColor(input.color[0], input.color[1], input.color[2], 1);
		light->GetComponent<Aen::DirectionalLight>().SetStrength(input.intensity);
		light->SetRot(input.direction[0], input.direction[1], input.direction[2]);

		AddLight(light);
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
				m_levelImporter.ReadFromFile(fileName);
			}
		}
	}
}


