#pragma once
#include "Node.h"

namespace Aen
{
	class Quadtree
	{
	public:
		Quadtree();
		Quadtree(const unsigned& level = 0,
			const unsigned& maxLevel = 1, const unsigned& capacity = 3);
		~Quadtree();
		Node* GetRoot();


		void Initialize();
		std::vector<NodeStruct>& Update();
	private:
		Node* mp_root;
		std::vector<NodeStruct*> m_treeStructure;
		std::vector<NodeStruct> m_quadObjectsToRender;
		DirectX::BoundingFrustum m_cameraFrustrum;
		Aen::Vec3f m_playerPosition;
	};
}