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

		unordered_map<unsigned int, MatTexContainer> m_textureModelsMap;


		int m_selectedEntity = 0;
		float m_xyzTranslation[3] = { 0,0,0 };
		float m_xyzRotation[3] = { 0,0,0 };
		float m_xyzScale[3] = { 1,1,1 };
		
		int m_entityCount = 0;
		int m_lightCount = 0;
		int m_modelNumber = 0;

		ImGui::FileBrowser m_fileDialog;

		// Level exporter info
		LevelExporter m_levelExporter;
		vector<string> m_meshObjList;
		vector<string> m_textureFileList;

		vector<string> m_MaterialNameList;

		// Level importer info
		AenIMP::LevelImporter m_levelImporter;

		bool m_saveWindowActive = false;

	public:
		AenIF::Room GetRoom(size_t index);

		void AddLight(Aen::Entity* entity);

		void AddBase(AenIF::Model& model, AenIF::Texture &texture);
		void AddPointLight(AenIF::Light& input);
		void AddSpotLight(AenIF::Light& input);
		void AddDirectional(AenIF::Light& input);


	public:
		ImGuiHandler();
		~ImGuiHandler();

		void StartUp();
		void RenderAllWindows();

		void Initialize(const HWND& hwnd, ID3D11Device* mp_device, ID3D11DeviceContext* mp_dContext);
		void NewFrame();
		void Render();
		void Release();
		void SceneListWindow();
		void AssetWindow();
		void ColorWheel();
		void PropertyWindow();
		void ToolWindow();
		void SaveWindow();

		void AddModel(Aen::Entity* entity);
		void AddLight(Aen::Entity* entity, string type);
		void ReadAllModelsFromHandler();
		void CreatePreviewTextureThumbnail();

		void SetValues();
		void SetDefaultValue();
		void ZeroValue();
		void SetMaterialValues();

		void AddBase(const string &meshName, const string &objName);
		void AddPointLight();
		void AddSpotLight();
		void AddDirectional();
		void RemoveObject();

		void ReadAllFilesFromResourceFolder();
		bool AddButton(const string& name);
		void ModelButtons();
		void LightButtons();
		const string CheckType(Aen::Entity* entity);

		void CustomCombo(vector<string>& list,string name, string type);

		void ChangeMaterial(int &currentIndex);

		void UpdateMap(unsigned int key, string& texValue, string& matValue, string& meshName, string& texName);
	};
}



