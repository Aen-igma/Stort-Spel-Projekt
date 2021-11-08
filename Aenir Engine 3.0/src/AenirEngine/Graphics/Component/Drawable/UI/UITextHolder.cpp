#include "PCH.h"
#include "UITextHolder.h"

namespace Aen
{
	Aen::UITextHolder::UITextHolder()
		:m_Format(nullptr), m_pDWriteFactory(nullptr), mp_Brush(nullptr), m_nr(-1)
	{}

	Aen::UITextHolder::~UITextHolder()
	{
		if (m_pDWriteFactory)
			m_pDWriteFactory->Release();

		if (m_Format)
			m_Format->Release();

		if (mp_Brush)
			mp_Brush->Release();

		for (int i = 0; i < m_UITextData.size(); i++)
		{
			m_UITextData.at(i).m_pFormat->Release();
			m_UITextData.at(i).m_pBrush->Release();
		}
		m_UITextData.clear();
	}

	void UITextHolder::Initialize()
	{
		ASSERT_HR(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&m_pDWriteFactory)));
		ASSERT_HR(m_target2D->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Green), &mp_Brush));
		ASSERT_HR(m_pDWriteFactory->CreateTextFormat(L"Arial", NULL, DWRITE_FONT_WEIGHT_REGULAR, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 72.0f, L"en-us", &m_Format));
		ASSERT_HR(m_Format->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER));
		ASSERT_HR(m_Format->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER));
	}

	void UITextHolder::AddText(LPCWSTR text, LPCWSTR font)
	{
		UITextData temp;
		ASSERT_HR(m_target2D->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Green), &temp.m_pBrush));
		ASSERT_HR(m_pDWriteFactory->CreateTextFormat(font, NULL, DWRITE_FONT_WEIGHT_REGULAR, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 72.0f, L"en-us", &temp.m_pFormat));
		ASSERT_HR(temp.m_pFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER));
		ASSERT_HR(temp.m_pFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER));
		m_nr++;

		temp.m_Text = text;
		temp.m_Lenght = temp.m_Text.size();
		m_UITextData.push_back(temp);
	}

	void Aen::UITextHolder::Draw(UITextData& data)
	{
		this->m_target2D->BeginDraw();
		this->m_target2D->DrawText(data.m_Text.c_str(), data.m_Lenght, data.m_pFormat, data.rc, data.m_pBrush);
		this->m_target2D->EndDraw();
	}

	void Aen::UITextHolder::setTextSize(float width, float height)
	{
		float l = getTextCenter(m_UITextData.at(m_nr).rc).x + (width / 2.0f);
		float r = getTextCenter(m_UITextData.at(m_nr).rc).x - (width / 2.0f);
		float b = getTextCenter(m_UITextData.at(m_nr).rc).y + (height / 2.0f);
		float t = getTextCenter(m_UITextData.at(m_nr).rc).y - (height / 2.0f);

		m_UITextData.at(m_nr).rc.left = l;
		m_UITextData.at(m_nr).rc.right = r;
		m_UITextData.at(m_nr).rc.top = t;
		m_UITextData.at(m_nr).rc.bottom = b;
	}

	void Aen::UITextHolder::setTextPosition(float x, float y)
	{
		m_UITextData.at(m_nr).rc.left = x;
		m_UITextData.at(m_nr).rc.right = x;
		m_UITextData.at(m_nr).rc.top = y;
		m_UITextData.at(m_nr).rc.bottom = y;
	}

	Vec2f Aen::UITextHolder::getTextCenter(D2D1_RECT_F& rect) const
	{
		float xCenter = rect.right - (getTextSize(rect).x / 2.0f);
		float yCenter = rect.bottom - (getTextSize(rect).y / 2.0f);
		return Vec2f(xCenter, yCenter);
	}

	Vec2f Aen::UITextHolder::getTextSize(D2D1_RECT_F& rect) const
	{
		float RectWidth = rect.right - rect.left;
		float RectHeight = rect.bottom - rect.top;
		return Vec2f(RectWidth, RectHeight);
	}

	std::vector<UITextData> Aen::UITextHolder::GetData() const
	{
		return m_UITextData;
	}
}

