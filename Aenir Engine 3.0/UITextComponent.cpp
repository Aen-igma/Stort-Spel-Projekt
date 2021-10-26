#include "UITextComponent.h"

Aen::UITextComponent::UITextComponent()
{

}

Aen::UITextComponent::~UITextComponent()
{
}

void Aen::UITextComponent::createText()
{
	hr = D2D1CreateFactory(
	D2D1_FACTORY_TYPE_SINGLE_THREADED, &this->m_pD2DFactory);
	if (SUCCEEDED(hr))
	{
		hr = DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
			__uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&this->m_pDWriteFactory)
		);
	}
	this->m_pCharText = L"Hi again!";
	this->m_TextLenght = (UINT32)wcslen(this->m_pCharText);

	if (SUCCEEDED(hr))
	{
		hr = this->m_pDWriteFactory->CreateTextFormat(L"Gabriola", 
			NULL, 
			DWRITE_FONT_WEIGHT_REGULAR,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			72.0f,
			L"en-us",
			&this->m_pTextFormat);
	}
	if (SUCCEEDED(hr))
	{
		hr = this->m_pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	}
	if (SUCCEEDED(hr))
	{
		hr = this->m_pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	}
}

void Aen::UITextComponent::createDeviceResource()
{
	RECT rc;
	GetClientRect(this->hwnd, &rc);
	D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);

	if (!this->m_pRtv)
	{
		hr = this->m_pD2DFactory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(this->hwnd, size), &this->m_pRtv);
	}
	if (SUCCEEDED(hr))
	{
		hr = this->m_pRtv->CreateSolidColorBrush
		(D2D1::ColorF(D2D1::ColorF::Black), &this->m_pBlackBrush);
	}
}

void Aen::UITextComponent::renderText()
{
}
