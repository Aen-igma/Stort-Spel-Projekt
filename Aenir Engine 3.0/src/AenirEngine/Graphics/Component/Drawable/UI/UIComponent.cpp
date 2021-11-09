#include "PCH.h"
#include "UIComponent.h"

namespace Aen {

	Aen::UIComponent::UIComponent(const size_t& id)
		:Drawable(id)
	{
		m_button.Initialize();
        m_picture.Initialize();
        m_text.Initialize();
	}

    Aen::UIComponent::~UIComponent()
    {
    }

    //----------------------	Button	----------------------------//

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

    void UIComponent::SetButtonPos(float x, float y)
    {
        m_button.SetButtonPos(x, y);
    }

    bool UIComponent::Intersects(int index)
    {
       return m_button.Intersect(index);
    }

    //----------------------	Text	----------------------------//

    void Aen::UIComponent::AddText(LPCWSTR text, float size)
    {
        m_text.AddText(text, size);
    }

    void UIComponent::SetTextSize(float width, float height)
    {
        m_text.setTextSize(width,height);
    }

    void UIComponent::SetTextPos(float x, float y)
    {
        m_text.setTextPosition(x, y);
    }

    //----------------------	Just pictures	----------------------------//

    void UIComponent::AddPicture(const std::wstring& dir)
    {
        m_picture.AddPicture(dir.c_str());
    }

    void UIComponent::SetPicPos(float x, float y)
    {
        m_picture.SetPicPos(x, y);
    }

    void UIComponent::SetPicSize(float width, float height)
    {
        m_picture.SetPicSize(width, height);
    }

    void UIComponent::LessenPic(float width, int indX)
    {
        m_picture.LessenPic(width, indX);
    }

    //----------------------	General stuff   ----------------------------//
	void UIComponent::Update()
	{
        for (int i = 0; i < m_button.GetData().size(); i++) {
            m_button.Update(i);
		}
	}

	void Aen::UIComponent::Draw(Renderer& renderer, const uint32_t& layer)
	{
        //Draw Picture
        for (auto& b : m_picture.GetData()) {

            m_picture.Draw(b);
        }
		//Draw button
		for (auto& b : m_button.GetData()) {

            m_button.Draw(b);
        }
        //Draw Text
        for (auto& b :m_text.GetData()) {

            m_text.Draw(b);
        }
    }

	void Aen::UIComponent::DepthDraw(Renderer& renderer, const uint32_t& layer)
	{

    }
}
