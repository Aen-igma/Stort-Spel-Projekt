#include "PCH.h"
#include "UIComponent.h"

Aen::UIComponent::UIComponent()
{
	button.Initialize();
}

Aen::UIComponent::~UIComponent()
{

}

void Aen::UIComponent::AddButton(LPCWSTR dir)
{
	button.AddButton(dir);
}

void Aen::UIComponent::AddText()
{
	text.createText();
}

void Aen::UIComponent::Draw(Renderer& renderer, const uint32_t& id, const uint32_t& layer)
{
	//Draw button
	button.Draw();
	text.renderText();
}

void Aen::UIComponent::DepthDraw(Renderer& renderer, const uint32_t& id, const uint32_t& layer)
{

}
