#include "PCH.h"
#include "UIComponent.h"

namespace Aen {

	Aen::UIComponent::UIComponent(const size_t& id)
		:Drawable(id)
	{
		m_button.Initialize();
        m_picture.Initialize();
        m_text.Initialize();

        canDraw = true;
	}

    Aen::UIComponent::~UIComponent()
    {
    }

    //--------------------------------------------    	    Button          --------------------------------------------------//
	void UIComponent::SaveButtonData()
	{
		m_button.SaveData();
	}

	void Aen::UIComponent::AddButton(const std::wstring& dir)
	{
		m_button.AddButton(dir.c_str());
	}

    void UIComponent::SetButtonSize(float width, float height)
    {
        m_button.SetButtonSize(width, height);
    }

    void UIComponent::SetButtonSize(float width, float height, int indX)
    {
        m_button.SetButtonSize(width, height, indX);
    }

    void UIComponent::SetButtonPos(float x, float y)
    {
        m_button.SetButtonPos(x, y);
    }

    void UIComponent::SetButtonPos(float x, float y, int indX)
    {
        m_button.SetButtonPos(x, y, indX);
    }

    bool UIComponent::Intersects(int index)
    {
       return m_button.Intersect(index);
    }

    //--------------------------------------------    	    Text            --------------------------------------------------//

    void Aen::UIComponent::AddText(LPCWSTR text, float size)
    {
        m_text.AddText(text, size);
    }

    void UIComponent::SetTextSize(float width, float height)
    {
        m_text.setTextSize(width,height);
    }

    void UIComponent::SetTextSize(float width, float height, int indX)
    {
        m_text.setTextSize(width, height, indX);
    }

    void UIComponent::SetTextPos(float x, float y)
    {
        m_text.setTextPosition(x, y);
    }

    void UIComponent::SetTextPos(float x, float y, int indX)
    {
        m_text.setTextPosition(x, y, indX);
    }

    void UIComponent::ChangeText(int index, LPCWSTR text)
    {
        m_text.ChangeText(index, text);
    }

    void UIComponent::SetColor(D2D1::ColorF color)
    {
        m_text.SetColor(color);
    }

    void UIComponent::SetFont(LPCWSTR font)
    {
        m_text.SetFont(font);
    }

    //--------------------------------------------    	    Picutres        --------------------------------------------------//

    void UIComponent::AddPicture(const std::wstring& dir)
    {
        m_picture.AddPicture(dir.c_str());
    }

    void UIComponent::SetPicPos(float x, float y)
    {
        m_picture.SetPicPos(x, y);
    }

    void UIComponent::SetPicPos(float x, float y, int indX)
    {
        m_picture.SetPicPos(x, y, indX);
    }

    void UIComponent::SetPicSize(float width, float height)
    {
        m_picture.SetPicSize(width, height);
    }

    void UIComponent::SetPicSize(float width, float height, int indX)
    {
        m_picture.SetPicSize(width, height, indX);
    }

    void UIComponent::UpdatePicture(float width, int indX)
    {
        m_picture.UpdatePicture(width, indX);
    }

    Vec2f UIComponent::GetPicSize(int indX)
    {
        return m_picture.GetPicSize(m_picture.GetData().at(indX).rect);
    }

    bool UIComponent::Check(int index)
    {
        return m_picture.Intersect(index);
    }

    //--------------------------------------------    	General stuff       --------------------------------------------------//
	void UIComponent::Update()
	{
        for (int i = 0; i < m_button.GetData().size(); i++) {
            m_button.Update(i);
		}
	}

    void UIComponent::SetDraw(bool test)
    {
        canDraw = test;
    }

	void Aen::UIComponent::Draw(Renderer& renderer, const uint32_t& layer)
	{
        if (canDraw) {
            //Draw Picture
            for (auto& b : m_picture.GetData()) {

                m_picture.Draw(b);
            }
            //Draw button
            for (auto& b : m_button.GetData()) {

                m_button.Draw(b);
            }
            //Draw Text
            for (auto& b : m_text.GetData()) {

                m_text.Draw(b);
            }
        }
    }

	void Aen::UIComponent::DepthDraw(Renderer& renderer, const uint32_t& layer)
	{

    }
    bool UIComponent::FrustumCull(Renderer& renderer)
    {
        return true;
    }
}
