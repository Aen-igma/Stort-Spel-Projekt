#pragma once
#include"Mesh.h"
#include"..\Drawable.h"

namespace Aen {

	class AEN_DECLSPEC MeshInstance : public Drawable{
		public:
		MeshInstance(const size_t& id);

		void RemoveMesh();
		void SetMesh(Mesh& mesh);
		void PrintMaterialSlots();
		void SetMaterial(Material& material);
		void SetMaterial(const std::string& materialSlotName, Material& material);
		void SetMaterial(const std::string& materialSlotName, const std::string& materialName);

		private:
		~MeshInstance();

		Mesh* m_pMesh;
		std::vector<Material*> m_pMaterials;

		friend class ComponentHandler;
		friend class Renderer;
		friend class Entity;

		virtual void Draw(Renderer& renderer, const uint32_t& layer) override;
		virtual void DepthDraw(Renderer& renderer, const uint32_t& layer) override;
	};

}