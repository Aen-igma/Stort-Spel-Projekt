#pragma once
#pragma once
#include "Graphics\Graphics.h"
#include "BuritoMath.h"
#include <wincodec.h>
#include <unordered_map>

namespace Aen {

	struct ButtonData
	{
		D2D1_RECT_F rect;
		ID2D1Bitmap* bmp;
	};

	class ButtonUI : public GCore
	{
	private:
		ID2D1Bitmap* testBmp;
		D2D1_RECT_F rect;

		std::unordered_map<std::string, ButtonData> buttonData;


	public:
		ButtonUI();
		~ButtonUI();

		void Initialize();
		void AddButton(LPCWSTR path);

		friend class GameLoop;

		void Draw();
	};
}
