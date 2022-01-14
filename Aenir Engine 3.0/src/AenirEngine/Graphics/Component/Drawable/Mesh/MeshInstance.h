#pragma once
#include"Mesh.h"
#include"..\Drawable.h"

namespace Aen {

	//struct BoundBoxStoreable
	//{
	//	DirectX::XMFLOAT3 center, extents;
	//};

	class AEN_DECLSPEC MeshInstance : public Drawable{
		public:
		MeshInstance(const size_t& id);

		void RemoveMesh();
		void SetMesh(Mesh& mesh);
		void SetMesh(const std::string& meshName);
		void PrintMaterialSlots();
		void SetMaterial(Material& material);
		void SetMaterial(const std::string& materialName);
		void SetMaterial(const std::string& materialSlotName, Material& material);
		void SetMaterial(const std::string& materialSlotName, const std::string& materialName);
		void SetMaterial(const UINT& index, Material& material);
		//Get AABB based on mesh
		DirectX::BoundingBox GetBox();

		private:
		~MeshInstance();

		Mesh* m_pMesh;
		//BoundBoxStoreable m_boundBox;
		std::vector<Material*> m_pMaterials;



		friend class ComponentHandler;
		friend class Renderer;
		friend class Entity;
		friend class AABoundBox;
		friend class OBBox;
		friend class StaticBody;
		friend class DynamicBody;
		friend class PSSystemcomponent;

		DirectX::BoundingOrientedBox GetOrientedBox();
		virtual void Draw(Renderer& renderer, const uint32_t& layer = 0) override;
		virtual void DepthDraw(Renderer& renderer, const uint32_t& layer = 0) override;
		virtual bool FrustumCull(Renderer& renderer) override;
	};

}
/*
Mat4f m = EntityHandler::GetEntity(m_id).GetTransformation();
			renderer.m_cbTransform.GetData().m_mdlMat = m.Transposed();
			renderer.m_cbTransform.UpdateBuffer();

			DirectX::BoundingOrientedBox box;
			box.Extents = m_pMesh->m_aabb.Extents;
			box.Transform(box, m.smMat);

			if(GlobalSettings::GetMainCamera())
				if(box.Intersects(GlobalSettings::GetMainCamera()->GetComponent<Camera>().GetFrustum())) {
*/