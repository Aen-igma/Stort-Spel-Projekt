#include "PCH.h"
#include "UITextHolder.h"

namespace Aen
{
	Aen::UITextHolder::UITextHolder()
	{
	}

	Aen::UITextHolder::~UITextHolder()
	{
	}

	void Aen::UITextHolder::createText(std::wstring text, int index)
	{
		UITextData tempData;
		tempData.m_UIText = text;
		tempData.m_Index = index;
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
	void UITextHolder::setTextSize(float width, float height, int index)
	{
		//left: Top left vertex X Pos
		//top:  Top left vertex Y Pos
		//right: Bottom right vertex X Pos
		//bottom: Bottom right vertex Y Pos
		float l = getTextCenter(index).x + (width / 2.0f);
		float r = getTextCenter(index).x - (width / 2.0f);
		float b = getTextCenter(index).y + (height / 2.0f);
		float t = getTextCenter(index).y - (height / 2.0f);

		m_UITextData.at(index).rc.left = l;
		m_UITextData.at(index).rc.right = r;
		m_UITextData.at(index).rc.top = t;
		m_UITextData.at(index).rc.bottom = b;

	}
	void UITextHolder::setTextPosition(float x, float y, int index)
	{
		m_UITextData.at(index).rc.left = x;
		m_UITextData.at(index).rc.right = x;
		m_UITextData.at(index).rc.top = y;
		m_UITextData.at(index).rc.bottom = y;
	}
	Vec2f UITextHolder::getTextCenter(int index) const
	{
		float xCenter = m_UITextData.at(index).rc.right - (getTextSize(index).x / 2.0f);
		float yCenter = m_UITextData.at(index).rc.top - (getTextSize(index).y / 2.0f);
		return Vec2f(xCenter,yCenter);
	}
	Vec2f UITextHolder::getTextSize(int index) const
	{

		float RectWidth = m_UITextData.at(index).rc.right - m_UITextData.at(index).rc.left;
		float RectHeight = m_UITextData.at(index).rc.bottom - m_UITextData.at(index).rc.top;
		return Vec2f(RectWidth,RectHeight);
	}
}
