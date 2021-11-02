#include"PCH.h"
#include "ImGuiHandler.h"


namespace Aen {

	AenIF::Room ImGuiHandler::GetRoom(size_t index)
	{
		return m_levelImporter.GetRoomVector()[index].GetRoom();
	}

	ImGuiHandler::ImGuiHandler()
	{
		imguiImporter = new ImGuiImporter(&m_entityList, &m_itemList, &m_modelMap, &m_lightMap);
	}

	ImGuiHandler::~ImGuiHandler()
	{
		if (imguiImporter != nullptr)
		{
			delete imguiImporter;
		}
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
		ImportWindow();

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
				if (ImGui::MenuItem((IGH::OPEN + "..").c_str(), "Ctrl + O"))
				{
					m_openOrSave = IGH::OPEN;
					m_fileDialog.SetTitle(IGH::OPEN.c_str());
					m_fileDialog.Open();
				}

				if (ImGui::MenuItem((IGH::SAVE + "..").c_str(), "Ctrl + S"))
				{
					m_saveWindowActive = true;
				}

				if (ImGui::MenuItem((IGH::COPY + "..").c_str()))
				{
				}

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		m_fileDialog.Display();

		if (m_fileDialog.HasSelected())
		{
			if (m_openOrSave == IGH::OPEN.c_str())
			{
				levelPath = m_fileDialog.GetSelected().string();
				m_ImportWindowActive = true;
				m_openOrSave = "";
			}
			m_fileDialog.ClearSelected();
		}

		ImGui::BeginChild("List");

		if (ImGui::BeginListBox(""))
		{
			for (size_t i = 0; i < m_itemList.size(); i++)
			{
				const bool is_selected = (m_selectedEntity == i);
				if (ImGui::Selectable(m_itemList[i].c_str(), is_selected, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups))
				{
					m_selectedEntity = static_cast<int>(i);
				}

				if (is_selected == true)
				{
					/*if (ImGui::IsItemHovered && ImGui::IsMouseDoubleClicked(0))
					{

					}*/
					ImGui::SetItemDefaultFocus();

				}
			}
			ImGui::EndListBox();
		}
		ImGui::EndChild();
		ImGui::End();
		

	}

	void ImGuiHandler::AssetWindow()
	{
		ImGui::Begin("Assets", nullptr);
		if (ImGui::BeginTabBar("#AssetTab", ImGuiTabBarFlags_None))
		{
			if (ImGui::BeginTabItem(IGH::MODEL.c_str()))
			{
			
				ModelButtons();

				if (AddButton("Delete"))
				{
					RemoveObject();
				}

				ImGui::Columns(1);
				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem(IGH::LIGHT.c_str()))
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
		ImGui::Begin(IGH::COLOR.c_str(), nullptr);
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



		static int selectedHitBoxType = 0;
		static int selectedMaterial = 0;
		static int selectedParent = 0;

		static char materialName[MESH_NAME_MAX_LENGTH];



		SetDefaultValue();

		ImGui::Begin("Properties", nullptr);

		if (ImGui::BeginTabBar("#Property Tab", ImGuiTabBarFlags_None))
		{
			if (ImGui::BeginTabItem(IGH::MODEL.c_str()))
			{
				if (ImGui::CollapsingHeader("Transform Attributes"))
				{
					SetValues();
				}


				ImGui::Checkbox("Hit box", &hitBoxEnable);
				ImGui::Combo("Hit box Type", &selectedHitBoxType, IGH::HITBOXTYPE, IM_ARRAYSIZE(IGH::HITBOXTYPE));

				if (hitBoxEnable == true)
				{
					size_t key = m_entityList[m_selectedEntity]->GetID();
					unordered_map<size_t, IGH::ModelContainer>::iterator it = m_modelMap.find(key);

					if (it != m_modelMap.end())
					{
						it->second.m_model.rigidBody = hitBoxEnable;
						it->second.m_model.rigidBodyType = IGH::HITBOXTYPE[selectedHitBoxType];
					}
				}
				CustomCombo(m_itemList, "Parent", "", selectedParent);
				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem(IGH::MATERIAL.c_str()))
			{
				static bool createNewMaterial = false;

				if (ImGui::CollapsingHeader("Common Material Properties"))
				{
					ImGui::Checkbox("Same Material", &createNewMaterial);

					if (createNewMaterial == true)
					{
						ImGui::InputText((IGH::MATERIAL + " Name").c_str(), materialName, MESH_NAME_MAX_LENGTH);
					}

					if (m_entityList.size() > 0 && m_selectedEntity < m_entityList.size())
					{
						size_t id = m_entityList[m_selectedEntity]->GetID();

						if (Aen::ComponentHandler::MeshInstanceExist(id))
						{
							CustomCombo(m_textureName, "Test", IGH::MATERIAL.c_str(),selectedMaterial);
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

			RoomTab();

			lightTab();
		}
		ImGui::End();
	}

	void ImGuiHandler::ToolWindow()
	{
		static int mode = -1;
		static std::string currentMode = "";

		ImGui::Begin("a", nullptr);
		for (int i = 0; i < 3; i++)
		{
			if (ImGui::Button(IGH::TOOLS[i]))
			{
				mode = i;

				if (mode == 0)
				{
					currentMode = IGH::TOOLS[i];
				}
				else if (mode == 1)
				{
					currentMode = IGH::TOOLS[i];
				}
				else if (mode == 2)
				{
					currentMode = IGH::TOOLS[i];
				}
			}
		}
		ImGui::End();
	}

	void ImGuiHandler::SaveWindow()
	{
		if (m_saveWindowActive == true)
		{
			ImGui::Begin(IGH::SAVE.c_str(), nullptr);
			static char inputString[MESH_NAME_MAX_LENGTH];
			ImGui::InputText("FileName", inputString, MESH_NAME_MAX_LENGTH);

			if (ImGui::Button(IGH::SAVE.c_str()))
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
			static int CurrentIndex = 0;
			static char name[MESH_NAME_MAX_LENGTH];
			static string mesh = "";

			ImGui::Begin("Create Enemy", nullptr);

			ImGui::InputText("Name", name, MESH_NAME_MAX_LENGTH);
			ImGui::Combo("Type", &selectedType, IGH::ENEMYTYPE, IM_ARRAYSIZE(IGH::ENEMYTYPE));

			mesh = CustomCombo(m_objFileName, "Mesh", CurrentIndex);

			if (ImGui::Button("Create"))
			{
				if (mesh != "")
				{
					size_t id = imguiImporter->AddBase(name, mesh);
					m_modelMap.at(id).m_type = IGH::ENEMYTYPE[selectedType];
				}
			
				m_createEnemyWindowActive = false;
				std::memset(name, '\0', sizeof(char) * MESH_NAME_MAX_LENGTH);
			}
			ImGui::End();
		}
	}

	void ImGuiHandler::ImportWindow()
	{
		if (m_ImportWindowActive == true)
		{
			ImGui::Begin("Offset", nullptr);

			static float tempTranslation[3] = { 0,0,0 };
			static float tempRotation[3] = { 0,0,0 };
			static float tempScale[3] = { 1,1,1 };
			//static float roomRotation[2] = { 0,0 };
			//static bool roomRotate = false;


			ImGui::DragFloat3("Translate", tempTranslation, 0.02f);
			ImGui::DragFloat3("Rotate", tempRotation, 0.5f);
			ImGui::DragFloat3("Scale", tempScale, 0.02f);
			//ImGui::DragFloat2("Room Rotation", roomRotation, 0.02f);
			//ImGui::Checkbox("Room Rotation", &roomRotate);
			
			if (ImGui::Button("Import"))
			{
				m_ImportWindowActive = imguiImporter->import(m_levelImporter,levelPath,tempTranslation,tempRotation,tempScale);
			}
			ImGui::End();
		}
	}

	void ImGuiHandler::ReadAllModelsFromHandler()
	{
		if (m_entityList.size() < mp_entityHandlerPtr->m_entities.size())
		{
			for (std::pair<size_t, Aen::Entity*> element : mp_entityHandlerPtr->m_entities)
			{
				m_entityList.push_back(element.second);
				m_itemList.push_back(CheckType(element.second));
				//m_selected.push_back(false);
				//m_selectedEntities.push_back(-1);
				m_OriginalCount++;
			}
		}
	}

	void ImGuiHandler::CreatePreviewTextureThumbnail()
	{
		string filePathDestination = IGH::RESOURCEPATH + "Thumbnail/";
		string filePathSource = IGH::RESOURCEPATH;
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

	//void ImGuiHandler::AddEnemy(Aen::Entity* entity, AenIF::Model& model)
	//{
	//	if (model.type == IGH::NORMALENEMY)
	//	{
	//		OutputDebugStringA(IGH::NORMALENEMY.c_str());
	//	}
	//	else if (model.type == IGH::BOSS.c_str())
	//	{
	//		OutputDebugStringA(IGH::NORMALENEMY.c_str());
	//	}
	//}



	void ImGuiHandler::ChangeMaterial(int& currentIndex)
	{
		size_t id = m_entityList[m_selectedEntity]->GetID();

		string imageName = AEN_RESOURCE_DIR(m_textureFileName[currentIndex]);
		string materialName = IGH::MATERIAL + to_string(m_selectedEntity);
		string textureName = IGH::TEXTURE + to_string(m_selectedEntity);
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

			ImGui::DragFloat3(IGH::TRANSLATION.c_str(), m_xyzTranslation, 0.02f);
			ImGui::DragFloat3(IGH::ROTATION.c_str(), m_xyzTranslation, 0.02f);
			ImGui::DragFloat3(IGH::SCALE.c_str(), m_xyzTranslation, 0.02f);
			ImGui::SliderInt("Number of copies", &copies, 0, 10);


			if (ImGui::Button("Create"))
			{
				active = false;
			}
			ImGui::End();
		}
	}

	void ImGuiHandler::lightTab()
	{
		if (ImGui::BeginTabItem(IGH::LIGHT.c_str()))
		{
			size_t id = m_entityList[m_selectedEntity]->GetID();
			static float color[4];
			static float attenuation[3];
			static float intensity;
			static float range;
			static float angle;

			if (Aen::ComponentHandler::DirectionalLightExist(id))
			{
				Aen::DirectionalLight& lightPtr = Aen::ComponentHandler::GetDirectionalLight(id);

				color[0] = lightPtr.m_light.m_color.r;
				color[1] = lightPtr.m_light.m_color.g;
				color[2] = lightPtr.m_light.m_color.b;
				color[3] = lightPtr.m_light.m_color.a;
				intensity = lightPtr.m_light.m_strength;

				SetValues();

				ImGui::DragFloat4(IGH::COLOR.c_str(), color, 0.02f);
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

				ImGui::DragFloat4(IGH::COLOR.c_str(), color, 0.02f);
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

				ImGui::DragFloat4(IGH::COLOR.c_str(), color, 0.02f);
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

	void ImGuiHandler::RoomTab()
	{
		static int selectedTheme = 0;
		static int selectedType = 0;
		static int selectedSpecial = 0;
		static int probability = 0;
		static int roomSize = 0;

		m_roomProperty[0] = IGH::ROOMTYPE[selectedType];
		m_roomProperty[1] = IGH::SPECIALROOM[selectedSpecial];
		m_roomProperty[2] = IGH::ROOMTHEME[selectedTheme];
		m_roomProperty[3] = to_string(probability);
		m_roomProperty[4] = to_string(roomSize);

		if (ImGui::BeginTabItem("Room"))
		{
			ImGui::Combo("Room Theme", &selectedTheme, IGH::ROOMTHEME, IM_ARRAYSIZE(IGH::ROOMTHEME));
			ImGui::Combo("Room Type", &selectedType, IGH::ROOMTYPE, IM_ARRAYSIZE(IGH::ROOMTYPE));
			ImGui::Combo("Special Room", &selectedSpecial, IGH::SPECIALROOM, IM_ARRAYSIZE(IGH::SPECIALROOM));
			ImGui::DragInt("Probability", &probability, 1, 0, 100, "%.d", 0);
			ImGui::DragInt("Room Size", &roomSize, 1);

			ImGui::EndTabItem();
		}
	}



	void ImGuiHandler::UpdateMap(size_t key, string& texValue, string& matValue, string& meshName, string& texName)
	{
		unordered_map<size_t, IGH::ModelContainer>::iterator it;

		it = m_modelMap.find(key);

		if (it != m_modelMap.end())
		{
			it->second.update(texValue, matValue, texName, meshName);
		}

	}

	void ImGuiHandler::ReadAllFilesFromResourceFolder()
	{
		string filePath = IGH::RESOURCEPATH;
		string fileName = "";
		string fileType = "";

		m_objFileName.reserve(5);
		m_objName.reserve(5);

		for (const auto& entry : std::filesystem::directory_iterator(filePath))
		{
			fileName = entry.path().filename().string();
			fileType = fileName.substr(fileName.find_last_of(".") + 1);

			if (fileType == IGH::OBJ)
			{
				m_objFileName.push_back(fileName);
				m_objName.push_back(fileName.substr(0, fileName.length() - 4));
			} 
			else if(fileType == IGH::FBX)
			{
				m_objFileName.push_back(fileName);
				m_objName.push_back(fileName.substr(0, fileName.length() - 4));
			}
			else if (fileType == IGH::PNG || fileType == IGH::JPG)
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
				imguiImporter->AddBase(m_objName[i], m_objFileName[i]);
			}
			ImGui::NextColumn();
		}
	}

	void ImGuiHandler::LightButtons()
	{
		if (AddButton(IGH::POINTLIGHT.c_str()))
		{
			imguiImporter->AddPointLight();
		}

		if (AddButton(IGH::SPOTLIGHT.c_str()))
		{
			imguiImporter->AddSpotLight();
		}

		if (AddButton(IGH::DIRECTIONALLIGHT.c_str()))
		{
			imguiImporter->AddDirectional();
		}
	}

	const string ImGuiHandler::CheckType(Aen::Entity* entity)
	{
		string type = "";
		size_t id = entity->GetID();

		if (Aen::ComponentHandler::MeshInstanceExist(id))
		{
			type = IGH::MODEL + std::to_string(m_entityCount);
			m_entityCount++;
		}
		else if (Aen::ComponentHandler::CameraExist(id))
		{
			type = IGH::CAMERA;
		}
		else if (Aen::ComponentHandler::DirectionalLightExist(id))
		{
			type = IGH::DIRECTIONALLIGHT;
		}
		else if (Aen::ComponentHandler::SpotLightExist(id))
		{
			type = IGH::SPOTLIGHT;
		}
		else if (Aen::ComponentHandler::PointLightExist(id))
		{
			type = IGH::POINTLIGHT;
		}
		else
		{
			type = "Asset" + std::to_string(m_entityCount);
			m_entityCount++;
		}
		return type;
	}

	void ImGuiHandler::CustomCombo(vector<string>& list, string name, string type, int& index)
	{

		if (ImGui::BeginCombo(name.c_str(), list[index].c_str()))
		{
			for (size_t i = 0; i < list.size(); i++)
			{
				static bool isSelected = (index == i);

				if (ImGui::Selectable(list[i].data(), isSelected, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups))
				{
					index = static_cast<int>(i);
					
					if (type == IGH::MATERIAL)
					{
						ChangeMaterial(index);
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

	string ImGuiHandler::CustomCombo(vector<string>& list, string name, int& index)
	{

		static string value = "";

		if (ImGui::BeginCombo(name.c_str(), list[index].c_str()))
		{
			static bool isSelected = false;
			for (size_t i = 0; i < list.size(); i++)
			{
				isSelected = (index == i);

				if (ImGui::Selectable(list[i].data(), isSelected, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups))
				{
					index = static_cast<int>(i);
					value = list[index];
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
			//m_selected.erase(m_selected.begin() + m_selectedEntity);
			m_selectedEntity--;
		}
	}
}