#pragma once
#include "Graphics\Graphics.h"
#include "../Drawable.h"
#include "BuritoMath.h"
#include "ButtonUI.h"

namespace Aen {

	class AEN_DECLSPEC UIComponent : public Drawable
	{
	private:
		ButtonUI button;

	public:
		UIComponent();
		~UIComponent();

		void SaveButtonData();
		void AddButton(LPCWSTR dir, int indX);
		void SetButtonSize(float width, float height, int indX);
		void SetButtonPos(float x, float y, int indX);
		bool getBool()const;

		void AddText();

		friend class ComponentHandler;
		friend class Renderer;
		friend class Entity;
		friend class GCore;

		// Inherited via Drawable
		void Update(Window& window);
		virtual void Draw(Renderer& renderer, const uint32_t& id, const uint32_t& layer) override;
		virtual void DepthDraw(Renderer& renderer, const uint32_t& id, const uint32_t& layer) override;
	};
}