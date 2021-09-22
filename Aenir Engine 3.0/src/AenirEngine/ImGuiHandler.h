#pragma once
#include "Graphics/DirectX11/DX11Core.h"
#include "Graphics/Component/Resource.h"
#include "Graphics/Component/ComponentHandler.h"
#include "Graphics/Component/Entity.h"

#include "ThirdParty/ImGui/imgui.h"
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
		//bool initialized = false;

		Aen::EntityHandler* entityHandlerPtr;

		vector<Aen::Entity*> entityList;

		vector<string> itemList;
		vector<bool> hitBoxList;
		vector<int> deleteList;

		int selectedEntity = 0;
		float xyzTranslation[3] = { 0,0,0 };
		float xyzRotation[3] = { 0,0,0 };
		float xyzScale[3] = { 1,1,1 };
		int entityCount = 0;
		int cubeCount = 0;
		int planeCount = 0;

	public:
		ImGuiHandler();
		~ImGuiHandler();

		void initialize(HWND& hwnd, ID3D11Device* m_device, ID3D11DeviceContext* m_dContext);
		void newFrame();
		void render();
		void release();
		void sceneListWindow();
		void assetWindow();
		void colorWheel();
		void propertyWindow();
		void toolWindow();
		void materialWindow();
		void update();

		void addModel(Aen::Entity* entity);
		void readAllModelsFromHandler();

		void setValues();
		void setDefaultValue();
		void zeroValue();

		void addCube();
		void addPlane();
		void addBase(string meshName, int& count, string objName);
		void removeObject();

		string checkType(Aen::Entity* entity);
	};
}



