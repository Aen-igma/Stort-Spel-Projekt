#include"PCH.h"
#include "ImGuiHandler.h"

namespace Aen {

	
	ImGuiHandler::ImGuiHandler()
	{
	}

	ImGuiHandler::~ImGuiHandler()
	{

		if (m_deleteList.size() > 0)
		{
			for (size_t i = 0; i < m_deleteList.size(); i++)
			{
				delete m_entityList[m_deleteList[i] - static_cast<size_t>(1)];
			}
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
		CreatePreviewTextureThumbnail();
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

		fileDialog.SetTitle("Open");
		fileDialog.SetTypeFilters({ ".h",".cpp" });

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

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Open..", "Ctrl + O"))
				{
					openOrSave = "Open";
					fileDialog.SetTitle("Open");
					fileDialog.Open();
				}

				if (ImGui::MenuItem("Save..", "Ctrl + S"))
				{
					openOrSave = "Save";
					fileDialog.SetTitle("Save as");
					fileDialog.Open();
				}

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		fileDialog.Display();

		if (fileDialog.HasSelected())
		{
			if (openOrSave == "Open")
			{
				std::cout << "Selected filename " << fileDialog.GetSelected().string() << std::endl;

			}
			else if(openOrSave == "Save")
			{
				std::cout << "Selected fileDir " << fileDialog.GetPwd().string() << std::endl;
				m_levelExporter.writeInto(m_entityList, m_itemList, m_meshObjList, m_textureFileName, m_entityType);

			}

			fileDialog.ClearSelected();
		}

		ImGui::BeginChild("List");

		if (ImGui::BeginListBox("Test"))
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
		ImGui::Separator();
		if (ImGui::BeginTabBar("#AssetTab", ImGuiTabBarFlags_None))
		{
			if (ImGui::BeginTabItem("Model"))
			{
				HandleButton();

				if (AddButton("Delete"))
				{
					RemoveObject();
				}

				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Light"))
			{
				ImGui::Text("Light");
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

		static std::string mapName = "";

		//string imageName = AEN_RESOURCE_DIR("Place_Holder.png");

		//Aen::Texture& texture = Aen::Resource::CreateTexture("ImGuiTexture");
		//texture.LoadTexture(imageName)*/;

		//// texture friend
		////static ImVec2 imageSize = ImVec2(50, 50);
		////ImGui::Image("Display", imageSize,)

		SetDefaultValue();

		ImGui::Begin("Mesh Properties", nullptr);

		if (ImGui::BeginTabBar("#Property Tab", ImGuiTabBarFlags_None))
		{
			if (ImGui::BeginTabItem("Mesh"))
			{
				if (ImGui::CollapsingHeader("Transform Attributes"))
				{
					SetValues();
				}

				ImGui::Checkbox("Hit box", &hitBoxEnable);

				static int selectedItem = 0;
				CustomCombo(selectedItem,"Parent");

				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Material"))
			{
				if (ImGui::CollapsingHeader("Common Material Properties"))
				{
					if (m_entityList.size() > 0 && m_selectedEntity < m_entityList.size())
					{
						uint32_t id = m_entityList[m_selectedEntity]->getID();

						if (Aen::ComponentHandler::MeshInstanceExist(id))
						{
							static int currentIndex = 0;
							if (ImGui::BeginCombo("Test", m_textureName[currentIndex].c_str()))
							{
								for (size_t i = 0; i < m_textureName.size(); i++)
								{
									static bool isSelected = (currentIndex == i);

									if (ImGui::Selectable(m_textureName[i].data(), isSelected, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups))
									{
										currentIndex = static_cast<int>(i);
										string imageName = AEN_RESOURCE_DIR(m_textureFileName[currentIndex]);
										Aen::Texture& texture = Aen::Resource::CreateTexture("Texture");
										texture.LoadTexture(imageName);
										Aen::Material& mat = Aen::Resource::CreateMaterial("Material", true);
										mat.SetDiffuseMap(texture);

										size_t id = m_entityList[m_selectedEntity]->getID();
										Aen::ComponentHandler::GetMeshInstance(static_cast<uint32_t>(id)).SetMaterial(mat);
									}

									if (isSelected)
									{
										ImGui::SetItemDefaultFocus();
									}
								}
								ImGui::EndCombo();
							}
							ImGui::SliderFloat("Transparency", &transparency, 0.0f, 1.0f);
							ImGui::SliderFloat("Ambient Color", &ambientColor, 0.0f, 1.0f);
							ImGui::SliderFloat("Diffuse", &diffuse, 0.0f, 1.0f);
							
						}
					}
				}

				if (ImGui::CollapsingHeader("Normal Mapping"))
				{
					ImGui::InputText("Map", const_cast<char*>(mapName.c_str()), IM_ARRAYSIZE(mapName.c_str()));
				}

				if (ImGui::CollapsingHeader("Specular Shading"))
				{
					ImGui::SliderFloat("Specular Color", &specularColor, 0.0f, 1.0f);
					ImGui::SliderFloat("Reflectivity", &reflectivity, 0.0f, 1.0f);
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

	void ImGuiHandler::MaterialWindow()
	{
	}

	void ImGuiHandler::Update()
	{


	}

	void ImGuiHandler::AddModel(Aen::Entity* entity)
	{
		m_entityList.push_back(entity);
		m_itemList.push_back("Model" + std::to_string(m_entityCount));
		m_entityType.push_back("Model");
		m_entityCount++;
	}

	void ImGuiHandler::ReadAllModelsFromHandler()
	{
		if (m_entityList.size() < mp_entityHandlerPtr->m_entities.size())
		{
			for (std::pair<uint32_t, Aen::Entity*> element : mp_entityHandlerPtr->m_entities)
			{
				m_entityList.push_back(element.second);
				m_itemList.push_back(CheckType(element.second));
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
			cout << "is empty" << endl;

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
			uint32_t id = m_entityList[m_selectedEntity]->getID();

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
			uint32_t id = m_entityList[m_selectedEntity]->getID();

			if (Aen::ComponentHandler::TranslationExist(id)) 
			{
				ImGui::InputFloat3("Translate", m_xyzTranslation);
				m_entityList[m_selectedEntity]->SetPos(m_xyzTranslation[0], m_xyzTranslation[1], m_xyzTranslation[2]);
			}
						
			if (Aen::ComponentHandler::RotationExist(id)) 
			{
				ImGui::InputFloat3("Rotate", m_xyzRotation);
				m_entityList[m_selectedEntity]->SetRot(m_xyzRotation[0], m_xyzRotation[1], m_xyzRotation[2]);
			}
					
			if (Aen::ComponentHandler::ScaleExist(id)) 
			{
				ImGui::InputFloat3("Scale", m_xyzScale);
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

	void ImGuiHandler::SetMaterialValues()
	{
		if (m_entityList.size() > 0 && m_selectedEntity < m_entityList.size())
		{
			uint32_t id = m_entityList[m_selectedEntity]->getID();

			m_entityList[m_selectedEntity]->SetPos(m_xyzTranslation[0], m_xyzTranslation[1], m_xyzTranslation[2]);
						
			m_entityList[m_selectedEntity]->SetRot(m_xyzRotation[0], m_xyzRotation[1], m_xyzRotation[2]);
			

			if (Aen::ComponentHandler::ScaleExist(id))
			{
				m_entityList[m_selectedEntity]->SetScale(m_xyzScale[0], m_xyzScale[1], m_xyzScale[2]);
			}

		}
		else
		{
			ZeroValue();
		}
	}

	void ImGuiHandler::AddBase(const string& meshName, const string& objName)
	{
		Aen::Entity* entity = AEN_NEW(Aen::Entity);
		Aen::Mesh& mesh = Aen::Resource::CreateMesh(meshName + std::to_string(m_entityCount));
		mesh.Load(AEN_RESOURCE_DIR(objName));

		entity->AddComponent<Aen::MeshInstance>();
		entity->GetComponent<Aen::MeshInstance>().SetMesh(mesh);

		m_meshObjList.push_back(objName);

		AddModel(entity);
		m_deleteList.push_back(static_cast<int>(m_entityList.size()));
	}

	const string ImGuiHandler::CheckType(Aen::Entity* entity)
	{
		string type = "";
		uint32_t id = entity->getID();

		if (Aen::ComponentHandler::MeshInstanceExist(id))
		{
			type = "Model" + std::to_string(m_entityCount);
			m_entityType.push_back("Model");
			m_meshObjList.push_back("Cube.obj");
			m_entityCount++;
		}
		else if (Aen::ComponentHandler::CameraExist(id))
		{
			type = "Camera";
			m_entityType.push_back(type);			
		}
		else if (Aen::ComponentHandler::DirectionalLightExist(id))
		{
			type = "Directional light";
			m_entityType.push_back(type);
		}
		else if (Aen::ComponentHandler::SpotLightExist(id))
		{
			type = "Spot light";
			m_entityType.push_back(type);
		}
		else if (Aen::ComponentHandler::PointLightExist(id))
		{
			type = "Point light";
			m_entityType.push_back(type);
		}
		else
		{
			type = "Asset" + std::to_string(m_entityCount);
			m_entityType.push_back("Asset");
			m_entityCount++;
		}
		return type;
	}

	void ImGuiHandler::CustomCombo(int& index, string name)
	{
		if (ImGui::BeginCombo(name.c_str(), m_itemList[index].c_str()))
		{
			for (size_t i = 0; i < m_itemList.size(); i++)
			{
				static bool isSelected = (index == i);

				if (ImGui::Selectable(m_itemList[i].data(), isSelected, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups))
				{
					index = static_cast<int>(i);
				}

				if (isSelected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}
	}


	
	void ImGuiHandler::ReadAllFilesFromResourceFolder()
	{
		string filePath = "../Resource/";
		string fileName = "";
		string fileType = "";

		for (const auto & entry : std::filesystem::directory_iterator(filePath))
		{
			fileName = entry.path().filename().string();
			fileType = fileName.substr(fileName.find_last_of(".") + 1);
			
			if (fileType == "obj")
			{
				m_objFileName.push_back(fileName);
				m_objName.push_back(fileName.substr(0, fileName.length() - 4));
				cout << fileName.substr(0, fileName.length() - 4) << endl;
			}
			else if (fileType == "png" || fileType == "jpg")
			{
				m_textureFileName.push_back(fileName);
				m_textureName.push_back(fileName.substr(0, fileName.length() - 4));
				cout << fileName.substr(0, fileName.length() - 4) << endl;
			}
		}
	}

	bool ImGuiHandler::AddButton(const string &name)
	{
			return ImGui::Button(name.c_str());
	}

	void ImGuiHandler::HandleButton()
	{
		for (size_t i = 0; i < m_objFileName.size(); i++)
		{
			if (AddButton(m_objName[i]))
			{
				AddBase(m_objName[i], m_objFileName[i]);
			}
		}

	}



	void ImGuiHandler::RemoveObject()
	{

		for (int i = 0; i < m_deleteList.size(); i++)
		{
			if (m_selectedEntity == m_deleteList[i] - 1)
			{
				delete m_entityList[m_selectedEntity];
				m_entityList.erase(m_entityList.begin() + m_selectedEntity);
				m_itemList.erase(m_itemList.begin() + m_selectedEntity);
				m_deleteList.erase(m_deleteList.begin() + i);
				m_entityCount--;
			}
		}
	}
}

