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
		UIComponent(const size_t& id);
		~UIComponent();

		void SaveButtonData();
		void AddButton(LPCWSTR dir, int indX);
		void SetButtonSize(float width, float height, int indX);
		void SetButtonPos(float x, float y, int indX);
		bool getBool()const;
		void AddText();
		void SetTextSize(float width, float height);
		void SetTextPos(float x, float y);


		void Update(Window& window);

		friend class ComponentHandler;
		friend class Renderer;
		friend class Entity;
		friend class GCore;

		// Inherited via Drawable
		virtual void Draw(Renderer& renderer, const uint32_t& layer) override;
		virtual void DepthDraw(Renderer& renderer, const uint32_t& layer) override;
	};
}