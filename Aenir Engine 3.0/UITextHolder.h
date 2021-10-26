#pragma once
#pragma comment(lib,"Dwrite.lib")
#include <dwrite.h>
#include <d2d1.h>

template <class T> void SafeRelease(T** ppT)
{
	if (*ppT)
	{
		(*ppT)->Release();
		*ppT = NULL;
	}
}

namespace Aen
{
	class UITextHolder
	{
	public:
		UITextHolder();
		~UITextHolder();

		bool createText();
		bool createDeviceResources();
		bool renderText();
	protected:

	private:

		IDWriteFactory* m_pDWriteFactory;
		IDWriteTextFormat* m_pTextFormat;
		const wchar_t* m_pCharText;
		UINT32 m_TextLenght;

		ID2D1Factory* m_pD2DFactory;
		ID2D1HwndRenderTarget* m_pRtv;
		ID2D1SolidColorBrush* m_pBlackBrush;

		RECT rc;
		HWND hwnd;
		HRESULT hr;
	};



}