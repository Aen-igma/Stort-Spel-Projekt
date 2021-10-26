#include "PCH.h"
#include "UIComponent.h"

Aen::UIComponent::UIComponent()
{
	button.Initialize();
	AddButton();
}

Aen::UIComponent::UIComponent(std::string dir)
{
	//button.Initialize();
}

Aen::UIComponent::~UIComponent()
{

}

void Aen::UIComponent::AddButton()
{
	button.AddButton(L"../Resource/Reimu.png");
}

void Aen::UIComponent::AddText()
{
}

void Aen::UIComponent::Draw(Renderer& renderer, const uint32_t& id, const uint32_t& layer)
{
	//Draw button
	button.Draw();
}

void Aen::UIComponent::DepthDraw(Renderer& renderer, const uint32_t& id, const uint32_t& layer)
{

}
