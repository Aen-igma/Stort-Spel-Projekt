#pragma once
#include"Mesh.h"
#include"..\Drawable.h"

namespace Aen {

	class AEN_DECLSPEC MeshInstance : public Drawable{
		public:
		MeshInstance();
		MeshInstance(Mesh& mesh);

		void SetMesh(Mesh& mesh);

		private:
		~MeshInstance();

		Mesh* m_pMesh;

		friend class ComponentHandler;
		friend class Renderer;
		friend class Entity;

		virtual void Draw(Renderer& renderer, const uint32_t& id, const uint32_t& layer) override;
		virtual void DepthDraw(Renderer& renderer, const uint32_t& id, const uint32_t& layer) override;
	};

}