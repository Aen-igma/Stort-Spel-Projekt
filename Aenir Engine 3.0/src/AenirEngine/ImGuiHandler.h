#pragma once
#include "Graphics/DirectX11/DX11Core.h"
#include "Graphics/Component/Resource.h"
#include <filesystem>
#include "CreateThumbnail.h"
#include "LevelExporter.h"
#include "LevelImporter.h"

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

	struct Enemy 
	{
		string m_mesh;
		float m_translation[3];
		float m_scale[3];
		float m_rotation[3];
		string m_type;
	};


	class AEN_DECLSPEC ImGuiHandler {

	private:
		void SaveThumbnail(string& destinationPath, string& filePathDestination,
			string& sourcePath, string& filePathSource,
			Aen::ImageByteData& source, Aen::ImageByteData& destination, int& i);

	private:

		string m_roomProperty[4] = {"","","",""};

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

		unordered_map< uint32_t, ModelContainer> m_modelMap;


		int m_selectedEntity = 0;
		float m_xyzTranslation[3] = { 0,0,0 };
		float m_xyzRotation[3] = { 0,0,0 };
		float m_xyzScale[3] = { 1,1,1 };
		


		ImGui::FileBrowser m_fileDialog;

		// Level exporter info
		LevelExporter m_levelExporter;

		// Level importer info
		AenIMP::LevelImporter m_levelImporter;

		bool m_saveWindowActive = false;
		bool m_createEnemyWindowActive = false;

		// Counters
		unsigned int m_enemyCount = 0;
		int m_entityCount = 0;
		int m_lightCount = 0;

	public:
		// All Add func here

		void AddLight(Aen::Entity* entity);
		void AddLight(Aen::Entity* entity, string type);

		void AddBase(AenIF::Model& model, AenIF::Texture &texture);
		void AddBase(const string& meshName, const string& objName);

		void AddPointLight(AenIF::Light& input);
		void AddPointLight();

		void AddSpotLight(AenIF::Light& input);
		void AddSpotLight();

		void AddDirectional(AenIF::Light& input);
		void AddDirectional();

		void AddEnemy();
		void AddModel(Aen::Entity* entity, string name);

		void AddModel(Aen::Entity* entity);
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

	public:
		ImGuiHandler();
		~ImGuiHandler();

		void StartUp();
		void RenderAllWindows();

		void Initialize(const HWND& hwnd, ID3D11Device* mp_device, ID3D11DeviceContext* mp_dContext);
		void NewFrame();
		void Render();
		void Release();


		void ReadAllModelsFromHandler();
		void CreatePreviewTextureThumbnail();

		void RemoveObject();

		void ReadAllFilesFromResourceFolder();


		void UpdateMap(uint32_t key, string& texValue, string& matValue, string& meshName, string& texName);
	
		private:
			void ModelButtons();
			void LightButtons();
			const string CheckType(Aen::Entity* entity);

			void CustomCombo(vector<string>& list, string name, string type);

			string CustomCombo(vector<string>& list, string name);

			void ChangeMaterial(int& currentIndex);

};
}



