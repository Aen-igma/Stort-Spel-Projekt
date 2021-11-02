#include "PCH.h"
#include "UITextHolder.h"

namespace Aen
{
	Aen::UITextHolder::UITextHolder()
	{
	}

	Aen::UITextHolder::~UITextHolder()
	{
		if (m_pDWriteFactory)
			m_pDWriteFactory->Release();

		if (m_Format)
			m_Format->Release();

		if (mp_Brush)
			mp_Brush->Release();

		//for (int i = 0; i < m_UITextData.size(); i++)
		//{
		//	m_UITextData.at(i).m_pFormat->Release();
		//	m_UITextData.at(i).m_pBrush->Release();
		//}
		//m_UITextData.clear();
	}

	void UITextHolder::Initialize()
	{
		//ASSERT_HR(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&m_pDWriteFactory)));
		ASSERT_HR(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&m_pDWriteFactory)));
		ASSERT_HR(m_target2D->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Green), &mp_Brush));
		ASSERT_HR(m_pDWriteFactory->CreateTextFormat(L"Arial", NULL, DWRITE_FONT_WEIGHT_REGULAR, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 72.0f, L"en-us", &m_Format));
		ASSERT_HR(m_Format->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER));
		ASSERT_HR(m_Format->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER));
	}

	void UITextHolder::TextAdd(std::wstring text)
	{

		this->m_text = text;
		//this->m_Lenght = (UINT32)wcslen(this->m_text.c_str());
		this->m_Lenght = m_text.size();
	}

	void Aen::UITextHolder::AddText()
	{
		ASSERT_HR(m_target2D->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Green), &mp_Brush));
		ASSERT_HR(m_pDWriteFactory->CreateTextFormat(L"Arial", NULL, DWRITE_FONT_WEIGHT_REGULAR, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 72.0f, L"en-us", &m_Format));
		ASSERT_HR(m_Format->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER));
		ASSERT_HR(m_Format->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER));

		this->m_text = L"High Tower!";
		this->m_Lenght = m_text.size();
	}

	void Aen::UITextHolder::Draw()
	{
		this->m_target2D->BeginDraw();
		this->m_target2D->DrawText(m_text.c_str(), m_Lenght, m_Format, m_rc, mp_Brush);
		this->m_target2D->EndDraw();
	}

	void Aen::UITextHolder::setTextSize(float width, float height)
	{
		float l = getTextCenter().x + (width / 2.0f);
		float r = getTextCenter().x - (width / 2.0f);
		float b = getTextCenter().y + (height / 2.0f);
		float t = getTextCenter().y - (height / 2.0f);

		this->m_rc.left = l;
		this->m_rc.right = r;
		this->m_rc.top = t;
		this->m_rc.bottom = b;
	}

	void Aen::UITextHolder::setTextPosition(float x, float y)
	{
		this->m_rc.left = x;
		this->m_rc.right = x;
		this->m_rc.top = y;
		this->m_rc.bottom = y;
	}

	Vec2f Aen::UITextHolder::getTextCenter() const
	{
		float xCenter = this->m_rc.right - (getTextSize().x / 2.0f);
		float yCenter = this->m_rc.bottom - (getTextSize().y / 2.0f);
		return Vec2f(xCenter, yCenter);
	}

	Vec2f Aen::UITextHolder::getTextSize() const
	{
		float RectWidth = this->m_rc.right - this->m_rc.left;
		float RectHeight = this->m_rc.bottom - this->m_rc.top;
		return Vec2f(RectWidth, RectHeight);
	}

	//std::vector<UITextData> Aen::UITextHolder::getData() const
	//{
	//	return m_UITextData;
	//}
}

