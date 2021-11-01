#include "PCH.h"
#include "UIComponent.h"

namespace Aen {

	Aen::UIComponent::UIComponent(const size_t& id)
		:Drawable(id)
	{
		m_button.Initialize();
	}

    Aen::UIComponent::~UIComponent()
    {

    }

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
