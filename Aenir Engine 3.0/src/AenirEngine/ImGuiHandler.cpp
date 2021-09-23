#include"PCH.h"
#include "ImGuiHandler.h"

namespace Aen {

	ImGuiHandler::ImGuiHandler()
	{
	}

	ImGuiHandler::~ImGuiHandler()
	{

		if (deleteList.size() > 0)
		{
			for (size_t i = 0; i < deleteList.size(); i++)
			{
				delete entityList[deleteList[i] - 1];
			}
		}
	}

	void ImGuiHandler::initialize(HWND& hwnd, ID3D11Device* m_device, ID3D11DeviceContext* m_dContext)
	{
		// SetUp ImGui
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		ImGui_ImplWin32_Init(hwnd);
		ImGui_ImplDX11_Init(m_device, m_dContext);
		ImGui::StyleColorsDark();
	}

	void ImGuiHandler::newFrame()
	{
		// Start the ImGui frame
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
	}

	void ImGuiHandler::render()
	{
		// Draw
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}

	void ImGuiHandler::release()
	{
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
	}
	
	void ImGuiHandler::sceneListWindow()
	{
		ImGui::Begin("Scene", nullptr, ImGuiWindowFlags_MenuBar);

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Open..", "Ctrl + O"))
				{

				}

				if (ImGui::MenuItem("Save..", "Ctrl + S"))
				{

				}

				if (ImGui::MenuItem("Close..", "Ctrl + W"))
				{
					//windowClose = false;
				}

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}



		ImGui::BeginChild("List");

		if (ImGui::BeginListBox("Test"))
		{
			for (size_t i = 0; i < itemList.size(); i++)
			{
				const bool is_selected = (selectedEntity == i);
				if (ImGui::Selectable(itemList[i].c_str(), is_selected, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups))
				{
					selectedEntity = i;
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

	void ImGuiHandler::assetWindow()
	{
		ImGui::Begin("Assets", nullptr);
		ImGui::Separator();
		if (ImGui::BeginTabBar("#AssetTab", ImGuiTabBarFlags_None))
		{
			if (ImGui::BeginTabItem("Model"))
			{
				handleButton();

				if (addButton("Delete"))
				{
					removeObject();
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

	void ImGuiHandler::colorWheel()
	{
		ImGui::Begin("Color", nullptr);
		static float color[3] = { 255,255,255 };
		ImGui::ColorPicker3("##picker", color, ImGuiColorEditFlags_PickerHueWheel);
		ImGui::End();
	}

	void ImGuiHandler::propertyWindow()
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

		setDefaultValue();

		ImGui::Begin("Mesh Properties", nullptr);

		if (ImGui::BeginTabBar("#Property Tab", ImGuiTabBarFlags_None))
		{
			if (ImGui::BeginTabItem("Mesh"))
			{
				if (ImGui::CollapsingHeader("Transform Attributes"))
				{
					ImGui::InputFloat3("Translate", xyzTranslation);
					ImGui::InputFloat3("Rotate", xyzRotation);
					ImGui::InputFloat3("Scale", xyzScale);

					setValues();
				}

				ImGui::Checkbox("Hit box", &hitBoxEnable);

				static const char* items[]{ "None", "Test", "Test1", "Test2" };
				static int selectedItem = 0;
				ImGui::Combo("Parent", &selectedItem, items, IM_ARRAYSIZE(items));

				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Material"))
			{
				if (ImGui::CollapsingHeader("Common Material Properties"))
				{
					ImGui::SliderFloat("Color", &albedoColor, 0.0f, 1.0f);
					ImGui::SliderFloat("Transparency", &transparency, 0.0f, 1.0f);
					ImGui::SliderFloat("Ambient Color", &ambientColor, 0.0f, 1.0f);
					ImGui::SliderFloat("Diffuse", &diffuse, 0.0f, 1.0f);
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

	void ImGuiHandler::toolWindow()
	{
		static int mode = -1;
		static const std::string stringArray[3] = { "Move","Rotate","Scale" };
		static std::string currentMode = "";

		ImGui::Begin("a", nullptr);
		for (int i = 0; i < 3; i++)
		{
			if (ImGui::Button(stringArray[i].c_str()));
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

	void ImGuiHandler::materialWindow()
	{
		/*static string testString = "a";
		ImGui::InputText("test", &testString);*/
	}

	void ImGuiHandler::update()
	{


	}

	void ImGuiHandler::addModel(Aen::Entity* entity)
	{
		entityList.push_back(entity);
		itemList.push_back("Asset" + std::to_string(entityCount));
		entityCount++;
	}

	void ImGuiHandler::readAllModelsFromHandler()
	{
		if (entityList.size() < entityHandlerPtr->m_entities.size())
		{
			for (std::pair<uint32_t, Aen::Entity*> element : entityHandlerPtr->m_entities)
			{
				entityList.push_back(element.second);
				itemList.push_back(checkType(element.second));
			}
		}
	}

	void ImGuiHandler::setDefaultValue()
	{

		if (entityList.size() > 0 && selectedEntity < entityList.size())
		{
			uint32_t id = entityList[selectedEntity]->getID();

			if (Aen::ComponentHandler::TranslationExist(id))
			{
				xyzTranslation[0] = entityList[selectedEntity]->GetPos().x;
				xyzTranslation[1] = entityList[selectedEntity]->GetPos().y;
				xyzTranslation[2] = entityList[selectedEntity]->GetPos().z;
			}
			else
			{
				for (int i = 0; i < 3; i++)
				{
					xyzTranslation[i] = 0;
				}
			}

			if (Aen::ComponentHandler::RotationExist(id))
			{
				xyzRotation[0] = entityList[selectedEntity]->GetRot().x;
				xyzRotation[1] = entityList[selectedEntity]->GetRot().y;
				xyzRotation[2] = entityList[selectedEntity]->GetRot().z;
			}
			else
			{
				for (int i = 0; i < 3; i++)
				{
					xyzRotation[i] = 0;
				}
			}

			if (Aen::ComponentHandler::ScaleExist(id))
			{
				xyzScale[0] = entityList[selectedEntity]->GetScale().x;
				xyzScale[1] = entityList[selectedEntity]->GetScale().y;
				xyzScale[2] = entityList[selectedEntity]->GetScale().z;
			}
			else
			{

				for (int i = 0; i < 3; i++)
				{
					if ((Aen::ComponentHandler::DirectionalLightExist(id) || Aen::ComponentHandler::SpotLightExist(id) || Aen::ComponentHandler::PointLightExist(id)))
					{
						xyzScale[i] = 0;
					}
					else
					{
						xyzScale[i] = 1;
					}
				}
			}
		}
		else
		{
			zeroValue();

		}
	}

	void ImGuiHandler::setValues()
	{
		if (entityList.size() > 0 && selectedEntity < entityList.size())
		{
			uint32_t id = entityList[selectedEntity]->getID();

			if (Aen::ComponentHandler::TranslationExist(id)) 
			{
				entityList[selectedEntity]->SetPos(xyzTranslation[0], xyzTranslation[1], xyzTranslation[2]);
			}
						
			if (Aen::ComponentHandler::RotationExist(id)) 
			{
				entityList[selectedEntity]->SetRot(xyzRotation[0], xyzRotation[1], xyzRotation[2]);
			}
					
			if (Aen::ComponentHandler::ScaleExist(id)) 
			{
				entityList[selectedEntity]->SetScale(xyzScale[0], xyzScale[1], xyzScale[2]);
			}

		}
		else
		{
			zeroValue();
		}
	}

	void ImGuiHandler::zeroValue()
	{
		for (int i = 0; i < 3; i++)
		{
			xyzTranslation[i] = 0;
			xyzRotation[i] = 0;
			xyzScale[i] = 1;
		}
	}

	void ImGuiHandler::addCube()
	{
		addBase("Cube", "Cube.obj");
	}

	void ImGuiHandler::addPlane()
	{
		addBase("Plane", "Plane.obj");
	}

	void ImGuiHandler::addBase(string meshName, string objName)
	{
		Aen::Entity* entity = AEN_NEW(Aen::Entity);
		Aen::Mesh& mesh = Aen::Resource::CreateMesh(meshName + std::to_string(entityCount));
		mesh.Load(AEN_RESOURCE_DIR(objName));

		entity->AddComponent<Aen::MeshInstance>();
		entity->GetComponent<Aen::MeshInstance>().SetMesh(mesh);

		addModel(entity);
		deleteList.push_back(entityList.size());
	}
	
	string ImGuiHandler::checkType(Aen::Entity* entity)
	{
		string type = "";
		uint32_t id = entity->getID();

		if (Aen::ComponentHandler::CameraExist(id))
		{
			type = "Camera";
		}
		else if ((Aen::ComponentHandler::DirectionalLightExist(id) || Aen::ComponentHandler::SpotLightExist(id) || Aen::ComponentHandler::PointLightExist(id)))
		{
			type = "light";
		}
		else
		{
			type = "Asset" + std::to_string(entityCount);
			
		}
		entityCount++;

		return type;
	}

	void ImGuiHandler::readAllFilesFromResourceFolder()
	{
		string filePath = "../Resource/";
		string fileName = "";
		string fileType = "";
		//int index = 0;

		for (const auto & entry : std::filesystem::directory_iterator(filePath))
		{
			fileName = entry.path().filename().string();
			fileType = fileName.substr(fileName.find_last_of(".") + 1);
			
			if (fileType == "obj")
			{
				objFileName.push_back(fileName);
				cout << fileName << endl;
			}
			else if (fileType == "png" || fileType == "jpg")
			{
				textureFileName.push_back(fileType);
			}
		}
	}

	bool ImGuiHandler::addButton(string name)
	{
			return ImGui::Button(name.c_str());
	}

	void ImGuiHandler::handleButton()
	{
		for (size_t i = 0; i < objFileName.size(); i++)
		{
			if (addButton(objFileName[i].substr(0, objFileName[i].length() - 4)))
			{
				addBase(objFileName[i].substr(0, objFileName[i].length() - 4), objFileName[i]);
			}
		}

	}

	void ImGuiHandler::removeObject()
	{

		for (int i = 0; i < deleteList.size(); i++)
		{
			if (selectedEntity == deleteList[i] - 1)
			{
				/*delete entityList[selectedEntity];
				entityList.erase(entityList.begin() + selectedEntity);
				itemList.erase(itemList.begin() + selectedEntity);
				deleteList.erase(deleteList.begin() + i);
				entityCount--;*/
			}
		}
	}
}

