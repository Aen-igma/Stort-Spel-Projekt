#pragma once
#include "Graphics\Graphics.h"
#include "../Drawable.h"
#include "BuritoMath.h"
#include "ButtonUI.h"
#include "UITextHolder.h"

namespace Aen {

	class AEN_DECLSPEC UIComponent : public Drawable
	{
	private:
		ButtonUI button;
		UITextHolder text;

	public:
		UIComponent();
		~UIComponent();


		void AddButton(LPCWSTR dir, int indX);
		void SetButtonSize(float width, float height, int indX);
		void SetButtonPos(float x, float y, int indX);
		void AddText(int index);
		void SetTextSize(float width, float height, int index);
		void SetTextPos(float x, float y, int index);

		friend class ComponentHandler;
		friend class Renderer;
		friend class Entity;
		friend class GCore;

		// Inherited via Drawable
		virtual void Draw(Renderer& renderer, const uint32_t& id, const uint32_t& layer) override;
		virtual void DepthDraw(Renderer& renderer, const uint32_t& id, const uint32_t& layer) override;
	};
}