#pragma once
#define AEN_GRAPHICS_DIRECTX11
#include"GlobalSettings.h"

namespace Aen {

	class AEN_DECLSPEC App {
		public:
		virtual ~App();
		App() = delete;
		App(const WindowDesc& desc, const std::wstring& windowName, const std::wstring& className);

		virtual void Start();
		virtual void Update(const float& deltaTime);

		friend class GameLoop;

		protected:
		Window m_window;
	};
}