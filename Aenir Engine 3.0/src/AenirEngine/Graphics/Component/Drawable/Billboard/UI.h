#pragma once
#include "Graphics\Graphics.h"
#include "../Drawable.h"
#include "BuritoMath.h"
#include "Sprite.h"

namespace Aen {

	struct Vertex2D {
		Vec2f pos;
		Vec2f uv;
	};

	class AEN_DECLSPEC UI : public Drawable
	{
	private:
		VBuffer<Vertex2D> m_vertices;

	public:
		UI();
		~UI();

		void Load(const std::string& dir);

		// Inherited via Drawable
		virtual void Draw(Renderer& renderer, const uint32_t& layer = 0) override;
		virtual void DepthDraw(Renderer& renderer, const uint32_t& layer = 0) override;
		virtual bool FrustumCull(Renderer& renderer) override;
	};
}