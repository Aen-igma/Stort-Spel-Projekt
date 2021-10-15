#pragma once
#include "Node.h"
#include "Graphics\Component\Entity.h"
#include "Graphics/Component/ComponentHandler.h"
#include "Core\GlobalSettings.h"

namespace Aen
{

	//MainClass for building quadtree
	class Quadtree
	{
	public:
		Quadtree();
		Quadtree(DirectX::BoundingBox &quad, const unsigned& level = 0,
			const unsigned& maxLevel = 1, const unsigned& capacity = 3);
		~Quadtree();
		Node* GetRoot();

		void Initialize();
		void Update(std::vector<int>& output);

	private:
		Node* mp_root;
		std::vector<ObjeStruct*> m_boundingVolStructs;
		Aen::EntityHandler* mp_entityHandlerPtr;
		DirectX::BoundingFrustum m_cameraFrustrum;

	};
}