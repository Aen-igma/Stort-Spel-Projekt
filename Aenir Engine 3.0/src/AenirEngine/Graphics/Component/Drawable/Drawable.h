#pragma once
#include <DirectXCollision.h>

namespace Aen {

	class Renderer;

	class Drawable {
		public:
		Drawable();
		~Drawable();

		DirectX::BoundingBox* GetDrawableAABB();

		private:
		virtual void Draw(Renderer& renderer, const uint32_t& id, const uint32_t& layer) = 0;
		virtual void DepthDraw(Renderer& renderer, const uint32_t& id, const uint32_t& layer) = 0;

		DirectX::BoundingBox* m_drawableAABB;

		friend class Renderer;
	};
}