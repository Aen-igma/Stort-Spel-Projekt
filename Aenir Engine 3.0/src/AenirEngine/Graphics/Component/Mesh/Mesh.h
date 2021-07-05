#pragma once
#include"MeshHandler.h"
#include"AenMath.h"

namespace Aen {

	struct Vertex {
		Vec3f pos;
		Vec3f norm;
		Vec2f uv;
	};

	class Mesh {
		public:
		Mesh() = delete;
		Mesh(const std::string& name);
		Mesh(const std::string& dir, const std::string& name);

		void Load(const std::string& dir);

		private:
		VBuffer<Vertex> m_vertices;
	};

	class MeshInstance {
		public:
		MeshInstance() = delete;
		MeshInstance(const uint32_t& id, Mesh& mesh);

		void SetMesh(Mesh& mesh);

		private:
		Mesh* m_mesh;
		uint32_t m_id;
	};
}