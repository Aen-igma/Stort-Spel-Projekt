#pragma once
#include "Graphics/DirectX11/DX11Core.h"
#include "Graphics/Component/Resource.h"
#include <filesystem>
#include "CreateThumbnail.h"
#include "LevelExporter.h"
#include "LevelImporter.h"
#include <algorithm>
#include "LevelEditor/ImguiType.h"
#include "LevelEditor/ImGuiImporter.h"

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
using std::to_string;
using std::unordered_map;

namespace Aen {

	class AEN_DECLSPEC ImGuiHandler {

	private:
		void SaveThumbnail(string& destinationPath, string& filePathDestination,
			string& sourcePath, string& filePathSource,
			Aen::ImageByteData& source, Aen::ImageByteData& destination, int& i);

	private:

		string levelPath = "";
		string m_roomProperty[5] = {"","","","1","1"};
		
		Aen::EntityHandler* mp_entityHandlerPtr;
		vector<Aen::Entity*> m_entityList;

		Aen::ImGuiImporter* imguiImporter;

		// Display in scene list
		vector<string> m_itemList;
		//vector<bool> m_hitBoxList;

		vector<string> m_objFileName;
		vector<string> m_objName;

		vector<string> m_textureFileName;
		vector<string> m_textureName;

		unordered_map< size_t, IGH::ModelContainer> m_modelMap;
		unordered_map< size_t, Aen::Entity*> m_lightMap;

		bool m_multiSelectActive = false;
		int m_selectedEntity = 0;
		//vector<bool> m_selected;
		//vector<int> m_selectedEntities;


		float m_xyzTranslation[3] = { 0,0,0 };
		float m_xyzRotation[3] = { 0,0,0 };
		float m_xyzScale[3] = { 1,1,1 };
		
		ImGui::FileBrowser m_fileDialog;

		// Level exporter info
		LevelExporter m_levelExporter;

		// Level importer info
		AenIMP::LevelImporter m_levelImporter;

		bool m_saveWindowActive = false;
		bool m_ImportWindowActive = false;
		bool m_createEnemyWindowActive = false;

		// Counters
		unsigned int m_enemyCount = 0;
		int m_entityCount = 0;
		int m_lightCount = 0;
		int m_OriginalCount = 0;

	public:
		// All Add func here


		bool AddButton(const string& name);
		void AddEnemyButton();

	public:
		// All Set and Get here
		AenIF::Room GetRoom(size_t index);

		void SetValues();
		void SetDefaultValue();
		void ZeroValue();

	public: 
		// All window func here
		void SceneListWindow();
		void AssetWindow();
		void ColorWheel();
		void PropertyWindow();
		void ToolWindow();
		void SaveWindow();
		void EnemyCreateWindow();
		void ImportWindow();

	public:
		ImGuiHandler();
		~ImGuiHandler();

		void StartUp();
		void RenderAllWindows();

		void Initialize(const HWND& hwnd, ID3D11Device* mp_device, ID3D11DeviceContext* mp_dContext);
		void NewFrame();
		void Render();
		void Release();

	private:


		void ReadAllModelsFromHandler();
		void CreatePreviewTextureThumbnail();
		void RemoveObject();
		void ReadAllFilesFromResourceFolder();
		void UpdateMap(size_t key, string& texValue, string& matValue, string& meshName, string& texName);
	
		void ModelButtons();
		void LightButtons();
		const string CheckType(Aen::Entity* entity);

		void CustomCombo(vector<string>& list, string name, string type, int &index);

		string CustomCombo(vector<string>& list, string name, int& index);

		void ChangeMaterial(int& currentIndex);

		void update();

		void DuplicateWindow(bool& active);

		void lightTab();
		void RoomTab();



};
}



