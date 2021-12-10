#pragma once
#include "Node.h"

namespace Aen
{
	class Quadtree
	{
	public:
		Quadtree();
		Quadtree(const unsigned& level = 0, const unsigned& maxLevel = 1, const unsigned& capacity = 3);
		~Quadtree();
		Node* GetRoot() const;
		void RebuildAutoPass();

		void Initialize();
		std::vector<NodeStruct>& Update(Renderer& renderer);
	private:
		Node* mp_root;
		std::vector<NodeStruct> m_autoPass;
		std::vector<NodeStruct> m_quadObjectsToRender;
		DirectX::BoundingFrustum m_cameraFrustrum;
	};
}