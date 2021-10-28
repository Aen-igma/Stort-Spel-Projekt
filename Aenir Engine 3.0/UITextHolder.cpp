#include "PCH.h"
#include "UITextHolder.h"

namespace Aen
{
	Aen::UITextHolder::UITextHolder()
	{
	}

<<<<<<< HEAD:Aenir Engine 3.0/src/AenirEngine/Graphics/Component/Drawable/UI/UITextHolder.cpp
	Aen::UITextHolder::~UITextHolder()
	{
	}

	void Aen::UITextHolder::createText(std::wstring text)
	{
		UITextData tempData;
		tempData.m_UIText = text;
		tempData.m_TextLenght = (UINT32)wcslen(tempData.m_UIText.c_str());
		ASSERT_HR(m_target2D->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::SkyBlue), &tempData.m_pBrush));
		ASSERT_HR(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&m_pDWriteFactory)));
		ASSERT_HR(m_pDWriteFactory->CreateTextFormat(L"Arial", NULL, DWRITE_FONT_WEIGHT_REGULAR, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 72.0f, L"en-us", &tempData.m_pFormat));
		ASSERT_HR(tempData.m_pFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER));
		ASSERT_HR(tempData.m_pFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER));
		tempData.rc.left = 0;
		tempData.rc.top = 50;
		tempData.rc.right = 500;
		tempData.rc.bottom = 500;
		m_UITextData.push_back(tempData);
	}
	void Aen::UITextHolder::renderText(UITextData& data)
	{
		this->m_target2D->BeginDraw();
		this->m_target2D->DrawText(data.m_UIText.c_str(), data.m_TextLenght, data.m_pFormat, data.rc, data.m_pBrush);
		this->m_target2D->EndDraw();
	}
	std::vector<UITextData> UITextHolder::getData() const
	{
		return m_UITextData;
	}
=======
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
>>>>>>> parent of 23caf9e (UICanDrawText):Aenir Engine 3.0/UITextHolder.cpp
}
