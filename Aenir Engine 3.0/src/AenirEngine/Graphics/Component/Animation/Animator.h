#pragma once
#include"../../Graphics.h"

namespace Aen {

	class AEN_DECLSPEC Animator {
	public:
		void printSomething();

		friend class ComponentHandler;
		friend class Renderer;
	};
}