#pragma once
#include "Graphics\Graphics.h"
#include "../Drawable.h"
#include "BuritoMath.h"
#include "ButtonUI.h"
#include "UITextHolder.h"
#include "Picture.h"

namespace Aen {

	class AEN_DECLSPEC UIComponent : public Drawable
	{
	private:
		ButtonUI m_button;
		UITextHolder m_text;
		Picture m_picture;

	public:
		UIComponent(const size_t& id);
		~UIComponent();

		//----------------------	Button	----------------------------//
		void SaveButtonData();
		void AddButton(const std::wstring& dir);
		void SetButtonPos(float x, float y);
		void SetButtonSize(float width, float height);
		bool Intersects(int index);

		//----------------------	Text	----------------------------//
		void AddText(LPCWSTR text, float size);
		void SetTextSize(float width, float height);
		void SetTextSize(float width, float height, int indX);
		void SetTextPos(float x, float y);
		void SetTextPos(float x, float y, int indX);
		void TextNr(int index, LPCWSTR text);
		void SetColor(D2D1::ColorF color);
		void SetFont(LPCWSTR font);

		//----------------------	Just pictures	----------------------------//
		void AddPicture(const std::wstring& dir);
		void SetPicPos(float x, float y);
		void SetPicPos(float x, float y, int indX);
		void SetPicSize(float width, float height);
		void SetPicSize(float width, float height, int indX);
		void UpdatePicture(float width, int indX);
		Vec2f GetPicSize(int indX);
		bool Check(int index);
		
		friend class ComponentHandler;
		friend class Renderer;
		friend class Entity;
		friend class GCore;
		void Update();

		// Inherited via Drawable
		virtual void Draw(Renderer& renderer, const uint32_t& layer = 0) override;
		virtual void DepthDraw(Renderer& renderer, const uint32_t& layer = 0) override;
		virtual bool FrustumCull(Renderer& renderer) override;
	};
}