#pragma once
#include "Graphics\Graphics.h"
#include "../Drawable.h"
#include "BuritoMath.h"

namespace Aen {

	class AEN_DECLSPEC UIComponent : public Drawable
	{
	private:

	public:
		UIComponent();
		~UIComponent();

		void AddButton();
		void AddText();

		friend class ComponentHandler;
		friend class Renderer;
		friend class Entity;
		friend class GCore;

		// Inherited via Drawable
		virtual void Draw(Renderer& renderer, const uint32_t& id, const uint32_t& layer) override;
		virtual void DepthDraw(Renderer& renderer, const uint32_t& id, const uint32_t& layer) override;
	};
}