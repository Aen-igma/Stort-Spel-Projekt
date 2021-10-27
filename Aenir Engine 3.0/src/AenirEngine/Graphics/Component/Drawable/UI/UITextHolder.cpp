#include "PCH.h"
#include "UITextHolder.h"

Aen::UITextHolder::UITextHolder()
{
}

Aen::UITextHolder::~UITextHolder()
{
}

void Aen::UITextHolder::createText(/*std::wstring m_text, LPCWSTR m_fonts*/)
{
	/*UITextData data;
	data.m_UIText = m_text;
	data.m_TextLenght = (UINT32)wcslen(data.m_UIText.c_str());*/
	//Gabriola - textFormat
	ASSERT_HR(m_target2D->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::SkyBlue),&this->m_pBlackBrush));
	ASSERT_HR(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&m_pDWriteFactory)));
	this->m_pCharText = L"Hi again!";
	this->m_TextLenght = (UINT64)wcslen(this->m_pCharText);
	ASSERT_HR(m_pDWriteFactory->CreateTextFormat(L"Arial",
		NULL,
		DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		72.0f,
		L"en-us",
		&this->m_pTextFormat));
	ASSERT_HR(m_pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER));
	ASSERT_HR(m_pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER));
	m_rcf.left = 0;
	m_rcf.top = 50;
	m_rcf.right = 500;
	m_rcf.bottom = 500;
}

void Aen::UITextHolder::createDeviceResources()
{
	D2D1_SIZE_U size = D2D1::SizeU(m_rcf.right - m_rcf.left, m_rcf.bottom - m_rcf.top);
	SafeRelease(&this->m_pBlackBrush);
}

void Aen::UITextHolder::renderText()
{
	this->m_target2D->BeginDraw();
	this->m_target2D->DrawText(L"Make Leauge", this->m_TextLenght, this->m_pTextFormat, this->m_rcf, this->m_pBlackBrush);
	/*this->m_target2D->DrawText(tData.m_UIText.c_str(),tData.m_TextLenght,tData.m_pFormat, tData.rc, tData.m_pBrush);*/
	this->m_target2D->EndDraw();
}
