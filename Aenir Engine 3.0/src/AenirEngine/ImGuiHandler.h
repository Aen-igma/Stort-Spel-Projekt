#pragma once
#include "Graphics/DirectX11/DX11Core.h"
#include "Graphics/Component/Resource.h"
#include "CreateThumbnail.h"
#include "LevelExporter.h"
#include "LevelImporter.h"
#include <algorithm>
#include "LevelEditor/ImguiType.h"
#include "LevelEditor/ImGuiImporter.h"
#include "Core/GlobalSettings.h"

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
		//void SaveThumbnail(string& destinationPath, string& filePathDestination,
		//	string& sourcePath, string& filePathSource,
		//	Aen::ImageByteData& source, Aen::ImageByteData& destination, int& i);

	private:

		string levelPath = "";
		string m_roomProperty[5] = {"","","","1","1"};
		
		Aen::EntityHandler* mp_entityHandlerPtr;
		vector<Aen::Entity*> m_entityList;

		Aen::ImGuiImporter* imguiImporter;

		// Display in scene list
		vector<string> m_itemList;

		vector<string> m_objFileName;
		vector<string> m_objName;

		vector<string> m_textureFileName;
		vector<string> m_textureName;

		unordered_map< size_t, IGH::ModelContainer> m_modelMap;
		unordered_map< size_t, Aen::Entity*> m_lightMap;
		vector<IGH::MatTexName> m_materialList;




		int m_selectedEntity = 0;

		float m_xyzTranslation[3] = { 0,0,0 };
		float m_xyzRotation[3] = { 0,0,0 };
		float m_xyzScale[3] = { 1,1,1 };

		float m_baseColor[4] = {0,0,0,0};
		float m_shadowColor[4] = { 0,0,0,0 };
		float m_specularColor[4] = { 0,0,0,0 };
		float m_rimLightColor[4] = { 0,0,0,0 };
		float m_innerEdgeColor[4] = { 0,0,0,0 };
		float m_outerEdgeColor[4] = { 0,0,0,0 };

		float m_glowColor[4] = {0,0,0,0};
		float m_glowStr = 0;
		float m_innerEdgeThickness = 0;
		float m_outerEdgeThickness = 0;
		float m_specularPower = 0;
		float m_specularStrength = 0;
		float m_roughness = 0;

		float m_shadowOffset = 0;
		float m_innerFalloff = 0;
		float m_outerFalloff = 0;
		float m_rimLightIntensity = 0;
		float m_rimLightSize = 0;


		
		ImGui::FileBrowser m_fileDialog;

		LevelExporter m_levelExporter;
		AenIMP::LevelImporter m_levelImporter;

		bool m_saveWindowActive = false;
		bool m_ImportWindowActive = false;
		bool m_createEnemyWindowActive = false;
		bool m_createMaterialActive = false;

		// Counters
		unsigned int m_enemyCount = 0;
		int m_entityCount = 0;
		int m_lightCount = 0;
		int m_OriginalCount = 0;

		void print(string input);

	public:
		// All Add func here


		bool AddButton(const string& name);
		void AddEnemyButton();
		void AddMaterialButton();

	public:
		// All Set and Get here
		AenIF::Room GetRoom(size_t index);

		void MaterialDragFloats(float *baseColor, float* shadowColor, float* specularColor, 
			float* rimLightColor, float* innerEdgeColor, float* outerEdgeColor, float* glowColor,
			float & glowStr, float& innerEdgeThickness, float& outerEdgeThickness, float& specularPower,
			float& specularStrength, float& roughness, float& shadowOffset, float& innerFalloff, 
			float& outerFalloff, float& rimLightIntensity, float& rimLightSize
			);

		void SetMaterialValues(float* baseColor, float* shadowColor, float* specularColor,
			float* rimLightColor, float* innerEdgeColor, float* outerEdgeColor, float* glowColor,
			float& glowStr, float& innerEdgeThickness, float& outerEdgeThickness, float& specularPower,
			float& specularStrength, float& roughness, float& shadowOffset, float& innerFalloff,
			float& outerFalloff, float& rimLightIntensity, float& rimLightSize, Aen::Material &material
		);

		void SetMaterialValues(int selected);
		void SetDefaultMaterialValue(int selected);

		void SetValues();

		void SetDefaultValue();
		void ZeroValue();

		void Rotate(float angle);




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
		void MaterialCreateWindow();

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
		//void CreatePreviewTextureThumbnail();
		void RemoveObject();
		void ReadAllFilesFromResourceFolder();
		void UpdateMap(size_t key, string& texValue, string& matValue, string& meshName, string& texName);
	
		void ModelButtons();
		void LightButtons();
		const string CheckType(Aen::Entity* entity);

		bool CustomCombo(vector<string>& list, string name, int &index);

		bool CustomCombo(vector<IGH::MatTexName>& list, string name, int& index);

		string CustomComboString(vector<string>& list, string name, int& index);

		string CustomComboMap(unordered_map< string, AenIF::Material >& list, string name, string index);

		void ChangeTexture(int& currentIndex, string materialName, string materialTextureName);

		void update();

		void DuplicateWindow(bool& active);

		void LightTab();
		void RoomTab();
		void MaterialTab();
		void ModelTab();



};
}



