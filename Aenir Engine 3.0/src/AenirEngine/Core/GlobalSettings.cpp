#include"PCH.h"
#include"GlobalSettings.h"

namespace Aen {

	ShaderModel* GlobalSettings::m_pDefaultShader(nullptr);
	Window* GlobalSettings::m_pWindow(nullptr);
	Entity* GlobalSettings::m_pMainCamera(nullptr);
	Entity* GlobalSettings::m_pLightCamera(nullptr);
	Entity* GlobalSettings::m_pDebugCamera(nullptr);
	Color GlobalSettings::m_BGColor(0.08f, 0.08f, 0.13f, 1.f);
	bool GlobalSettings::m_vSync(true);
	bool GlobalSettings::m_useDebugCam(false);
	Renderer* GlobalSettings::m_pRenderer(nullptr);
	Quadtree* GlobalSettings::m_pQuadtree(nullptr);

	Material* GlobalSettings::m_defaultMaterial(nullptr);
	Texture* GlobalSettings::m_defaultTexture(nullptr);
}