#pragma once
#include"Material.h"
#include"BuritoMath.h"

namespace Aen {

	struct Vertex {
		Vec3f pos;
		Vec3f norm;
		Vec3f tan;
		Vec3f bi;
		Vec2f uv;
	};

	struct PartitionData {
		UINT size;
		UINT offset;
		UINT materialIndex;
	};

	class AEN_DECLSPEC Mesh {
		public:
		Mesh();
		Mesh(const std::string& dir);

		void PrintMaterialSlots();
		void Load(const std::string& dir);

		private:

		~Mesh();

		DirectX::BoundingBox m_aabb;
		DirectX::BoundingOrientedBox m_obb;

		VBuffer<Vertex> m_vertices;
		std::vector<PartitionData> m_partitions;
		std::unordered_map<std::string, uint32_t> m_meshMaterialName;

		friend class Resource;
		friend class MeshInstance;
		friend class Renderer;
		friend class AABoundBox;
		friend class OBBox;
		friend class StaticBody;
		friend class DynamicBody;
	};
}