#pragma once
#include "Graphics/DirectX11/DX11Core.h"
#include "Graphics/Component/Resource.h"
#include "Graphics/Component/Entity.h"
#include "Graphics/Component/ComponentHandler.h"
#include "Graphics/Component/Light/Light.h"

#include <filesystem>
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

		AenIF::Room GetRoom(size_t index);
		void AddModel(Aen::Entity* entity);
		void AddLight(Aen::Entity* entity);

		void AddBase(AenIF::Model& model);
		void AddPointLight(AenIF::Light& input);
		void AddSpotLight(AenIF::Light& input);
		void AddDirectional(AenIF::Light& input);

		void ReadAllFilesFromResourceFolder();

		void LoadLevel(int& index);
	};
}