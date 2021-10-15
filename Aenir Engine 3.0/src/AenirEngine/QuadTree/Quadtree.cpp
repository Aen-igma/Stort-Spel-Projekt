#include "PCH.h"
#include "Quadtree.h"

namespace Aen
{

	Quadtree::Quadtree()
	{
		this->mp_root = nullptr;
	}

	Quadtree::Quadtree(DirectX::BoundingBox &quad, const unsigned& level,
		const unsigned& maxLevel, const unsigned& capacity)
	{
		this->mp_root = new Node(quad, level, maxLevel, capacity);
		std::cout << "I'm root: \nLevel : " << level << std::endl << std::endl; //Säger hur långt det gick i levels
	}

	Quadtree::~Quadtree()
	{
		delete this->mp_root;
		for (auto& b : m_boundingVolStructs)
		{
			delete b;
		}
	}

	Node* Quadtree::GetRoot()
	{
		return this->mp_root;
	}

	void Quadtree::Initialize()
	{
		//// Setup First Quad in quadtree
		for (std::pair<uint32_t, Aen::Entity*> element : mp_entityHandlerPtr->m_entities)
		{
			if (Aen::ComponentHandler::MeshInstanceExist(element.first))
			{
				std::cout << "hello :)\n";
				
				ObjeStruct* tempObj = new ObjeStruct(element.first, element.second->GetComponent<MeshInstance>().GetDrawableAABB());
				this->mp_root->Insert(tempObj);
				this->m_boundingVolStructs.push_back(tempObj);
			}
		}

	}

	void Quadtree::Update(std::vector<int>& output)
	{
		if (GlobalSettings::GetMainCamera())
		{
			m_cameraFrustrum = DirectX::BoundingFrustum(GlobalSettings::GetMainCamera()->GetComponent<Camera>().GetProjecton().smMat);
			this->mp_root->IntersectTest(m_cameraFrustrum, output);

		/*	if (Input::KeyDown(Key::V))
			{
				std::cout << "Objects to render: ";
				for (auto& b : output)
					std::cout << b << ", \n";
			}*/
		}
	}

}

