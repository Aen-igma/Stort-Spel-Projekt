#pragma once
#pragma comment(lib,"Dwrite.lib")
#include <dwrite.h>
#include <d2d1_1.h>
#include "Graphics/Graphics.h"
#include <memory>
#define ASSERT_HR(hr) assert(SUCCEEDED(hr))

struct UITextData
{
	ID2D1SolidColorBrush* m_pBrush;
	D2D1_RECT_F rc;
	IDWriteTextFormat* m_pFormat;
	UINT32 m_TextLenght;
	std::wstring m_UIText;
	int m_Index;
};

namespace Aen
{
	class UITextHolder : public GCore
	{
	public:
		UITextHolder();
		~UITextHolder();

		std::vector<UITextData> getData() const;
		void Initialize();
		void createText();
		void renderText();

		void setTextSize(float width, float height);
		void setTextPosition(float x, float y);
		Vec2f getTextCenter()const;
		Vec2f getTextSize()const;

	private:
		IDWriteFactory* m_pDWriteFactory;
		IDWriteTextFormat* m_pTextFormat;
		std::wstring textName;
		UINT32 m_TextLenght;
		ID2D1SolidColorBrush* m_pBlackBrush;
		D2D1_RECT_F m_rcf;

		//För att skriva mera än en, not done
		std::vector<UITextData> m_UITextData;
	};



}