#include "PCH.h"
#include "UIComponent.h"

namespace Aen {

	UIComponent::UIComponent(const size_t& id) 
		:Drawable(id) {}

	UIComponent::~UIComponent() {}

	void UIComponent::AddButton() {}

	void UIComponent::AddText() {}

	void UIComponent::Draw(Renderer& renderer, const uint32_t& layer) {}

	void UIComponent::DepthDraw(Renderer& renderer, const uint32_t& layer) {}
}