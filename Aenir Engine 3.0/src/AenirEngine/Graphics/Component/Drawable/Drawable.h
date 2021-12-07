#pragma once
#include"BuritoMath.h"
#include <DirectXCollision.h>
#include"../Component.h"

namespace Aen {

	class Renderer;

	class AEN_DECLSPEC Drawable : public Component {
		public:
		Drawable(const size_t& id);
		~Drawable();

		void SetLayer(int& layer);
		const int& GetLayer();

		const DirectX::BoundingBox& GetAABB();

		protected:
		DirectX::BoundingBox m_boundingBox;

		private:
		virtual void Draw(Renderer& renderer, const uint32_t& layer = 0) = 0;
		virtual void DepthDraw(Renderer& renderer, const uint32_t& layer = 0) = 0;
		virtual bool FrustumCull(Renderer& renderer) = 0;

		int m_layer;

		friend class Renderer;
	};
}