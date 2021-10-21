#pragma once
#include"BuritoMath.h"
#include <DirectXCollision.h>

namespace Aen {

	class Renderer;

	class Drawable {
		public:
		Drawable();
		~Drawable();

		void SetLayer(const int& layer);
		const int& GetLayer();

		const DirectX::BoundingBox& GetAABB();

		protected:
		DirectX::BoundingBox m_boundingBox;

		private:
		virtual void Draw(Renderer& renderer, const uint32_t& id, const uint32_t& layer) = 0;
		virtual void DepthDraw(Renderer& renderer, const uint32_t& id, const uint32_t& layer) = 0;
		
		friend class Renderer;

		int m_layer;

	};
}