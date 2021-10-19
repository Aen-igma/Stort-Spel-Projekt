#pragma once
#include"Mesh.h"
#include"..\Drawable.h"
#include <DirectXCollision.h>

namespace Aen {

	class AEN_DECLSPEC MeshInstance : public Drawable{
		public:
		MeshInstance();
		MeshInstance(Mesh& mesh);

		void RemoveMesh();
		void SetMesh(Mesh& mesh);
		void PrintMaterialSlots();
		void SetMaterial(Material& material);
		void SetMaterial(const std::string& materialSlotName, Material& material);
		void SetMaterial(const std::string& materialSlotName, const std::string& materialName);

		DirectX::BoundingBox& GetMeshAABB();
		void SetInQuadtree(bool inQuad); //Put into quad

		
		private:
		~MeshInstance();

		Mesh* m_pMesh;
		std::vector<Material*> m_pMaterials;

		DirectX::BoundingBox m_MeshAABB; //gör om till stack
		bool m_InQuadtree;

		friend class ComponentHandler;
		friend class Renderer;
		friend class Entity;

		virtual void Draw(Renderer& renderer, const uint32_t& id, const uint32_t& layer) override;
		virtual void DepthDraw(Renderer& renderer, const uint32_t& id, const uint32_t& layer) override;
	};

}