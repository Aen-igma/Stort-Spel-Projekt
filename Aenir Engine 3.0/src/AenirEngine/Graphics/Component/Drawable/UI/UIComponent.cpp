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

void Aen::UIComponent::AddText()
{
	text.createText();
}

	void Aen::UIComponent::Draw(Renderer& renderer, const uint32_t& id, const uint32_t& layer)
	{
		//Draw button
		for (auto& b : button.GetData()) {

			button.Draw(b);
		}
	}

	void Aen::UIComponent::DepthDraw(Renderer& renderer, const uint32_t& id, const uint32_t& layer)
	{

	}

}
