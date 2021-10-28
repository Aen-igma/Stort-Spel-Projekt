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
		std::vector<ButtonData> buttonData;
		Vec2f GetButtonSize(D2D1_RECT_F& rect);
		Vec2f GetButtonCenter(D2D1_RECT_F& rect);

	public:
		ButtonUI();
		~ButtonUI();

		void Initialize();
		void AddButton(LPCWSTR path);
		void SetButtonPos(float x, float y, D2D1_RECT_F& rect);
		void SetButtonSize(float width, float height, D2D1_RECT_F& rect);

		friend class GameLoop;

		void Draw();
	};
}
