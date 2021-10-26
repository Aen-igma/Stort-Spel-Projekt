#include "PCH.h"
#include "UITextHolder.h"

Aen::UITextHolder::UITextHolder()
{
}

Aen::UITextHolder::~UITextHolder()
{
}

bool Aen::UITextHolder::createText()
{
	this->hr = D2D1CreateFactory(
		D2D1_FACTORY_TYPE_SINGLE_THREADED, &this->m_pD2DFactory);
	if (SUCCEEDED(this->hr))
	{
		this->hr = DWriteCreateFactory(
			DWRITE_FACTORY_TYPE_SHARED,
			__uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&this->m_pDWriteFactory)
		);
	}
	this->m_pCharText = L"Hi again!";
	this->m_TextLenght = (UINT32)wcslen(this->m_pCharText);

	if (SUCCEEDED(this->hr))
	{
		this->hr = this->m_pDWriteFactory->CreateTextFormat(L"Gabriola",
			NULL,
			DWRITE_FONT_WEIGHT_REGULAR,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			72.0f,
			L"en-us",
			&this->m_pTextFormat);
	}
	if (SUCCEEDED(this->hr))
	{
		this->hr = this->m_pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	}
	if (SUCCEEDED(this->hr))
	{
		this->hr = this->m_pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	}
	return this->hr;
}

bool Aen::UITextHolder::createDeviceResources()
{
	RECT rc;
	GetClientRect(this->hwnd, &rc);
	D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);

	if (!this->m_pRtv)
	{
		this->hr = this->m_pD2DFactory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(this->hwnd, size), &this->m_pRtv);
	}
	if (SUCCEEDED(this->hr))
	{
		this->hr = this->m_pRtv->CreateSolidColorBrush
		(D2D1::ColorF(D2D1::ColorF::Black), &this->m_pBlackBrush);
	}
	SafeRelease(&this->m_pRtv);
	SafeRelease(&this->m_pBlackBrush);
	return this->hr;
}

bool Aen::UITextHolder::renderText()
{

	return this->hr;
}
