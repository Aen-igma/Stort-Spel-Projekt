#pragma once
#include"..\AenWindow.h"

namespace Aen {

	class AEN_DECLSPEC App {
		public:
		App() = delete;
		App(const WindowDesc& desc, const std::wstring& windowName, const std::wstring& className);

		virtual void Start();
		virtual void Update(const float& deltaTime);

		protected:
		Window window;
	};

	App* CreateApp();
}