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
    };

	class ButtonUI : public GCore
	{
	private:
		std::vector<ButtonData> m_buttonData;
		std::vector<ButtonData> m_tempData;
		int m_nr;
		Vec2f m_gameSize;

		IWICImagingFactory* mp_WFactory;
		IWICBitmapDecoder* mp_BCoder;
		IWICFormatConverter* mp_FormatConverter;
		IWICBitmapFrameDecode* mp_FrameDecode;

		void SetButtonPos(float x, float y, int indX);
		void SetButtonSize(float width, float height, int indX);
	public:
		ButtonUI();
		~ButtonUI();

		void Initialize();
		void AddButton(LPCWSTR path);
		void SetButtonPos(float x, float y);
		void SetButtonSize(float width, float height);
		
		void SaveData();
		bool Intersect(int index);

		std::vector<ButtonData> GetData()const;

        friend class GameLoop;

		Vec2f GetButtonSize(D2D1_RECT_F& rect)const;
		Vec2f GetButtonCenter(D2D1_RECT_F& rect)const;

		void Draw(ButtonData& data);
		void Update(int index);
	};
}
