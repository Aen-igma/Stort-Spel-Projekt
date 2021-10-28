#include "PCH.h"
#include "UIComponent.h"

namespace Aen {

	Aen::UIComponent::UIComponent()
	{
		button.Initialize();
	}

	Aen::UIComponent::~UIComponent()
	{

	}

	void Aen::UIComponent::AddButton(LPCWSTR dir, int indX)
	{
		button.AddButton(dir, indX);
	}

	void UIComponent::SetButtonSize(float width, float height, int indX)
	{
		button.SetButtonSize(width, height, indX);
	}

	void UIComponent::SetButtonPos(float x, float y, int indX)
	{
		button.SetButtonPos(x, y, indX);
	}

void Aen::UIComponent::AddText(std::wstring texts, int indX)
{
	text.createText(texts, indX);
}
void UIComponent::SetTextSize(float width, float height, int indX)
{
	text.setTextSize(width,height,indX);
}
void UIComponent::SetTextPos(float x, float y, int indX)
{
	text.setTextSize(x,y,indX);
}
	void Aen::UIComponent::Draw(Renderer& renderer, const uint32_t& id, const uint32_t& layer)
	{
		//Draw button
		for (auto& b : button.GetData()) {

			button.Draw(b);
			
		}
		for (auto& t : text.getData())
		{
			text.renderText(t);
		}
	}
	void Aen::UIComponent::DepthDraw(Renderer& renderer, const uint32_t& id, const uint32_t& layer)
	{

	}

}
