#pragma once
#pragma comment(lib,"Dwrite.lib")
#include <dwrite.h>
#include <d2d1_1.h>
#include "Graphics/Graphics.h"

#define ASSERT_HR(hr) assert(SUCCEEDED(hr))


template <class T> void SafeRelease(T** ppT)
{
	if (*ppT)
	{
		(*ppT)->Release();
		*ppT = NULL;
	}
}
struct UITextData
{
	std::string name;
	ID2D1SolidColorBrush* m_pBrush;
	D2D1_RECT_F rc;
	IDWriteTextFormat* m_pFormat;
	UINT32 m_TextLenght;
	std::wstring m_UIText;

};

namespace Aen
{
	class UITextHolder : public GCore
	{
	public:
		UITextHolder();
		~UITextHolder();

		void createText();
		void createDeviceResources();
		void renderText();
	protected:

	private:

		IDWriteFactory* m_pDWriteFactory;
		IDWriteTextFormat* m_pTextFormat;
		const wchar_t* m_pCharText;
		UINT32 m_TextLenght;
		ID2D1SolidColorBrush* m_pBlackBrush;
		

		RECT rc;
		D2D1_RECT_F m_rcf;
		HWND hwnd;
		HRESULT hr;
		
	};



}