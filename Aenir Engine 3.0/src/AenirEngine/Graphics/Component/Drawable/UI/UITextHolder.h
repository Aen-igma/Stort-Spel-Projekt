#pragma once
#include <dwrite.h>
#include <d2d1_1.h>
#include <string>
#include <wincodec.h>
#include <sstream>
#include "Graphics/Graphics.h"
using namespace std;

struct UITextData
{
	std::wstring m_Text;
	UINT32 m_Lenght;
	IDWriteTextFormat* m_pFormat;
	ID2D1SolidColorBrush* m_pBrush;
	D2D1_RECT_F rc;
	LPCWSTR m_font;
};

namespace Aen
{
	class UITextHolder : public GCore
	{
	public:
		UITextHolder();
		~UITextHolder();

		std::vector<UITextData> GetData() const;
		void Initialize();
		void AddText(LPCWSTR text, float size);
		void SetFont(LPCWSTR font);
		void SetColor(D2D1::ColorF color);
		void Draw(UITextData& data);
		void ChangeText(int index, LPCWSTR text);

		void setTextSize(float width, float height);
		void setTextSize(float width, float height, int indX);
		void setTextPosition(float x, float y);
		void setTextPosition(float x, float y, int indX);
	private:
		std::vector<UITextData> m_UITextData;
		int m_nr;
		IDWriteFactory* m_pDWriteFactory;
		IDWriteTextFormat* m_Format;
		ID2D1SolidColorBrush* mp_Brush;

		Vec2f getTextCenter(D2D1_RECT_F& rect)const;
		Vec2f getTextSize(D2D1_RECT_F& rect)const;
	};
}