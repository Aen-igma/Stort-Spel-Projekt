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

	void Aen::UIComponent::AddButton(LPCWSTR dir, int indX)
	{
		m_button.AddButton(dir, indX);
	}

    void UIComponent::SetButtonSize(float width, float height, int indX)
    {
        m_button.SetButtonSize(width, height, indX);
    }

    void UIComponent::SetButtonPos(float x, float y, int indX)
    {
        m_button.SetButtonPos(x, y, indX);
    }

    bool UIComponent::Intersects(int indX)
    {
       return m_button.Intersect(indX);
    }

    //----------------------	Text	----------------------------//

    void Aen::UIComponent::AddText()
    {
        m_text.createText();
    }

    void UIComponent::SetTextSize(float width, float height)
    {
        m_text.setTextSize(width,height);
    }

    void UIComponent::SetTextPos(float x, float y)
    {
        m_text.setTextPosition(x,y);
    }

    //----------------------	Just pictures	----------------------------//

    void UIComponent::AddPicture(LPCWSTR dir, int indX)
    {
        m_picture.AddPicture(dir, indX);
    }

    void UIComponent::SetPicPos(float x, float y, int indX)
    {
        m_picture.SetPicPos(x, y, indX);
    }

    void UIComponent::SetPicSize(float width, float height, int indX)
    {
        m_picture.SetPicSize(width, height, indX);
    }

    //----------------------	General stuff   ----------------------------//
	void UIComponent::Update()
	{
		for (auto& b : GetData()) {

            m_button.Update(b.index);
		}
	}

    std::vector<ButtonData> UIComponent::GetData() const
    {
        return m_button.GetData();
    }

	void Aen::UIComponent::Draw(Renderer& renderer, const uint32_t& layer)
	{
        //Draw Picture
        for (auto& b : m_picture.GetData()) {

            m_picture.Draw(b);
        }
		//Draw button
		for (auto& b : GetData()) {

            m_button.Draw(b);
        }
		//text.renderText();
    }

	void Aen::UIComponent::DepthDraw(Renderer& renderer, const uint32_t& layer)
	{

    }
}
