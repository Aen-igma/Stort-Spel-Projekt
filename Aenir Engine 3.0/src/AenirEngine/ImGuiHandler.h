#pragma once
#include "Graphics/DirectX11/DX11Core.h"
#include "Graphics/Component/Resource.h"
#include "Graphics/Component/Entity.h"
#include "Graphics/Component/ComponentHandler.h"
#include "Graphics/Component/Light/Light.h"

#include <filesystem>
#include "LevelImporter.h"
#include "Graphics/Component/EntityHandler.h"

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


namespace Aen {

	class AEN_DECLSPEC ImGuiHandler {

	private:

		Aen::EntityHandler* mp_entityHandlerPtr;
		vector<Aen::Entity*> m_entityList;
		AenIMP::LevelImporter m_levelImporter;

		size_t m_entityCount;

	public:
		ImGuiHandler();
		~ImGuiHandler();

		vector<Aen::Entity*> GetEntityList();

		void ReadAllFilesFromResourceFolder();

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

		AenIF::Room GetRoom(size_t index);
		void AddModel(Aen::Entity* entity);
		void AddLight(Aen::Entity* entity);

		void AddBase(AenIF::Model& model, AenIF::Texture& texture);
		void AddPointLight(AenIF::Light& input);
		void AddSpotLight(AenIF::Light& input);
		void AddDirectional(AenIF::Light& input);

		bool LoadLevel(int index);


		bool LoadLevel(AenIMP::CompleteRoom* roomPtr, Aen::Vec2f offset, float angle);
		void AddBase(AenIF::Model& model, AenIF::Texture& texture, Aen::Vec2f offset, float angle);
		void AddPointLight(AenIF::Light& input, Aen::Vec2f offset, float angle);
		void AddSpotLight(AenIF::Light& input, Aen::Vec2f offset, float angle);

		AenIMP::LevelImporter* GetImporterPtr();
	};
}