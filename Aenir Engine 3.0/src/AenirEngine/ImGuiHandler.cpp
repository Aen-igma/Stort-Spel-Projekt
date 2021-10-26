#include"PCH.h"
#include "ImGuiHandler.h"

namespace Aen {

	AenIF::Room ImGuiHandler::GetRoom(size_t index)
	{
		return m_levelImporter.GetRoomVector()[index].GetRoom();
	}

	void ImGuiHandler::AddLight(Aen::Entity* entity)
	{
		m_entityList.push_back(entity);
		m_itemList.push_back("light " + std::to_string(m_lightCount));
		m_selectedEntities.push_back(false);
		m_lightCount++;
	}

	void ImGuiHandler::AddBase(AenIF::Model& model, AenIF::Texture& texture)
	{
		string imageName = AEN_RESOURCE_DIR(texture.name);
		string materialName = "Material" + to_string(m_entityCount);
		string textureName = "Texture" + to_string(m_entityCount);

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

		AddModel(entity, model.name);
		m_modelMap.insert(std::make_pair(entity->GetID(), ModelContainer(textureName, materialName, texture.name, model.name, model.mesh )));
	}

	void ImGuiHandler::AddPointLight(AenIF::Light& input)
	{
		Aen::Entity* light = &mp_entityHandlerPtr->CreateEntity();


		light->AddComponent<Aen::PointLight>();
		light->GetComponent<Aen::PointLight>().SetColor(input.color[0], input.color[1], input.color[2], 1);
		light->GetComponent<Aen::PointLight>().SetLightDist(input.attenuation[0], input.attenuation[1], input.attenuation[2], input.range);
		light->GetComponent<Aen::PointLight>().SetStrength(100);
		light->SetPos(input.translation[0], input.translation[1], input.translation[2]);

		AddLight(light, "Point light");
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

		AddLight(light, "Spot light");
	}

	void ImGuiHandler::AddDirectional(AenIF::Light& input)
	{
		Aen::Entity* light = &mp_entityHandlerPtr->CreateEntity();

		light->AddComponent<Aen::DirectionalLight>();
		light->GetComponent<Aen::DirectionalLight>().SetColor(input.color[0], input.color[1], input.color[2], 1);
		light->GetComponent<Aen::DirectionalLight>().SetStrength(input.intensity);
		light->SetRot(input.direction[0], input.direction[1], input.direction[2]);

		AddLight(light, "Directional light");
	}

	void ImGuiHandler::AddEnemy()
	{
		EnemyCreateWindow();
	}


	ImGuiHandler::ImGuiHandler()
	{
	}

	ImGuiHandler::~ImGuiHandler()
	{
	}

	void ImGuiHandler::SaveThumbnail(string& destinationPath, string& filePathDestination,
		string& sourcePath, string& filePathSource,
		Aen::ImageByteData& source, Aen::ImageByteData& destination, int& i)
	{
		destinationPath = filePathDestination + m_textureName[i] + "_Thumbnail.png";
		sourcePath = filePathSource + m_textureFileName[i];

		source.LoadImageThumb(sourcePath.c_str());
		source.MakeThumbnail(&destination);
		destination.SaveImage(destinationPath.c_str());

		source.FreeData();
		destination.FreeData();
	}

	void ImGuiHandler::StartUp()
	{
		ReadAllModelsFromHandler();
		ReadAllFilesFromResourceFolder();
		//CreatePreviewTextureThumbnail();
	}

	void ImGuiHandler::RenderAllWindows()
	{
		SceneListWindow();
		update();
		AssetWindow();
		PropertyWindow();
		ToolWindow();
		SaveWindow();
		EnemyCreateWindow();
	}

	void ImGuiHandler::Initialize(const HWND& hwnd, ID3D11Device* mp_device, ID3D11DeviceContext* mp_dContext)
	{
		// SetUp ImGui
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		ImGui_ImplWin32_Init(hwnd);
		ImGui_ImplDX11_Init(mp_device, mp_dContext);
		ImGui::StyleColorsDark();
	}

	void ImGuiHandler::NewFrame()
	{
		// Start the ImGui frame
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
	}

	void ImGuiHandler::Render()
	{
		// Draw
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}

	void ImGuiHandler::Release()
	{
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiHandler::SceneListWindow()
	{
		ImGui::Begin("Scene", nullptr, ImGuiWindowFlags_MenuBar);

		static string m_openOrSave = "";

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Open..", "Ctrl + O"))
				{
					m_openOrSave = "Open";
					m_fileDialog.SetTitle("Open");
					m_fileDialog.Open();
				}

				if (ImGui::MenuItem("Save..", "Ctrl + S"))
				{
					m_saveWindowActive = true;
				}

				if (ImGui::MenuItem("Copy.."))
				{
				}

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		m_fileDialog.Display();

		if (m_fileDialog.HasSelected())
		{
			if (m_openOrSave == "Open")
			{
				m_levelImporter.ReadFromFile(m_fileDialog.GetSelected().string());
				
				for (size_t i = 0; i < m_levelImporter.GetRoomVector()[0].GetModelVector().size(); i++)
				{
					AddBase(m_levelImporter.GetRoomVector()[0].GetModelVector()[i], m_levelImporter.GetRoomVector()[0].GetTextureVector()[i]);
				}

				for (size_t i = 0; i < m_levelImporter.GetRoomVector()[0].GetLightVector().size(); i++)
				{
					if (m_levelImporter.GetRoomVector()[0].GetLightVector()[i].type == "Directional light")
					{
						AddDirectional(m_levelImporter.GetRoomVector()[0].GetLightVector()[i]);
					}
					else if (m_levelImporter.GetRoomVector()[0].GetLightVector()[i].type == "Spot light")
					{
						AddSpotLight(m_levelImporter.GetRoomVector()[0].GetLightVector()[i]);
					}
					else if (m_levelImporter.GetRoomVector()[0].GetLightVector()[i].type == "Point light")
					{
						AddPointLight(m_levelImporter.GetRoomVector()[0].GetLightVector()[i]);
					}
				}
				m_levelImporter.GetRoomVector().clear();
				m_openOrSave = "";
			}
			m_fileDialog.ClearSelected();
		}

		ImGui::BeginChild("List");

		static string previewValue = m_itemList[0];

		static bool* selected = new bool[m_selectedEntities.size()];

		for (int i = 0; i < m_selectedEntities.size(); i++)
		{
			selected[i] = m_selectedEntities[i];
		}



		if (ImGui::BeginCombo("",previewValue.c_str()))
		{
			previewValue = "";
			vector<string> vec;

			for (size_t i = 0; i < m_itemList.size(); i++)
			{
				ImGui::Selectable(m_itemList[i].c_str(), &selected[i], ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);

				if (selected[i] == true)
				{
					vec.push_back(m_itemList[i]);
				}
			}
			for (size_t i = 0; i < vec.size(); i++)
			{
				if (vec.size() == 1)
				{
					previewValue += vec[i];

				}
				else if(!(i == vec.size()))
				{
					previewValue += vec[i] + ",";
				}
				else
				{
					previewValue += vec[i];

				}

			}

			ImGui::EndCombo();

			for (int i = 0; i < m_selectedEntities.size(); i++)
			{
				m_selectedEntities[i] = selected[i];
				string test = i + " " + std::to_string(m_selectedEntities[i]) + "\n";
				OutputDebugStringA(test.c_str());
			}
		}
		ImGui::EndChild();
		ImGui::End();

	}

	void ImGuiHandler::AssetWindow()
	{
		ImGui::Begin("Assets", nullptr);
		if (ImGui::BeginTabBar("#AssetTab", ImGuiTabBarFlags_None))
		{
			if (ImGui::BeginTabItem("Model"))
			{
			
				ModelButtons();

				if (AddButton("Delete"))
				{
					RemoveObject();
				}

				ImGui::Columns(1);
				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Light"))
			{
				LightButtons();
				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Entities"))
			{
				AddEnemyButton();
				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();
		}
		ImGui::End();
	}

	void ImGuiHandler::ColorWheel()
	{
		ImGui::Begin("Color", nullptr);
		static float color[3] = { 255,255,255 };
		ImGui::ColorPicker3("##picker", color, ImGuiColorEditFlags_PickerHueWheel);
		ImGui::End();
	}

	void ImGuiHandler::PropertyWindow()
	{
		static bool hitBoxEnable = false;
		static float albedoColor = 0;
		static float ambientColor = 0;
		static float transparency = 0;
		static float diffuse = 0;

		static float specularColor = 0;
		static float reflectivity = 0;

		static int selectedTheme = 0;
		static int selectedType = 0;
		static int selectedSpecial = 0;
		static int probability = 0;

		const char* roomTheme[] = { "Normal Dungeon", "Skeleton", "Gothic", "Aztec"};
		const char* roomType[] = { "Straight", "Bend", "T Junction" ,"Four Way", "None"};
		const char* specialRoom[] = { "Normal", "Entrance", "Exit", "Boss", "Arena", "Item"};

		static char materialName[MESH_NAME_MAX_LENGTH];

		m_roomProperty[0] = roomType[selectedType];
		m_roomProperty[1] = specialRoom[selectedSpecial];
		m_roomProperty[2] = roomTheme[selectedTheme];
		m_roomProperty[3] = to_string(probability);


		//string imageName = AEN_RESOURCE_DIR("Place_Holder.png");

		//Aen::Texture& texture = Aen::Resource::CreateTexture("ImGuiTexture");
		//texture.LoadTexture(imageName)*/;

		//// texture friend
		////static ImVec2 imageSize = ImVec2(50, 50);
		////ImGui::Image("Display", imageSize,)

		SetDefaultValue();

		ImGui::Begin("Properties", nullptr);

		if (ImGui::BeginTabBar("#Property Tab", ImGuiTabBarFlags_None))
		{
			if (ImGui::BeginTabItem("Mesh"))
			{
				if (ImGui::CollapsingHeader("Transform Attributes"))
				{
					SetValues();
				}

				ImGui::Checkbox("Hit box", &hitBoxEnable);
				CustomCombo(m_itemList,"Parent" ,"");

				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Material"))
			{
				static bool createNewMaterial = false;

				if (ImGui::CollapsingHeader("Common Material Properties"))
				{
					ImGui::Checkbox("Same Material", &createNewMaterial);

					if (createNewMaterial == true)
					{
						ImGui::InputText("Material Name", materialName, MESH_NAME_MAX_LENGTH);
					}

					if (m_entityList.size() > 0 && m_selectedEntity < m_entityList.size())
					{
						size_t id = m_entityList[m_selectedEntity]->GetID();

						if (Aen::ComponentHandler::MeshInstanceExist(id))
						{
							CustomCombo(m_textureName, "Test", "Material");
							ImGui::SliderFloat("Transparency", &transparency, 0.0f, 1.0f);
							ImGui::SliderFloat("Ambient Color", &ambientColor, 0.0f, 1.0f);
							ImGui::SliderFloat("Diffuse", &diffuse, 0.0f, 1.0f);
						}
					}
				}

				/*if (ImGui::CollapsingHeader("Normal Mapping"))
				{
					ImGui::InputText("Map", const_cast<char*>(mapName.c_str()), IM_ARRAYSIZE(mapName.c_str()));
				}*/

				if (ImGui::CollapsingHeader("Specular Shading"))
				{
					ImGui::SliderFloat("Specular Color", &specularColor, 0.0f, 1.0f);
					ImGui::SliderFloat("Reflectivity", &reflectivity, 0.0f, 1.0f);
				}
				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Room"))
			{
				ImGui::Combo("Room Theme", &selectedTheme, roomTheme, IM_ARRAYSIZE(roomTheme));

				ImGui::Combo("Room Type", &selectedType, roomType, IM_ARRAYSIZE(roomType));

				ImGui::Combo("Special Room", &selectedSpecial, specialRoom, IM_ARRAYSIZE(specialRoom));

				ImGui::DragInt("Probability", &probability, 1, 0, 100, "%.d", 0);

				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Light"))
			{
				size_t id = m_entityList[m_selectedEntity]->GetID();
				static float color[4];
				static float attenuation[3];
				static float intensity;
				static float range;
				static float angle;

				if (Aen::ComponentHandler::DirectionalLightExist(id))
				{
					Aen::DirectionalLight &lightPtr = Aen::ComponentHandler::GetDirectionalLight(id);

					color[0] = lightPtr.m_light.m_color.r;
					color[1] = lightPtr.m_light.m_color.g;
					color[2] = lightPtr.m_light.m_color.b;
					color[3] = lightPtr.m_light.m_color.a;
					intensity = lightPtr.m_light.m_strength;
					
					SetValues();

					ImGui::DragFloat4("Color", color, 0.02f);
					ImGui::DragFloat("Intensity", &intensity, 0.02f);

					lightPtr.SetColor(color[0], color[1], color[2], color[3]);
					lightPtr.SetStrength(intensity);
				}

				if (Aen::ComponentHandler::PointLightExist(id))
				{
					Aen::PointLight& lightPtr = Aen::ComponentHandler::GetPointLight(id);

					color[0] = lightPtr.m_light.m_color.r;
					color[1] = lightPtr.m_light.m_color.g;
					color[2] = lightPtr.m_light.m_color.b;
					color[3] = lightPtr.m_light.m_color.a;

					intensity = lightPtr.m_light.m_strength;

					attenuation[0] = lightPtr.m_light.m_dist.x;
					attenuation[1] = lightPtr.m_light.m_dist.y;
					attenuation[2] = lightPtr.m_light.m_dist.z;

					range = lightPtr.m_light.m_dist.w;

					SetValues();

					ImGui::DragFloat4("Color", color, 0.02f);
					ImGui::DragFloat("Intensity", &intensity, 0.02f);
					ImGui::DragFloat3("Attenuation", attenuation, 0.02f);
					ImGui::DragFloat("Range", &range, 0.02f);

					lightPtr.SetColor(color[0], color[1], color[2], color[3]);
					lightPtr.SetStrength(intensity);
					lightPtr.SetLightDist(attenuation[0], attenuation[1], attenuation[2], range);
				}

				if (Aen::ComponentHandler::SpotLightExist(id))
				{
					Aen::SpotLight& lightPtr = Aen::ComponentHandler::GetSpotLight(id);

					color[0] = lightPtr.m_light.m_color.r;
					color[1] = lightPtr.m_light.m_color.g;
					color[2] = lightPtr.m_light.m_color.b;
					color[3] = lightPtr.m_light.m_color.a;

					intensity = lightPtr.m_light.m_strength;

					attenuation[0] = lightPtr.m_light.m_dist.x;
					attenuation[1] = lightPtr.m_light.m_dist.y;
					attenuation[2] = lightPtr.m_light.m_dist.z;

					range = lightPtr.m_light.m_dist.w;
					angle = lightPtr.m_light.m_angle;

					SetValues();

					ImGui::DragFloat4("Color", color, 0.02f);
					ImGui::DragFloat("Intensity", &intensity, 0.02f);
					ImGui::DragFloat3("Attenuation", attenuation, 0.02f);
					ImGui::DragFloat("Range", &range, 0.02f);
					ImGui::DragFloat("angle", &angle, 0.02f);

					lightPtr.SetColor(color[0], color[1], color[2], color[3]);
					lightPtr.SetStrength(intensity);
					lightPtr.SetLightDist(attenuation[0], attenuation[1], attenuation[2], range);
					lightPtr.SetConeSize(angle);
				}
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}
		ImGui::End();
	}

	void ImGuiHandler::ToolWindow()
	{
		static int mode = -1;
		static const std::string stringArray[3] = { "Move","Rotate","Scale" };
		static std::string currentMode = "";

		ImGui::Begin("a", nullptr);
		for (int i = 0; i < 3; i++)
		{
			if (ImGui::Button(stringArray[i].c_str()))
			{
				mode = i;

				if (mode == 0)
				{
					currentMode = stringArray[0];
				}
				else if (mode == 1)
				{
					currentMode = stringArray[1];
				}
				else if (mode == 2)
				{
					currentMode = stringArray[2];
				}
			}
		}
		ImGui::End();
	}

	void ImGuiHandler::SaveWindow()
	{
		if (m_saveWindowActive == true)
		{
			ImGui::Begin("Save", nullptr);
			static char inputString[MESH_NAME_MAX_LENGTH];
			ImGui::InputText("FileName", inputString, MESH_NAME_MAX_LENGTH);

			if (ImGui::Button("Save"))
			{
				string input = inputString;
				m_levelExporter.WriteInto(m_entityList, m_itemList, m_modelMap, m_lightMap, m_roomProperty, input);
				
				m_saveWindowActive = false;
				std::memset(inputString, '\0', sizeof(char) * MESH_NAME_MAX_LENGTH);
			}
			ImGui::End();
		}
	}

	void ImGuiHandler::EnemyCreateWindow()
	{
		if (m_createEnemyWindowActive == true)
		{
			static int selectedType = 0;
			static char name[MESH_NAME_MAX_LENGTH];
			const char* type[] = { "Normal Enemy", "Boss Enemy" };
			static string mesh = "";

			ImGui::Begin("Create Enemy", nullptr);

			ImGui::InputText("Name", name, MESH_NAME_MAX_LENGTH);
			ImGui::Combo("Type", &selectedType, type, IM_ARRAYSIZE(type));

			mesh = CustomCombo(m_objFileName, "Mesh");

			if (ImGui::Button("Create"))
			{
				if (mesh != "")
				{
					AddBase(name, mesh);
				}
			
				m_createEnemyWindowActive = false;
				std::memset(name, '\0', sizeof(char) * MESH_NAME_MAX_LENGTH);
			}
			ImGui::End();
		}
	}

	void ImGuiHandler::AddModel(Aen::Entity* entity, string name)
	{
		m_entityList.push_back(entity);
		m_itemList.push_back(name + std::to_string(m_entityCount));
		m_selectedEntities.push_back(false);
		m_entityCount++;
	}

	void ImGuiHandler::AddModel(Aen::Entity* entity)
	{
		m_entityList.push_back(entity);
		m_itemList.push_back("Model" + std::to_string(m_entityCount));
		m_selectedEntities.push_back(false);
		m_entityCount++;
	}

	void ImGuiHandler::AddLight(Aen::Entity* entity, string type)
	{
		m_lightMap.insert(std::make_pair(entity->GetID(), entity));
		m_entityList.push_back(entity);
		m_itemList.push_back(type + std::to_string(m_lightCount));
		m_selectedEntities.push_back(false);

		m_lightCount++;
	}

	void ImGuiHandler::ReadAllModelsFromHandler()
	{
		if (m_entityList.size() < mp_entityHandlerPtr->m_entities.size())
		{
			for (std::pair<size_t, Aen::Entity*> element : mp_entityHandlerPtr->m_entities)
			{
				m_entityList.push_back(element.second);
				m_itemList.push_back(CheckType(element.second));
				m_selectedEntities.push_back(false);
				m_OriginalCount++;
			}
		}
	}

	void ImGuiHandler::CreatePreviewTextureThumbnail()
	{
		string filePathDestination = "../Resource/Thumbnail/";
		string filePathSource = "../Resource/";
		string destinationPath;
		string sourcePath;

		string fileNameDestination = "";

		Aen::ImageByteData destination;
		destination.x = 50;
		destination.y = 50;
		destination.channels = 4;

		Aen::ImageByteData source;

		if (std::filesystem::is_empty(filePathDestination))
		{
			for (int i = 0; i < m_textureFileName.size(); i++)
			{
				SaveThumbnail(destinationPath, filePathDestination,
					sourcePath, filePathSource,
					source, destination, i);
			}
		}
		else
		{
			int numberOfFile = 0;

			for (int i = 0; i < m_textureFileName.size(); i++)
			{
				for (const auto& entryTwo : std::filesystem::directory_iterator(filePathDestination))
				{
					fileNameDestination = entryTwo.path().filename().string();

					if ((m_textureName[i] == fileNameDestination.substr(0, fileNameDestination.length() - (4 + 10))) != true)
					{
						SaveThumbnail(destinationPath, filePathDestination,
							sourcePath, filePathSource,
							source, destination, i);
					}
					else
					{
						cout << "Already exist" << endl;
					}
				}
			}
		}
	}

	void ImGuiHandler::SetDefaultValue()
	{
		if (m_entityList.size() > 0 && m_selectedEntity < m_entityList.size())
		{
			size_t id = m_entityList[m_selectedEntity]->GetID();

			if (Aen::ComponentHandler::TranslationExist(id))
			{
				m_xyzTranslation[0] = m_entityList[m_selectedEntity]->GetPos().x;
				m_xyzTranslation[1] = m_entityList[m_selectedEntity]->GetPos().y;
				m_xyzTranslation[2] = m_entityList[m_selectedEntity]->GetPos().z;
			}
			else
			{
				for (int i = 0; i < 3; i++)
				{
					m_xyzTranslation[i] = 0;
				}
			}

			if (Aen::ComponentHandler::RotationExist(id))
			{
				m_xyzRotation[0] = m_entityList[m_selectedEntity]->GetRot().x;
				m_xyzRotation[1] = m_entityList[m_selectedEntity]->GetRot().y;
				m_xyzRotation[2] = m_entityList[m_selectedEntity]->GetRot().z;
			}
			else
			{
				for (int i = 0; i < 3; i++)
				{
					m_xyzRotation[i] = 0;
				}
			}

			if (Aen::ComponentHandler::ScaleExist(id))
			{
				m_xyzScale[0] = m_entityList[m_selectedEntity]->GetScale().x;
				m_xyzScale[1] = m_entityList[m_selectedEntity]->GetScale().y;
				m_xyzScale[2] = m_entityList[m_selectedEntity]->GetScale().z;
			}
			else
			{
				for (int i = 0; i < 3; i++)
				{
					if ((Aen::ComponentHandler::DirectionalLightExist(id) || Aen::ComponentHandler::SpotLightExist(id) || Aen::ComponentHandler::PointLightExist(id)))
					{
						m_xyzScale[i] = 0;
					}
					else
					{
						m_xyzScale[i] = 1;
					}
				}
			}
		}
		else
		{
			ZeroValue();
		}
	}

	void ImGuiHandler::SetValues()
	{
		if (m_entityList.size() > 0 && m_selectedEntity < m_entityList.size())
		{
			size_t id = m_entityList[m_selectedEntity]->GetID();

			if (Aen::ComponentHandler::TranslationExist(id))
			{
				ImGui::DragFloat3("Translate", m_xyzTranslation, 0.02f);
				m_entityList[m_selectedEntity]->SetPos(m_xyzTranslation[0], m_xyzTranslation[1], m_xyzTranslation[2]);
			}

			if (Aen::ComponentHandler::RotationExist(id))
			{
				ImGui::DragFloat3("Rotate", m_xyzRotation, 0.5f);
				m_entityList[m_selectedEntity]->SetRot(m_xyzRotation[0], m_xyzRotation[1], m_xyzRotation[2]);
			}

			if (Aen::ComponentHandler::ScaleExist(id))
			{
				ImGui::DragFloat3("Scale", m_xyzScale, 0.02f);
				m_entityList[m_selectedEntity]->SetScale(m_xyzScale[0], m_xyzScale[1], m_xyzScale[2]);
			}

		}
		else
		{
			ZeroValue();
		}
	}

	void ImGuiHandler::ZeroValue()
	{
		for (int i = 0; i < 3; i++)
		{
			m_xyzTranslation[i] = 0;
			m_xyzRotation[i] = 0;
			m_xyzScale[i] = 1;
		}
	}



	void ImGuiHandler::AddBase(const string& meshName, const string& objName)
	{
		string imageName = AEN_RESOURCE_DIR("Missing_Textures.png");
		string materialName = "Material" + to_string(m_entityCount);
		string textureName = "Texture" + to_string(m_entityCount);


		Aen::Entity* entity = &mp_entityHandlerPtr->CreateEntity();
		Aen::Mesh& mesh = Aen::Resource::CreateMesh(meshName + std::to_string(m_entityCount));
		mesh.Load(AEN_RESOURCE_DIR(objName));

		entity->AddComponent<Aen::MeshInstance>();
		entity->GetComponent<Aen::MeshInstance>().SetMesh(mesh);


		AddModel(entity, objName);
		m_modelMap.insert(std::make_pair(entity->GetID(), ModelContainer(textureName, materialName, imageName, meshName, objName)));

	}

	
	void ImGuiHandler::AddPointLight()
	{
		Aen::Entity* light = &mp_entityHandlerPtr->CreateEntity();

		light->AddComponent<Aen::PointLight>();
		light->GetComponent<Aen::PointLight>().SetColor(Aen::Color::White);
		light->GetComponent<Aen::PointLight>().SetLightDist(1, 1, 1, 1);
		light->GetComponent<Aen::PointLight>().SetStrength(100);
		light->SetPos(0.0f, 0.0f, 0.0f);

		AddLight(light, "Point light");			
	}

	void ImGuiHandler::AddSpotLight()
	{
		Aen::Entity* light = &mp_entityHandlerPtr->CreateEntity();

		light->AddComponent<Aen::SpotLight>();
		light->GetComponent<Aen::SpotLight>().SetColor(Aen::Color::White);
		light->GetComponent<Aen::SpotLight>().SetConeSize(1);
		light->GetComponent<Aen::SpotLight>().SetLightDist(1, 1, 1, 1);
		light->GetComponent<Aen::SpotLight>().SetStrength(100);
		light->SetPos(0.0f, 0.0f, 0.0f);
		light->SetRot(0.0f, 0.0f, 0.0f);

		AddLight(light, "Spot light");
	}

	void ImGuiHandler::AddDirectional()
	{
		Aen::Entity* light = &mp_entityHandlerPtr->CreateEntity();

		light->AddComponent<Aen::DirectionalLight>();
		light->GetComponent<Aen::DirectionalLight>().SetColor(Aen::Color::White);
		light->GetComponent<Aen::DirectionalLight>().SetStrength(1);
		light->SetRot(0.0f, 0.0f, 0.0f);

		AddLight(light, "Directional light");
	}

	void ImGuiHandler::ChangeMaterial(int& currentIndex)
	{
		size_t id = m_entityList[m_selectedEntity]->GetID();

		string imageName = AEN_RESOURCE_DIR(m_textureFileName[currentIndex]);
		string materialName = "Material" + to_string(m_selectedEntity);
		string textureName = "Texture" + to_string(m_selectedEntity);
		UpdateMap(id, textureName, materialName, m_itemList[m_selectedEntity], m_textureFileName[currentIndex]);

		Aen::Texture& texture = Aen::Resource::CreateTexture(textureName);
		texture.LoadTexture(imageName);
		Aen::Material& mat = Aen::Resource::CreateMaterial(materialName, true);
		mat.SetDiffuseMap(texture);

		Aen::ComponentHandler::GetMeshInstance(id).SetMaterial(mat);
	}

	void ImGuiHandler::update()
	{
		/*unordered_map <size_t, Aen::Entity*>::iterator it;

		if (m_OriginalCount < mp_entityHandlerPtr->m_entities.size())
		{
			for (size_t i = m_OriginalCount; i < mp_entityHandlerPtr->m_entities.size(); i++)
			{
				it = mp_entityHandlerPtr->m_entities.begin();
				std::advance(it, i);
				m_entityList.push_back(it->second);
				m_itemList.push_back(CheckType(it->second));
				m_OriginalCount++;
			}
		}*/
	}

	void ImGuiHandler::DuplicateWindow(bool &active)
	{
		if (active == true)
		{
			static float translation[3] = { 0 };
			static float rotation[3] = { 0 };
			static float scale[3] = { 0 };
			static int copies = 0;

			ImGui::Begin("Copy", nullptr);

			ImGui::DragFloat3("Translate", m_xyzTranslation, 0.02f);
			ImGui::DragFloat3("Rotate", m_xyzTranslation, 0.02f);
			ImGui::DragFloat3("Scale", m_xyzTranslation, 0.02f);
			ImGui::SliderInt("Number of copies", &copies, 0, 10);


			if (ImGui::Button("Create"))
			{
				active = false;
			}
			ImGui::End();
		}
	}

	void ImGuiHandler::UpdateMap(size_t key, string& texValue, string& matValue, string& meshName, string& texName)
	{
		unordered_map<size_t, ModelContainer>::iterator it;

		it = m_modelMap.find(key);

		if (it != m_modelMap.end())
		{
			//it->second = ModelContainer(texValue, matValue, texName, meshName);
			it->second.update(texValue, matValue, texName, meshName);

		}

	}

	void ImGuiHandler::ReadAllFilesFromResourceFolder()
	{
		string filePath = "../Resource/";
		string fileName = "";
		string fileType = "";

		m_objFileName.reserve(5);
		m_objName.reserve(5);

		for (const auto& entry : std::filesystem::directory_iterator(filePath))
		{
			fileName = entry.path().filename().string();
			fileType = fileName.substr(fileName.find_last_of(".") + 1);

			if (fileType == "obj")
			{
				m_objFileName.push_back(fileName);
				m_objName.push_back(fileName.substr(0, fileName.length() - 4));
			} 
			else if(fileType == "fbx")
			{
			
			}
			else if (fileType == "png" || fileType == "jpg")
			{
				m_textureFileName.push_back(fileName);
				m_textureName.push_back(fileName.substr(0, fileName.length() - 4));
			}
		}
	}

	bool ImGuiHandler::AddButton(const string& name)
	{
		return ImGui::Button(name.c_str());
	}

	void ImGuiHandler::AddEnemyButton()
	{
		if (AddButton("Enemy"))
		{
			m_createEnemyWindowActive = true;
		}
	}

	void ImGuiHandler::ModelButtons()
	{
		for (size_t i = 0; i < m_objFileName.size(); i++)
		{
			ImGui::Columns(4, nullptr);
			if (AddButton(m_objName[i]))
			{
				AddBase(m_objName[i], m_objFileName[i]);
			}
			ImGui::NextColumn();
		}
	}

	void ImGuiHandler::LightButtons()
	{
		if (AddButton("Point Light"))
		{
			AddPointLight();
		}

		if (AddButton("Spot Light"))
		{
			AddSpotLight();
		}

		if (AddButton("Directional light"))
		{
			AddDirectional();
		}
	}

	const string ImGuiHandler::CheckType(Aen::Entity* entity)
	{
		string type = "";
		size_t id = entity->GetID();

		if (Aen::ComponentHandler::MeshInstanceExist(id))
		{
			type = "Model" + std::to_string(m_entityCount);
			m_entityCount++;
		}
		else if (Aen::ComponentHandler::CameraExist(id))
		{
			type = "Camera";
		}
		else if (Aen::ComponentHandler::DirectionalLightExist(id))
		{
			type = "Directional light";
		}
		else if (Aen::ComponentHandler::SpotLightExist(id))
		{
			type = "Spot light";
		}
		else if (Aen::ComponentHandler::PointLightExist(id))
		{
			type = "Point light";
		}
		else
		{
			type = "Asset" + std::to_string(m_entityCount);
			m_entityCount++;
		}
		return type;
	}

	void ImGuiHandler::CustomCombo(vector<string>& list, string name, string type)
	{
		static int currentIndex = 0;

		if (ImGui::BeginCombo(name.c_str(), list[currentIndex].c_str()))
		{
			for (size_t i = 0; i < list.size(); i++)
			{
				static bool isSelected = (currentIndex == i);

				if (ImGui::Selectable(list[i].data(), isSelected, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups))
				{
					currentIndex = static_cast<int>(i);
					
					if (type == "Material")
					{
						ChangeMaterial(currentIndex);
					}
				}

				if (isSelected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}
	}

	string ImGuiHandler::CustomCombo(vector<string>& list, string name)
	{
		static int currentIndex = 0;
		static string value = "";

		if (ImGui::BeginCombo(name.c_str(), list[currentIndex].c_str()))
		{
			static bool isSelected = false;
			for (size_t i = 0; i < list.size(); i++)
			{
				isSelected = (currentIndex == i);

				if (ImGui::Selectable(list[i].data(), isSelected, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups))
				{
					currentIndex = static_cast<int>(i);
					value = list[currentIndex];
				}

				if (isSelected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}
		return value;
	}

	void ImGuiHandler::RemoveObject()
	{
		size_t id = m_entityList[m_selectedEntity]->GetID();
		if (!Aen::ComponentHandler::CameraExist(id))
		{
			if (m_modelMap.find(id) != m_modelMap.end())
			{
				m_modelMap.erase(id);
			}

			if (m_lightMap.find(id) != m_lightMap.end())
			{
				m_lightMap.erase(id);
			}

			mp_entityHandlerPtr->RemoveEntity(*m_entityList[m_selectedEntity]);
			m_entityList.erase(m_entityList.begin() + m_selectedEntity);
			m_itemList.erase(m_itemList.begin() + m_selectedEntity);
			m_selectedEntities.erase(m_selectedEntities.begin() + m_selectedEntity);
			m_selectedEntity--;
		}
	}
}