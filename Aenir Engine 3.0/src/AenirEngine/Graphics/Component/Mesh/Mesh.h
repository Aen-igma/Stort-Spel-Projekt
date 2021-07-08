#pragma once
#include"..\..\Graphics.h"
#include"AenMath.h"

namespace Aen {

	struct Vertex {
		Vec3f pos;
		Vec3f norm;
		Vec2f uv;
	};

	class Mesh {
		public:
		Mesh() = default;
		Mesh(const std::string& dir);

		void Load(const std::string& dir);

		private:
		~Mesh() = default;

		VBuffer<Vertex> m_vertices;

		friend class MeshHandler;
	};

	class MeshInstance {
		public:
		MeshInstance();
		MeshInstance(Mesh& mesh);

		void SetMesh(Mesh& mesh);

		private:
		~MeshInstance() = default;

		Mesh* m_mesh;

		friend class MeshIHandler;
	};
}