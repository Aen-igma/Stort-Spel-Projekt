#pragma once
#include"../Component.h"

namespace Aen {

	class Renderer;

	class AEN_DECLSPEC Drawable : public Component {
		public:
		Drawable(const size_t& id);
		~Drawable();

		private:
		virtual void Draw(Renderer& renderer, const uint32_t& layer = 0) = 0;
		virtual void DepthDraw(Renderer& renderer, const uint32_t& layer = 0) = 0;
		virtual bool FrustumCull(Renderer& renderer) = 0;

		friend class Renderer;
		friend class Quadtree;
	};
}