#pragma once
#pragma comment(lib,"Dwrite.lib")
#include <dwrite.h>
#include <d2d1_1.h>
#include "Graphics/Graphics.h"

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

<<<<<<< HEAD:Aenir Engine 3.0/src/AenirEngine/Graphics/Component/Drawable/UI/UITextHolder.h
		void createText(std::wstring m_text);
		void renderText(UITextData& data);
		std::vector<UITextData> getData() const;
=======
		void createText();
		void createDeviceResources();
		void renderText(UITextData& tData);
	protected:
>>>>>>> parent of 23caf9e (UICanDrawText):Aenir Engine 3.0/UITextHolder.h

	private:
		IDWriteFactory* m_pDWriteFactory;
		IDWriteTextFormat* m_pTextFormat;
		const wchar_t* m_pCharText;
		UINT32 m_TextLenght;
		ID2D1SolidColorBrush* m_pBlackBrush;

		//För att skriva mera än en
		std::vector<UITextData> m_UITextData;
	};



}