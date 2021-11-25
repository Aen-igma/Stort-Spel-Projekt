#include"PCH.h"
#include"GlobalSettings.h"

namespace Aen {

	ShaderModel* GlobalSettings::m_pDefaultShader(nullptr);
	Window* GlobalSettings::m_pWindow(nullptr);
	Entity* GlobalSettings::m_pMainCamera(nullptr);
	Color GlobalSettings::m_BGColor(0.08f, 0.08f, 0.13f, 1.f);
	bool GlobalSettings::m_vSync(true);
	Renderer* GlobalSettings::m_pRenderer(nullptr);
	//ImGuiHandler* GlobalSettings::mp_guiHandler = AEN_NEW ImGuiHandler();

	Material* GlobalSettings::m_defaultMaterial(nullptr);
	Texture* GlobalSettings::m_defaultTexture(nullptr);
	Camera* GlobalSettings::m_rCam(nullptr);
}