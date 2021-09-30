#pragma once
#include "Graphics/DirectX11/DX11Core.h"
#include "Graphics/Component/Resource.h"
#include "Graphics/Component/ComponentHandler.h"
#include "Graphics/Component/Entity.h"
#include <filesystem>
#include "CreateThumbnail.h"
#include "LevelExporter.h"

#include "ThirdParty/ImGui/imgui.h"
#include "ThirdParty/ImGui/imfilebrowser.h"
#include "ThirdParty/ImGui/imgui_stdlib.h"
#include "ThirdParty/ImGui/imgui_impl_win32.h"
#include "ThirdParty/ImGui/imgui_impl_dx11.h"

#include <vector>

using std::string;
using std::vector;
using std::cout;
using std::endl;

namespace Aen {

	class AEN_DECLSPEC ImGuiHandler {

	private:
		void SaveThumbnail(string& destinationPath, string& filePathDestination,
			string& sourcePath, string& filePathSource,
			Aen::ImageByteData& source, Aen::ImageByteData& destination, int& i);

	private:

		Aen::EntityHandler* mp_entityHandlerPtr;
		vector<Aen::Entity*> m_entityList;

		// Display in scene list
		vector<string> m_itemList;
		//vector<bool> m_hitBoxList;

		vector<int> m_deleteList;

		vector<string> m_objFileName;
		vector<string> m_objName;

		vector<string> m_textureFileName;
		vector<string> m_textureName;

		// Type
		vector<string> m_entityType;


		int m_selectedEntity = 0;
		float m_xyzTranslation[3] = { 0,0,0 };
		float m_xyzRotation[3] = { 0,0,0 };
		float m_xyzScale[3] = { 1,1,1 };
		int m_entityCount = 0;

		ImGui::FileBrowser fileDialog;
		string openOrSave;

		LevelExporter m_levelExporter;

		vector<string> m_meshObjList;

		

	public:
		ImGuiHandler();
		~ImGuiHandler();

		void StartUp();

		void Initialize(const HWND& hwnd, ID3D11Device* mp_device, ID3D11DeviceContext* mp_dContext);
		void NewFrame();
		void Render();
		void Release();
		void SceneListWindow();
		void AssetWindow();
		void ColorWheel();
		void PropertyWindow();
		void ToolWindow();
		void MaterialWindow();
		void Update();

		void AddModel(Aen::Entity* entity);
		void ReadAllModelsFromHandler();
		void CreatePreviewTextureThumbnail();

		void SetValues();
		void SetDefaultValue();
		void ZeroValue();
		void SetMaterialValues();

		void AddBase(const string &meshName, const string &objName);
		void RemoveObject();

		void ReadAllFilesFromResourceFolder();
		bool AddButton(const string& name);
		void HandleButton();
		const string CheckType(Aen::Entity* entity);

		void CustomCombo(int& index, string name);

	};
}



