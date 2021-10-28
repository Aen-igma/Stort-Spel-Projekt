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

	void Aen::UITextHolder::createText()
	{

		this->m_pCharText = L"Hi again!";
		this->m_TextLenght = (UINT32)wcslen(this->m_pCharText);

		ASSERT_HR(m_target2D->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::SkyBlue), &this->m_pBlackBrush));
		ASSERT_HR(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&m_pDWriteFactory)));
		ASSERT_HR(m_pDWriteFactory->CreateTextFormat(L"Ariel", NULL, DWRITE_FONT_WEIGHT_REGULAR, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 72.0f, L"en-us", &this->m_pTextFormat));
		ASSERT_HR(m_pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER));
		ASSERT_HR(m_pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER));

		//this->m_rcf.left = 0;
		//this->m_rcf.top = 50;
		//this->m_rcf.right = 500;
		//this->m_rcf.top = 500;
	}

	void Aen::UITextHolder::createDeviceResources()
	{
		//RECT rc;
		//GetClientRect(this->hwnd, &rc);
		//D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);
		//SafeRelease(&this->m_pBlackBrush);

	}

	void Aen::UITextHolder::renderText()
	{
		this->m_target2D->BeginDraw();
		this->m_target2D->DrawText(L"LIGMA THESE NUTS", this->m_TextLenght, this->m_pTextFormat, this->m_rcf, this->m_pBlackBrush);
		this->m_target2D->EndDraw();
	}

	void Aen::UITextHolder::setTextSize(float width, float height)
	{
		float l = getTextCenter().x + (width / 2.0f);
		float r = getTextCenter().x - (width / 2.0f);
		float b = getTextCenter().y + (height / 2.0f);
		float t = getTextCenter().y - (height / 2.0f);
		this->m_rcf.left = l;
		this->m_rcf.right = r;
		this->m_rcf.top = t;
		this->m_rcf.bottom = b;
	}

	void Aen::UITextHolder::setTextPosition(float x, float y)
	{
		this->m_rcf.left = x;
		this->m_rcf.right = x;
		this->m_rcf.top = y;
		this->m_rcf.bottom = y;
	}

	Vec2f Aen::UITextHolder::getTextCenter() const
	{
		float xCenter = this->m_rcf.right - (getTextSize().x / 2.0f);
		float yCenter = this->m_rcf.top - (getTextSize().y / 2.0f);
		return Vec2f(xCenter, yCenter);
	}

	Vec2f Aen::UITextHolder::getTextSize() const
	{
		float RectWidth = this->m_rcf.right - this->m_rcf.left;
		float RectHeight = this->m_rcf.bottom - this->m_rcf.top;
		return Vec2f(RectWidth, RectHeight);
	}

	std::vector<UITextData> Aen::UITextHolder::getData() const
	{
		return std::vector<UITextData>();
	}
}

