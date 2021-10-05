#pragma once
#include"../../Graphics.h"

namespace Aen {

	class AEN_DECLSPEC Animation {
	public:
		Animation();
		void printTest();
	private:
		~Animation();

		friend class Resource;
		friend class Renderer;
	};

}
