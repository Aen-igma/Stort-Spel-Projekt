#include"PCH.h"
#include"GlobalSettings.h"

namespace Aen {

	ShaderModel* GlobalSettings::m_pDefaultShader(nullptr);
	Window* GlobalSettings::m_pWindow(nullptr);
	Entity* GlobalSettings::m_pMainCamera(nullptr);
	Color GlobalSettings::m_BGColor(0.08f, 0.08f, 0.13f, 1.f);

	Material* GlobalSettings::m_defaultMaterial(nullptr);
	Texture* GlobalSettings::m_defaultTexture(nullptr);

	ImGuiHandler* GlobalSettings::mp_guiHandler = AEN_NEW ImGuiHandler();
}