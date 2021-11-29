#pragma once
#include"Window.h"

namespace Aen {

	struct AEN_DECLSPEC WindowHandle {

		static Window*& GetWindows() {
			static Window* windows;
			return windows;
		}

		static bool HandleMsg() {
			
			if (!(GetWindows()->HandleMsg()))
			{
				return false;
			}
			return true;
		}
	};
}