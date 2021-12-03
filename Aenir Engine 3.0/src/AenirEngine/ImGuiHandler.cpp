#include"PCH.h"
#include "ImGuiHandler.h"


namespace Aen {

	AenIF::Room ImGuiHandler::GetRoom(size_t index)
	{
		return m_levelImporter.GetRoomVector()[index].GetRoom();
	}

	void ImGuiHandler::MaterialDragFloats(float* baseColor, float* shadowColor, float* specularColor, float* rimLightColor, float* innerEdgeColor, float* outerEdgeColor, float* glowColor, float& glowStr, float& innerEdgeThickness, float& outerEdgeThickness, float& specularPower, float& specularStrength, float& roughness, float& shadowOffset, float& innerFalloff, float& outerFalloff, float& rimLightIntensity, float& rimLightSize)
	{
		ImGui::DragFloat4("Base Color", baseColor, 0.02f);
		ImGui::DragFloat4("Shadow Color", shadowColor, 0.02f);
		ImGui::DragFloat4("Specular Color", specularColor, 0.02f);

		ImGui::DragFloat4("RimLight Color", rimLightColor, 0.02f);
		ImGui::DragFloat4("Inner EdgeColor", innerEdgeColor, 0.02f);
		ImGui::DragFloat4("Outer EdgeColor", outerEdgeColor, 0.02f);
		ImGui::DragFloat4("Glow Color", glowColor, 0.02f);
		ImGui::DragFloat("Glow GlowStr", &glowStr, 0.02f, 0.0f, 100.0f);
		ImGui::DragFloat("InnerEdge Thickness", &innerEdgeThickness, 1.0f, 0.0f, 100.0f);
		ImGui::DragFloat("OuterEdge Thickness", &outerEdgeThickness, 1.0f, 0.0f, 100.0f);
		ImGui::DragFloat("Specular Power", &specularPower, 0.02f, 0.0f, 1.0f);
		ImGui::DragFloat("Specular Strength", &specularStrength, 0.02f, 0.0f, 1.0f);
		ImGui::DragFloat("Roughness", &roughness, 0.02f, 0.0f, 1.0f);
		ImGui::DragFloat("Shadow Offset", &shadowOffset, 0.02f, 0.0f, 1.0f);
		ImGui::DragFloat("Inner Falloff", &innerFalloff, 0.02f, 0.0f, 1.0f);
		ImGui::DragFloat("Outer Falloff", &outerFalloff, 0.02f, 0.0f, 1.0f);
		ImGui::DragFloat("RimLight Intensity", &rimLightIntensity, 0.02f, 0.0f, 1.0f);
		ImGui::DragFloat("RimLight Size", &rimLightSize, 0.02f, 0.0f, 1.0f);
	}

	void ImGuiHandler::SetMaterialValues(float* baseColor, float* shadowColor, float* specularColor, float* rimLightColor, float* innerEdgeColor, float* outerEdgeColor, float* glowColor, float& glowStr, float& innerEdgeThickness, float& outerEdgeThickness, float& specularPower, float& specularStrength, float& roughness, float& shadowOffset, float& innerFalloff, float& outerFalloff, float& rimLightIntensity, float& rimLightSize, Aen::Material& material)
	{
		material["BaseColor"] = Color(baseColor[0], baseColor[1], baseColor[2], baseColor[3]);
		material["ShadowColor"] = Color(shadowColor[0], shadowColor[1], shadowColor[2], shadowColor[3]);
		material["SpecularColor"] = Color(specularColor[0], specularColor[1], specularColor[2], specularColor[3]);
		material["RimLightColor"] = Color(rimLightColor[0], rimLightColor[1], rimLightColor[2], rimLightColor[3]);
		material["InnerEdgeColor"] = Color(innerEdgeColor[0], innerEdgeColor[1], innerEdgeColor[2], innerEdgeColor[3]);
		material["OuterEdgeColor"] = Color(outerEdgeColor[0], outerEdgeColor[1], outerEdgeColor[2], outerEdgeColor[3]);
		material["GlowColor"] = Color(glowColor[0], glowColor[1], glowColor[2], glowColor[3]);
		material["GlowStr"] = glowStr;
		material["InnerEdgeThickness"] = innerEdgeThickness;
		material["OuterEdgeThickness"] = outerEdgeThickness;
		material["SpecularPower"] = specularPower;
		material["SpecularStrength"] = specularPower;
		material["Roughness"] = roughness;
		material["ShadowOffset"] = shadowOffset;
		material["InnerFalloff"] = innerFalloff;
		material["OuterFalloff"] = outerFalloff;
		material["RimLightIntensity"] = rimLightIntensity;
		material["RimLightSize"] = rimLightSize;
	}

	ImGuiHandler::ImGuiHandler()
	{
		m_imguiImporter = new ImGuiImporter(&m_entityList, &m_itemList, &m_modelMap, &m_lightMap, &m_levelImporter, &m_materialList, &m_parentList);

		m_normalMapTextureFileName.push_back("None");
		m_normalMapTexture.push_back("None");
		m_textureFileName.push_back("None");
		m_textureName.push_back("None");
	}

	ImGuiHandler::~ImGuiHandler()
	{
		if (m_imguiImporter != nullptr)
		{
			delete m_imguiImporter;
		}
	}

	/*void ImGuiHandler::SaveThumbnail(string& destinationPath, string& filePathDestination,
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
	}*/

	void ImGuiHandler::StartUp()
	{
		ReadAllModelsFromHandler();
		ReadAllFilesFromFolder(IGH::FBXPATH);
		ReadAllFilesFromFolder(IGH::TEXTUREPATH);
		ReadAllFilesFromFolder(IGH::NORMALTEXTUREPATH);


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
		MaterialCreateWindow();
		ParticleCreateWindow();

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

				ImGui::Columns(1);
				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem(IGH::LIGHT.c_str()))
			{
				LightButtons();

				if(AddButton("Torch")) 
				{
					m_imguiImporter->AddTorchWithLight();
				}

				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Entities"))
			{
				AddEnemyButton();
				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem(IGH::MATERIAL.c_str()))
			{
				AddMaterialButton();
				ImGui::EndTabItem();
			} 
			
			if (ImGui::BeginTabItem("Particle"))
			{
				AddParticleButton();
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
		
		SetDefaultValue();

		ImGui::Begin("Properties", nullptr);

		if (ImGui::BeginTabBar("#Property Tab", ImGuiTabBarFlags_None))
		{
			ModelTab();

			MaterialTab();

			RoomTab();

			LightTab();

			ParticleTab();

			ImGui::EndTabBar();

		}
		ImGui::End();
	}

	void ImGuiHandler::ToolWindow()
	{
		static int mode = -1;
		static std::string currentMode = "";

		ImGui::Begin("Tools", nullptr);
		for (int i = 0; i < 3; i++)
		{
			if (ImGui::Button(m_imguiTypes.TOOLS[i]))
			{
				mode = i;

				if (mode == 0)
				{
					currentMode = m_imguiTypes.TOOLS[i];
				}
				else if (mode == 1)
				{
					currentMode = m_imguiTypes.TOOLS[i];
				}
				else if (mode == 2)
				{
					currentMode = m_imguiTypes.TOOLS[i];
				}
			}
		}

		if (AddButton("Delete"))
		{
			RemoveObject();
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

			if (ImGui::Button(IGH::CLOSE.c_str()))
			{
				m_saveWindowActive = false;
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
			ImGui::Combo("Type", &selectedType, m_imguiTypes.ENEMYTYPE, IM_ARRAYSIZE(m_imguiTypes.ENEMYTYPE));

			CustomComboString(m_objFileName, "Mesh", mesh,CurrentIndex);

			if (ImGui::Button("Create"))
			{
				if (mesh != "")
				{
					size_t id = m_imguiImporter->AddBase(name, mesh);
					m_modelMap.at(id).m_type = m_imguiTypes.ENEMYTYPE[selectedType];
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

			ImGui::DragFloat3("Translate", tempTranslation, 0.02f);
			ImGui::DragFloat3("Rotate", tempRotation, 0.5f);
			ImGui::DragFloat3("Scale", tempScale, 0.02f);

			if (ImGui::Button("Import"))
			{
				m_ImportWindowActive = m_imguiImporter->import(m_levelImporter,levelPath,tempTranslation,tempRotation,tempScale, m_roomProperty);
				m_imported = true;
			}
			ImGui::End();
		}
	}

	void ImGuiHandler::MaterialCreateWindow()
	{
		if (m_createMaterialActive == true)
		{
			static float baseColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
			static float shadowColor[4] = { 0.3f, 0.3f, 0.5f, 1.f };
			static float specularColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
			static float rimLightColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
			static float innerEdgeColor[4] = { 0, 0, 0, 1 };
			static float outerEdgeColor[4] = { 0, 0, 0, 1 };
			static float glowColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

			static float glowStr = 100;
			static float innerEdgeThickness = 0;
			static float outerEdgeThickness = 0;
			static float specularPower = 0.6f;
			static float specularStrength = 1.0f;
			static float roughness = 0.5f;
			static float shadowOffset = 0.0f;
			static float innerFalloff = 0.0f;
			static float outerFalloff = 0.0f;
			static float rimLightIntensity = 1.0f;
			static float rimLightSize = 0.0f;

			static char materialName[MATERIAL_NAME_MAX_LENGTH] = "";
			static char textureName[TEXTURE_NAME_MAX_LENGTH] = "";
			static char materialTextureName[MATERIAL_NAME_MAX_LENGTH] = "";
			static char normalTextureName[TEXTURE_NAME_MAX_LENGTH] = "";

			static int selectedTex = 0;
			static int selectedNormTex = 0;

			ImGui::Begin((IGH::CREATE + IGH::MATERIAL).c_str(), nullptr);

			ImGui::InputText((IGH::MATERIAL + " " + IGH::NAME).c_str(), materialName, MATERIAL_NAME_MAX_LENGTH);
			ImGui::InputText((IGH::MATERIAL + " " + IGH::TEXTURE + " " + IGH::NAME).c_str(), materialTextureName, MATERIAL_NAME_MAX_LENGTH);

			strcpy(textureName, CustomComboString(m_textureFileName, (IGH::TEXTURE + IGH::NAME), selectedTex).c_str());
			strcpy(normalTextureName, CustomComboString(m_normalMapTextureFileName, (IGH::NORMALTEXTURE + IGH::NAME), selectedNormTex).c_str());

			MaterialDragFloats(baseColor, shadowColor, specularColor, rimLightColor, 
				innerEdgeColor, outerEdgeColor, glowColor, glowStr, 
				innerEdgeThickness, outerEdgeThickness, specularPower, specularStrength, 
				roughness, shadowOffset, innerFalloff, outerFalloff, rimLightIntensity, rimLightSize);
			
			if (ImGui::Button(IGH::CREATE.c_str()))
			{
				if (strcmp(materialTextureName, "") != 0)
				{
					Aen::Material& material = Aen::Resource::CreateMaterial(materialName, true);

					if (strcmp(materialTextureName, "") != 0 && strcmp(textureName, "") != 0)
					{
						Aen::Texture& texture = Aen::Resource::CreateTexture(materialTextureName);
						Aen::Texture& normalTexture = Aen::Resource::CreateTexture(normalTextureName);

						texture.LoadTexture(AEN_RESOURCE_DIR(textureName));
						normalTexture.LoadTexture(AEN_NORMALTEXTURE_DIR(normalTextureName));

						material.SetDiffuseMap(texture);
						material.SetNormalMap(normalTexture);
					}

					SetMaterialValues(baseColor, shadowColor, specularColor, rimLightColor,
						innerEdgeColor, outerEdgeColor, glowColor, glowStr,
						innerEdgeThickness, outerEdgeThickness, specularPower, specularStrength,
						roughness, shadowOffset, innerFalloff, outerFalloff, rimLightIntensity, rimLightSize, material);


					m_materialList.push_back(IGH::MatTexName(materialName,materialTextureName,normalTextureName));
					std::memset(materialName, '\0', sizeof(char) * MATERIAL_NAME_MAX_LENGTH);
					std::memset(materialTextureName, '\0', sizeof(char) * MATERIAL_NAME_MAX_LENGTH);

					m_createMaterialActive = false;
				}
			}
			ImGui::End();
		}
	}

	void ImGuiHandler::ParticleCreateWindow()
	{
		if (m_createParticleWindowActive == true)
		{
			static int selectedType = 0;
			static int CurrentIndex = 0;
			static char name[MESH_NAME_MAX_LENGTH];
			static string mesh = "";

			ImGui::Begin("Create Particle", nullptr);

			ImGui::InputText("Name", name, MESH_NAME_MAX_LENGTH);
			ImGui::Combo("Type", &selectedType, m_imguiTypes.PARTICLETAG, IM_ARRAYSIZE(m_imguiTypes.PARTICLETAG));

			mesh = CustomComboString(m_objFileName, "Mesh", CurrentIndex);

			if (ImGui::Button("Create"))
			{
				if (mesh != "")
				{
					size_t id = m_imguiImporter->AddBase(name, mesh);
					m_modelMap.at(id).m_type = m_imguiTypes.PARTICLETAG[selectedType];
				}

				m_createParticleWindowActive = false;
				std::memset(name, '\0', sizeof(char) * MESH_NAME_MAX_LENGTH);
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
			}
		}
	}

	/*void ImGuiHandler::CreatePreviewTextureThumbnail()
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
	}*/

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

	void ImGuiHandler::SetMaterialValues(int selected)
	{
		Aen::Material& material = Aen::Resource::GetMaterial((m_materialList[selected].matName));

		m_imguiImporter->Convert(m_baseColor, material["BaseColor"]);
		m_imguiImporter->Convert(m_shadowColor, material["ShadowColor"]);
		m_imguiImporter->Convert(m_specularColor, material["SpecularColor"]);
		m_imguiImporter->Convert(m_rimLightColor, material["RimLightColor"]);
		m_imguiImporter->Convert(m_innerEdgeColor, material["InnerEdgeColor"]);
		m_imguiImporter->Convert(m_outerEdgeColor, material["OuterEdgeColor"]);
		m_imguiImporter->Convert(m_glowColor, material["GlowColor"]);

		material["GlowStr"] = m_glowStr;
		material["InnerEdgeThickness"] = m_innerEdgeThickness;
		material["OuterEdgeThickness"] = m_outerEdgeThickness;
		material["SpecularPower"] = m_specularPower;
		material["SpecularStrength"] = m_specularStrength;
		material["Roughness"] = m_roughness;
		material["ShadowOffset"] = m_shadowOffset;
		material["InnerFalloff"] = m_innerFalloff;
		material["OuterFalloff"] = m_outerFalloff;
		material["RimLightIntensity"] = m_rimLightIntensity;
		material["RimLightSize"] = m_rimLightSize;		
	}

	void ImGuiHandler::SetDefaultMaterialValue(int selected)
	{
		Aen::Material& material = Aen::Resource::GetMaterial((m_materialList[selected].matName));
		m_imguiImporter->Convert(material["BaseColor"], m_baseColor);
		m_imguiImporter->Convert(material["ShadowColor"], m_shadowColor);
		m_imguiImporter->Convert(material["SpecularColor"], m_specularColor);
		m_imguiImporter->Convert(material["RimLightColor"], m_rimLightColor);
		m_imguiImporter->Convert(material["InnerEdgeColor"], m_innerEdgeColor);
		m_imguiImporter->Convert(material["OuterEdgeColor"], m_outerEdgeColor);
		m_imguiImporter->Convert(material["GlowColor"], m_glowColor);

		m_glowStr = material["GlowStr"];
		m_innerEdgeThickness = material["InnerEdgeThickness"];
		m_outerEdgeThickness = material["OuterEdgeThickness"];
		m_specularPower = material["SpecularPower"];
		m_specularStrength = material["SpecularStrength"];
		m_roughness = material["Roughness"];
		m_shadowOffset = material["ShadowOffset"];
		m_innerFalloff = material["InnerFalloff"];
		m_outerFalloff = material["OuterFalloff"];
		m_rimLightIntensity = material["RimLightIntensity"];
		m_rimLightSize = material["RimLightSize"];
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

	void ImGuiHandler::Rotate(float angle)
	{
		float s = static_cast<float>(sin(angle * (C_PI / 180)));
		float c = static_cast<float>(cos(angle * (C_PI / 180)));
		float posX = 0;
		float posZ = 0;
		Vec3f translation;
		Vec3f rotation;
		size_t id;
		for (size_t i = 0; i < m_entityList.size(); i++)
		{
			id = m_entityList[i]->GetID();
			if (Aen::ComponentHandler::TranslationExist(id))
			{
				translation = m_entityList[i]->GetPos();

				if (Aen::ComponentHandler::RotationExist(id))
				{
					rotation = m_entityList[i]->GetRot();
					m_entityList[i]->SetRot(rotation.x, rotation.y + angle, rotation.z);
				}
				posX = (translation.x * c) - (translation.z * s);
				posZ = (translation.x * s) + (translation.z * c);

				m_entityList[i]->SetPos(posX, translation.y, posZ);

			}
		}
	}

	void ImGuiHandler::ChangeTexture(int& currentIndexTex, int& currentIndexNorm, string materialName, string materialTextureName, string materialNormalTextureName)
	{
		size_t id = m_entityList[m_selectedEntity]->GetID();

		if (m_modelMap.find(id) != m_modelMap.end())
		{
			if (m_textureFileName[currentIndexTex] == m_modelMap.at(id).m_texture.m_textureName && m_normalMapTextureFileName[currentIndexNorm] == m_modelMap.at(id).m_texture.m_normalTexture)
			{
				Aen::Material& mat = Aen::Resource::GetMaterial(materialName);
				Aen::ComponentHandler::GetMeshInstance(id).SetMaterial(mat);
			}
			else
			{
				string imageName = m_modelMap.at(id).m_texture.m_textureName;
				string textureName = materialTextureName;
				string normalImageName = m_modelMap.at(id).m_texture.m_normalTexture;

				if (m_textureFileName[currentIndexTex] != m_modelMap.at(id).m_texture.m_textureName)
				{
					imageName = m_textureFileName[currentIndexTex];
					textureName = materialTextureName;
				}

				if (m_normalMapTextureFileName[currentIndexNorm] != m_modelMap.at(id).m_texture.m_normalTexture)
				{
					normalImageName = m_normalMapTextureFileName[currentIndexNorm];
				}

				UpdateMap(id, textureName, materialName, m_itemList[m_selectedEntity], imageName, normalImageName);

				Aen::Texture& texture = Aen::Resource::CreateTexture(textureName);

				Aen::Texture& normalTexture = Aen::Resource::CreateTexture(materialNormalTextureName);
				Aen::Material& mat = Aen::Resource::GetMaterial(materialName);

				if (imageName.find(".png") != string::npos || imageName.find(".jpg") != string::npos)
				{
					texture.LoadTexture(AEN_TEXTURE_DIR(imageName));
					mat.SetDiffuseMap(texture);
				}

				if (normalImageName.find(".png") != string::npos || normalImageName.find(".jpg") != string::npos)
				{
					normalTexture.LoadTexture(AEN_NORMALTEXTURE_DIR(normalImageName));
					mat.SetNormalMap(normalTexture);

				}

				Aen::ComponentHandler::GetMeshInstance(id).SetMaterial(mat);
			}
		}
	}

	void ImGuiHandler::update()
	{
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

	void ImGuiHandler::LightTab()
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
	}

	void ImGuiHandler::RoomTab()
	{
		static int selectedTheme = 0;
		static int selectedType = 0;
		static int selectedSpecial = 0;
		static int probability = 0;
		static int roomSize = 0;
		static float rotateRoom = 0;

		

		if (ImGui::BeginTabItem("Room"))
		{
			if (m_imported == true)
			{
				selectedType = stoi(m_roomProperty[0]);
				selectedSpecial = stoi(m_roomProperty[1]);
				selectedTheme = stoi(m_roomProperty[2]);
				probability = stoi(m_roomProperty[3]);
				roomSize = stoi(m_roomProperty[4]);
				m_imported = false;

			}
			else
			{
				m_roomProperty[0] = m_imguiTypes.ROOMTYPE[selectedType];
				m_roomProperty[1] = m_imguiTypes.SPECIALROOM[selectedSpecial];
				m_roomProperty[2] = m_imguiTypes.ROOMTHEME[selectedTheme];
				m_roomProperty[3] = to_string(probability);
				m_roomProperty[4] = to_string(roomSize);
			}


			ImGui::Combo("Room Theme", &selectedTheme, m_imguiTypes.ROOMTHEME, IM_ARRAYSIZE(m_imguiTypes.ROOMTHEME));
			ImGui::Combo("Room Type", &selectedType, m_imguiTypes.ROOMTYPE, IM_ARRAYSIZE(m_imguiTypes.ROOMTYPE));
			ImGui::Combo("Special Room", &selectedSpecial, m_imguiTypes.SPECIALROOM, IM_ARRAYSIZE(m_imguiTypes.SPECIALROOM));
			ImGui::DragInt("Probability", &probability, 1, 0, 100, "%.d", 0);
			ImGui::DragInt("Room Size", &roomSize, 1);
			ImGui::DragFloat("Rotate Room", &rotateRoom, 1);

			if (AddButton("Rotate"))
			{
				Rotate(rotateRoom);
			}

			m_roomProperty[0] = m_imguiTypes.ROOMTYPE[selectedType];
			m_roomProperty[1] = m_imguiTypes.SPECIALROOM[selectedSpecial];
			m_roomProperty[2] = m_imguiTypes.ROOMTHEME[selectedTheme];
			m_roomProperty[3] = to_string(probability);
			m_roomProperty[4] = to_string(roomSize);

			ImGui::EndTabItem();
		}
	}

	void ImGuiHandler::MaterialTab()
	{
		static int selected = 0;
		static int selectedTexture = 0;
		static int selectedNormalTexture = 0;

		if (ImGui::BeginTabItem(IGH::MATERIAL.c_str()))
		{
			size_t id = m_entityList[m_selectedEntity]->GetID();

			if (Aen::ComponentHandler::MeshInstanceExist(id) && m_materialList.size() > 0)
			{
				if (ImGui::CollapsingHeader("Common Material Properties"))
				{

					CustomCombo(m_materialList, (IGH::MATERIAL + " List"), selected);
					SetDefaultMaterialValue(selected);

					MaterialDragFloats(m_baseColor, m_shadowColor, m_specularColor, m_rimLightColor,
						m_innerEdgeColor, m_outerEdgeColor, m_glowColor, m_glowStr,
						m_innerEdgeThickness, m_outerEdgeThickness, m_specularPower, m_specularStrength,
						m_roughness, m_shadowOffset, m_innerFalloff, m_outerFalloff, m_rimLightIntensity, m_rimLightSize);
				
					SetMaterialValues(selected);

					ExistInList(m_textureFileName, m_modelMap.at(id).m_texture.m_textureName, selectedTexture);
					CustomCombo(m_textureFileName, IGH::TEXTURE.c_str(), selectedTexture);

					ExistInList(m_normalMapTextureFileName, m_modelMap.at(id).m_texture.m_normalTexture, selectedNormalTexture);
					CustomCombo(m_normalMapTextureFileName, IGH::NORMALTEXTURE.c_str(), selectedNormalTexture);


					if (AddButton("Change Material"))
					{
						if (m_modelMap.find(id) != m_modelMap.end())
						{
							ChangeTexture(selectedTexture, selectedNormalTexture, m_materialList[selected].matName, m_materialList[selected].texName, m_materialList[selected].normTexName);
							m_modelMap.at(id).m_material.set(Aen::Resource::GetMaterial(m_materialList[selected].matName));
						}
					}

					if (AddButton("Set Material"))
					{
						if (m_modelMap.find(id) != m_modelMap.end())
						{
							m_modelMap.at(id).m_material.set(Aen::Resource::GetMaterial(m_materialList[selected].matName));
						}
					}
				}
			}
			ImGui::EndTabItem();

			static int selected = 0;
			static int selectedTexture = 0;
			static int selectedNormalTexture = 0;
		}
	}

	void ImGuiHandler::ModelTab()
	{
		static bool hitBoxEnable = false;
		static bool castShadow = false;

		static int selectedHitBoxType = 0;

		static int selectedParent = 0;

		if (ImGui::BeginTabItem(IGH::MODEL.c_str()))
		{
			size_t id = m_entityList[m_selectedEntity]->GetID();

			if (ImGui::CollapsingHeader("Transform Attributes"))
			{
				SetValues();
			}

			if (Aen::ComponentHandler::MeshInstanceExist(id))
			{
				ImGui::Checkbox("Hit box", &hitBoxEnable);
				ImGui::Combo("Hit box Type", &selectedHitBoxType, m_imguiTypes.HITBOXTYPE, IM_ARRAYSIZE(m_imguiTypes.HITBOXTYPE));
				ImGui::Checkbox("Cast Shadow", &castShadow);

				if (hitBoxEnable == true || castShadow == true)
				{
					unordered_map<size_t, IGH::ModelContainer>::iterator it = m_modelMap.find(id);

					if (it != m_modelMap.end())
					{
						it->second.m_model.rigidBody = hitBoxEnable;
						it->second.m_model.rigidBodyType = m_imguiTypes.HITBOXTYPE[selectedHitBoxType];
						it->second.m_model.m_castShadow = castShadow;

					}
				}
				CustomCombo(m_itemList, "Parent", selectedParent);
			}
			ImGui::EndTabItem();
		}
	}

	void ImGuiHandler::ParticleTab()
	{
		/*static int selectedType = 0;
		static string mesh = "";
		IGH::ImguiTypes types;

		if (ImGui::BeginTabItem(IGH::PARTICLE.c_str()))
		{
			size_t id = m_entityList[m_selectedEntity]->GetID();
			if (types.GetParticleTag(m_modelMap.at(id).m_type) == true)
			{
				selectedType = types.GetParticleTagIndex(m_modelMap.at(id).m_type);
			}

			ImGui::Combo("Type", &selectedType, m_imguiTypes.PARTICLETAG, IM_ARRAYSIZE(m_imguiTypes.PARTICLETAG));

			if (m_imguiTypes.PARTICLETAG[selectedType] != "NONE")
			{
				m_modelMap.at(id).m_type = m_imguiTypes.PARTICLETAG[selectedType];
			}
			else
			{
				m_modelMap.at(id).m_type = "";
			}
			ImGui::EndTabItem();
		}*/
	}

	void ImGuiHandler::UpdateMap(size_t key, string& texValue, string& matValue, string& meshName, string& texName, string &normalTexName)
	{
		unordered_map<size_t, IGH::ModelContainer>::iterator it;

		it = m_modelMap.find(key);

		if (it != m_modelMap.end())
		{
			it->second.update(texValue, matValue, texName, meshName, normalTexName);
		}
	}

	void ImGuiHandler::ReadAllFilesFromFolder(string folder)
	{
		string filePath = folder;
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
				if (filePath == IGH::NORMALTEXTUREPATH)
				{
					m_normalMapTextureFileName.push_back(fileName);
					m_normalMapTexture.push_back(fileName.substr(0, fileName.length() - 4));
				}
				else
				{
					m_textureFileName.push_back(fileName);
					m_textureName.push_back(fileName.substr(0, fileName.length() - 4));
				}
			}
		}
	}

	void ImGuiHandler::print(string input)
	{
		OutputDebugStringA((input + "\n").c_str());
	}

	void ImGuiHandler::ExistInList(vector<string>& list,string &target, int& index)
	{
		for (size_t i = 0; i < list.size(); i++)
		{
			if (list[i] == target)
			{
				index = i;
				break;
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

	void ImGuiHandler::AddMaterialButton()
	{
		if (AddButton(IGH::MATERIAL))
		{
			m_createMaterialActive = true;
		}
	}

	void ImGuiHandler::AddParticleButton()
	{
		if (AddButton(IGH::PARTICLE))
		{
			m_createParticleWindowActive = true;
		}
	}

	void ImGuiHandler::ModelButtons()
	{
		for (size_t i = 0; i < m_objFileName.size(); i++)
		{
			ImGui::Columns(4, nullptr);
			if (AddButton(m_objName[i]))
			{
				m_imguiImporter->AddBase(m_objName[i], m_objFileName[i]);
			}
			ImGui::NextColumn();
		}
	}

	void ImGuiHandler::LightButtons()
	{
		if (AddButton(IGH::POINTLIGHT.c_str()))
		{
			m_imguiImporter->AddPointLight();
		}

		if (AddButton(IGH::SPOTLIGHT.c_str()))
		{
			m_imguiImporter->AddSpotLight();
		}

		if (AddButton(IGH::DIRECTIONALLIGHT.c_str()))
		{
			m_imguiImporter->AddDirectional();
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

	bool ImGuiHandler::CustomCombo(vector<string>& list, string name, int& index)
	{
		bool selected = false;
		if (ImGui::BeginCombo(name.c_str(), list[index].c_str()))
		{
			for (size_t i = 0; i < list.size(); i++)
			{
				static bool isSelected = (index == i);

				if (ImGui::Selectable(list[i].data(), isSelected, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups))
				{
					index = static_cast<int>(i);
				}

				if (isSelected)
				{
					ImGui::SetItemDefaultFocus();
					selected = isSelected;
				}
			}
			ImGui::EndCombo();
		}
		return selected;
	}

	bool ImGuiHandler::CustomCombo(vector<IGH::MatTexName>& list, string name, int& index)
	{
		bool selected = false;
		if (ImGui::BeginCombo(name.c_str(), list[index].matName.c_str()))
		{
			for (size_t i = 0; i < list.size(); i++)
			{
				static bool isSelected = (index == i);

				if (ImGui::Selectable(list[i].matName.data(), isSelected, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups))
				{
					index = static_cast<int>(i);
				}

				if (isSelected)
				{
					ImGui::SetItemDefaultFocus();
					selected = isSelected;
				}
			}
			ImGui::EndCombo();
		}
		return selected;
	}

	string ImGuiHandler::CustomComboString(vector<string>& list, string name, int& index)
	{
		if (list.size() > 0)
		{
			//static string value = "";
			if (ImGui::BeginCombo(name.c_str(), list[index].c_str()))
			{
				static bool isSelected = false;

				for (size_t i = 0; i < list.size(); i++)
				{
					isSelected = (index == i);

					if (ImGui::Selectable(list[i].data(), isSelected, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups))
					{
						index = static_cast<int>(i);
					}

					if (isSelected)
					{
						ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndCombo();
				return list[index];

			}
		}
		return "";
	}

	void ImGuiHandler::CustomComboString(vector<string>& list, string name, string& input, int& index)
	{
		if (list.size() > 0)
		{
			//static string value = "";
			if (ImGui::BeginCombo(name.c_str(), list[index].c_str()))
			{
				static bool isSelected = false;

				for (size_t i = 0; i < list.size(); i++)
				{
					isSelected = (index == i);

					if (ImGui::Selectable(list[i].data(), isSelected, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups))
					{
						index = static_cast<int>(i);
						input = list[index];
					}

					if (isSelected)
					{
						ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndCombo();
			}
		}
	}

	string ImGuiHandler::CustomComboMap(unordered_map<string, AenIF::Material>& list, string name, string index)
	{
		static string value = "";

		if (list.size() > 0)
		{
			
			if (ImGui::BeginCombo(name.c_str(), value.c_str()))
			{
				static bool isSelected = false;
				for (unordered_map<string, AenIF::Material>::iterator it = list.begin(); it != list.end(); it++)
				{
					isSelected = (index == it->first);

					if (ImGui::Selectable(it->first.data(), isSelected, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups))
					{
						value = it->first;
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
		return "";
	}

	void ImGuiHandler::RemoveObject()
	{
		size_t id = m_entityList[m_selectedEntity]->GetID();
		if (!Aen::ComponentHandler::CameraExist(id))
		{
			if (m_parentList.find(id) != m_parentList.end())
			{
				mp_entityHandlerPtr->GetEntity(m_parentList.at(id)).RemoveParent();
			}

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