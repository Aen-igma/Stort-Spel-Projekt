#pragma once
#include"Material.h"
#include"AenMath.h"

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
		void SetMaterial(Material& material);
		void SetMaterial(const std::string& materialSlotName, Material& material);

		private:
		~Mesh();

		VBuffer<Vertex> m_vertices;
		std::vector<Material*> m_pMaterials;
		std::vector<PartitionData> m_partitions;
		std::unordered_map<std::string, uint32_t> m_meshMaterialName;

		friend class ResourceHandler;
		friend class MeshInstance;
		friend class Renderer;
	};

	class AEN_DECLSPEC MeshInstance {
		public:
		MeshInstance();
		MeshInstance(Mesh& mesh);

		void SetMesh(Mesh& mesh);

		private:
		~MeshInstance();

		Mesh* m_mesh;

		friend class ComponentHandler;
		friend class Renderer;
		friend class Entity;
	};
}