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
		//std::string name;
		int index;
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
		void AddButton(LPCWSTR path, int indX);
		void SetButtonPos(float x, float y, int indX);
		void SetButtonSize(float width, float height, int indX);
		std::vector<ButtonData> GetData()const;

		friend class GameLoop;

		void Draw(ButtonData& data);
	};
}
