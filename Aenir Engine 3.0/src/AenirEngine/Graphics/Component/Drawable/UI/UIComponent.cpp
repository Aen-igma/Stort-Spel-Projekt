#include "PCH.h"
#include "UIComponent.h"

Aen::UIComponent::UIComponent()
{
	button.Initialize();
}

Aen::UIComponent::~UIComponent()
{

<<<<<<< HEAD
void Aen::UIComponent::AddText(std::wstring texts)
{
	text.createText(texts);
=======
>>>>>>> parent of bf48282 (Can have multiple buttons)
}

void Aen::UIComponent::AddButton(LPCWSTR dir)
{
	button.AddButton(dir);
}

<<<<<<< HEAD
			button.Draw(b);
			
		}
		for (auto& t : text.getData())
		{
			text.renderText(t);
		}
		
	}
	void Aen::UIComponent::DepthDraw(Renderer& renderer, const uint32_t& id, const uint32_t& layer)
	{
=======
void Aen::UIComponent::AddText()
{
}

void Aen::UIComponent::Draw(Renderer& renderer, const uint32_t& id, const uint32_t& layer)
{
	//Draw button
	button.Draw();
}
>>>>>>> parent of bf48282 (Can have multiple buttons)

void Aen::UIComponent::DepthDraw(Renderer& renderer, const uint32_t& id, const uint32_t& layer)
{

}
