#pragma once
#include "Graphics\Graphics.h"
#include "BuritoMath.h"
#include "Window\Input.h"
#include <wincodec.h>
#include <string>

namespace Aen {

    struct ButtonData
    {
        D2D1_RECT_F rect;
        ID2D1Bitmap* bmp;
        int index;
    };

	class ButtonUI : public GCore
	{
	private:
		std::vector<ButtonData> m_buttonData;
		std::vector<ButtonData> m_tempData;
		Vec2f m_gameSize;

		IWICImagingFactory* mp_WFactory;
		IWICBitmapDecoder* mp_BCoder;
		IWICFormatConverter* mp_FormatConverter;
		IWICBitmapFrameDecode* mp_FrameDecode;
	public:
		ButtonUI();
		~ButtonUI();

		void Initialize();
		void AddButton(LPCWSTR path, int indX);
		void SetButtonPos(float x, float y, int indX);
		void SetButtonSize(float width, float height, int indX);
		
		void SaveData();
		bool Intersect(int index);
		std::vector<ButtonData> GetData()const;

        friend class GameLoop;

		Vec2f GetButtonSize(D2D1_RECT_F& rect);
		Vec2f GetButtonCenter(D2D1_RECT_F& rect);

		void Draw(ButtonData& data);
		void Update(int index);
	};
}
