#include"PCH.h"
#include"App.h"

namespace Aen {

	App::~App() {

	}

	App::App(const WindowDesc& desc, const std::wstring& windowName, const std::wstring& className)
		:window(desc, windowName, className) {}

	void App::Start() {
	}

	void App::Update(const float& deltaTime) {
	}
}