#include "PCH.h"
#include "UIComponent.h"

namespace Aen {

	Aen::UIComponent::UIComponent(const size_t& id)
		:Drawable(id)
	{
		button.Initialize();
	}

	Aen::UIComponent::~UIComponent()
	{

	}

	void UIComponent::SaveButtonData()
	{
		button.SaveData();
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

	bool UIComponent::getBool() const
	{
		return button.getBool();
	}

	void Aen::UIComponent::AddText()
	{
	}

	void UIComponent::Update(Window& window)
	{
		for (auto& b : button.GetData()) {

			button.Update(window, b.index);
		}
	}

	void Aen::UIComponent::Draw(Renderer& renderer, const uint32_t& layer)
	{
		//Draw button
		for (auto& b : button.GetData()) {

			button.Draw(b);
		}
	}

	void Aen::UIComponent::DepthDraw(Renderer& renderer, const uint32_t& layer)
	{

	}

}
