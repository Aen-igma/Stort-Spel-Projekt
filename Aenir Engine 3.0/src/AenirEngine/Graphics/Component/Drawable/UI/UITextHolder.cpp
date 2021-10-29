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

	void Aen::UITextHolder::createText(int index)
	{
		//this->textName = L"Final Fantasy 14"; //Where we type text
		//this->m_TextLenght = (UINT64)wcslen(this->textName.c_str());
		//ASSERT_HR(m_target2D->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Crimson), &this->m_pBlackBrush));
		//ASSERT_HR(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&m_pDWriteFactory)));
		//ASSERT_HR(m_pDWriteFactory->CreateTextFormat(L"Ariel", NULL, DWRITE_FONT_WEIGHT_REGULAR, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 72.0f, L"en-us", &this->m_pTextFormat));
		//ASSERT_HR(m_pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER));
		//ASSERT_HR(m_pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER));

		/*this->textName = L"Final Fantasy 14";*/ //Where we type text

		UITextData tempData;
		tempData.m_Index = index;
		tempData.m_UIText = L"Final Fantasy 14";
		/*this->m_pCharText = L"Final Fantasy 14";*/
		tempData.m_TextLenght = (UINT64)wcslen(tempData.m_UIText.c_str());
		
		ASSERT_HR(m_target2D->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Crimson), &tempData.m_pBrush));
		ASSERT_HR(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&m_pDWriteFactory)));
		ASSERT_HR(m_pDWriteFactory->CreateTextFormat(L"Ariel", NULL, DWRITE_FONT_WEIGHT_REGULAR, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 72.0f, L"en-us", &tempData.m_pFormat));
		ASSERT_HR(tempData.m_pFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER));
		ASSERT_HR(tempData.m_pFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER));
		m_UITextData.push_back(tempData);

	}
	void Aen::UITextHolder::renderText(UITextData& data)
	{
		this->m_target2D->BeginDraw();
		/*this->m_target2D->DrawText(this->textName.c_str(), this->m_TextLenght, this->m_pTextFormat, this->m_rcf, this->m_pBlackBrush);*/
		this->m_target2D->DrawText(data.m_UIText.c_str(), data.m_TextLenght, data.m_pFormat, data.rc, data.m_pBrush);
		this->m_target2D->EndDraw();
	}
	void Aen::UITextHolder::setTextSize(float width, float height, int index)
	{
		//float l = getTextCenter().x + (width / 2.0f);
		//float r = getTextCenter().x - (width / 2.0f);
		//float b = getTextCenter().y + (height / 2.0f);
		//float t = getTextCenter().y - (height / 2.0f);
		//this->m_rcf.left = l;
		//this->m_rcf.right = r;
		//this->m_rcf.top = t;
		//this->m_rcf.bottom = b;

		float l = getTextCenter(index).x + (width / 2.0f);
		float r = getTextCenter(index).x - (width / 2.0f);
		float b = getTextCenter(index).y + (height / 2.0f);
		float t = getTextCenter(index).y - (height / 2.0f);

		this->m_UITextData.at(index).rc.left = l;
		this->m_UITextData.at(index).rc.right = r;
		this->m_UITextData.at(index).rc.top = t;
		this->m_UITextData.at(index).rc.bottom = b;
	}
	void Aen::UITextHolder::setTextPosition(float x, float y, int index)
	{
		//this->m_rcf.left = x;
		//this->m_rcf.right = x;
		//this->m_rcf.top = y;
		//this->m_rcf.bottom = y;

		m_UITextData.at(index).rc.right = x;
		m_UITextData.at(index).rc.left = x;
		m_UITextData.at(index).rc.top = y;
		m_UITextData.at(index).rc.bottom = y;
	}
	void UITextHolder::ChangeName()
	{
		this->textName = L"Game Over!";
	}
	Vec2f Aen::UITextHolder::getTextCenter(int index) const
	{
		//float xCenter = this->m_rcf.right - (getTextSize().x / 2.0f);
		//float yCenter = this->m_rcf.top - (getTextSize().y / 2.0f);
		float xCenter = m_UITextData.at(index).rc.right - (getTextSize(index).x / 2.0f);
		float yCenter = m_UITextData.at(index).rc.top - (getTextSize(index).y / 2.0f);
		return Vec2f(xCenter, yCenter);
	}
	Vec2f Aen::UITextHolder::getTextSize(int index) const
	{
		//float RectWidth = this->m_rcf.right - this->m_rcf.left;
		//float RectHeight = this->m_rcf.bottom - this->m_rcf.top;
		float RectWidth = m_UITextData.at(index).rc.right - m_UITextData.at(index).rc.left;
		float RectHeight = m_UITextData.at(index).rc.bottom - m_UITextData.at(index).rc.top;
		return Vec2f(RectWidth, RectHeight);
	}
	std::vector<UITextData> Aen::UITextHolder::getData() const
	{
		return std::vector<UITextData>();
	}

}

