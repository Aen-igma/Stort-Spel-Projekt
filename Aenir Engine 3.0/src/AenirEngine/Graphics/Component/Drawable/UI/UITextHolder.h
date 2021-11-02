#pragma once
#include <dwrite.h>
#include <d2d1_1.h>
#include <string>
#include <wincodec.h>
#include "Graphics/Graphics.h"

struct UITextData
{
	std::wstring m_Text;
	UINT32 m_Lenght;
	IDWriteTextFormat* m_pFormat;
	ID2D1SolidColorBrush* m_pBrush;
	D2D1_RECT_F rc;
	int m_Index;
};

namespace Aen
{
	class UITextHolder : public GCore
	{
	public:
		UITextHolder();
		~UITextHolder();

		//std::vector<UITextData> getData() const;
		void Initialize();
		void TextAdd(std::wstring text);
		void AddText();
		void Draw();

		void setTextSize(float width, float height);
		void setTextPosition(float x, float y);
		Vec2f getTextCenter()const;
		Vec2f getTextSize()const;

	private:
		//std::vector<UITextData> m_UITextData;
		IDWriteFactory* m_pDWriteFactory;

		std::wstring m_text;
		UINT32 m_Lenght;
		IDWriteTextFormat* m_Format;
		D2D1_RECT_F m_rc;
		ID2D1SolidColorBrush* mp_Brush;
	};



}