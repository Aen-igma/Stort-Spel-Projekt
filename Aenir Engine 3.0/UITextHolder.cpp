#include "PCH.h"
#include "UITextHolder.h"

Aen::UITextHolder::UITextHolder()
{
}

Aen::UITextHolder::~UITextHolder()
{
}

void Aen::UITextHolder::createText()
{
	ASSERT_HR(m_target2D->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::SkyBlue),&this->m_pBlackBrush));
	ASSERT_HR(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&m_pDWriteFactory)));
	this->m_pCharText = L"Hi again!";
	this->m_TextLenght = (UINT32)wcslen(this->m_pCharText);
	ASSERT_HR(m_pDWriteFactory->CreateTextFormat(L"Gabriola",
		NULL,
		DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		72.0f,
		L"en-us",
		&this->m_pTextFormat));
	ASSERT_HR(m_pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER));
	ASSERT_HR(m_pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER));
	
}

void Aen::UITextHolder::createDeviceResources()
{
	RECT rc;
	GetClientRect(this->hwnd, &rc);
	D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);
	SafeRelease(&this->m_pBlackBrush);

}

void Aen::UITextHolder::renderText(UITextData& tData)
{
	this->m_target2D->BeginDraw();
	this->m_target2D->DrawText(tData.m_UIText.c_str(),tData.m_TextLenght,tData.m_pFormat, tData.rc, tData.m_pBrush);
	this->m_target2D->EndDraw();
}
