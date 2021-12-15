#pragma once
#include "Graphics\Graphics.h"
#include "../Drawable.h"
#include "BuritoMath.h"
#include "Sprite.h"

//Idea
//Can take in an entity and change its values to 2D, maybe change it in the VS or smth
//The component structure may help in this
//ex
//Entity->AddComponent<Aen::UI>(); // Changes the entities values to 2D
//Entity->GetComponent<Aen::UI>().Transform2D(); // If the above statement isn't possible, maybe a function that can do this?
//And then maybe like
//Entity->GetComponent<Aen::UI>().SetPos(0,0);

namespace Aen {

	struct Vertex2D {
		Vec2f pos;
		Vec2f uv;
	};

	class AEN_DECLSPEC UI : public Drawable
	{
	private:
		VBuffer<Vertex2D> m_vertices;
		Mat4f m_transform;

		float width;
		float height;


	public:
		UI(const size_t& id);
		~UI();

		void SetPos(const Vec2f& pos);
		void SetPos(const float& x, const float& y);

		const Vec2f GetPos() const;
		const Vec2f GetScale() const;

		// Inherited via Drawable
		virtual void Draw(Renderer& renderer, const uint32_t& layer = 0) override;
		virtual void DepthDraw(Renderer& renderer, const uint32_t& layer = 0) override;
		virtual bool FrustumCull(Renderer& renderer) override;
	};
}