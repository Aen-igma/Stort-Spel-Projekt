#pragma once
#include "Node.h"

namespace Aen
{
	class Quadtree
	{
	public:
		Quadtree();
		Quadtree(/*const Aen::Vec3f& MinPos, const Aen::Vec3f& MaxPos, */
			const unsigned& level = 0, const unsigned& maxLevel = 1, const unsigned& capacity = 3);
		~Quadtree();
		Node* GetRoot() const;
		void RebuildAutoPass();

		void Initialize();
		void Update(Renderer& renderer, std::array<std::vector<Drawable*>, 7>& drawtable);
	private:
		Node* mp_root;
		std::vector<NodeStruct> m_autoPass;
		std::vector<NodeStruct> m_quadObjectsToRender;
		DirectX::BoundingFrustum m_cameraFrustrum;
	};
}