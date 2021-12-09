#include "PCH.h"
#include "Quadtree.h"
#include "Core\GlobalSettings.h"
#include "Graphics/Component/ComponentHandler.h"

namespace Aen
{
	Quadtree::Quadtree()
	{
		mp_root = nullptr;
	}

	Quadtree::Quadtree(const unsigned& level,
		const unsigned& maxLevel, const unsigned& capacity)
	{
		DirectX::BoundingBox quad;
		quad.Center = DirectX::XMFLOAT3(460.f, 0.f, 460.f);
		quad.Extents = DirectX::XMFLOAT3(460.f, 10.f, 460.f);
		mp_root = new Node(quad, level, maxLevel, capacity);
		//std::cout << "I'm root: \nLevel : " << level << std::endl << std::endl; //Säger hur långt det gick i levels
		
	}

	Quadtree::~Quadtree()
	{
		delete mp_root;
		for (auto& b : m_treeStructure)
		{
			delete b;
		}
	}

	Node* Quadtree::GetRoot()
	{
		return mp_root;
	}

	void Quadtree::Initialize()
	{
		DirectX::BoundingBox box;
		Aen::Vec3f centerPoint;
		// Setup tree structure
		//first = ID, second = component,
		for (uint32_t i = 0u; i < 7u; i++)
		{
			for(auto & j: ComponentHandler::m_meshLayer[i])
			{
				if (ComponentHandler::StaticBodyExist(j.first)) // if object has a static body put it in the quadtree
				{
					box = ComponentHandler::GetMeshInstance(j.first).GetBox();
					centerPoint = Aen::Vec3f(box.Center.x, box.Center.y, box.Center.z);
					NodeStruct* tempObj = AEN_NEW NodeStruct(j.first, i, box, centerPoint, j.second);
					mp_root->Insert(tempObj);
					m_treeStructure.emplace_back(tempObj);
				}
				else
				{	
					m_autoPass.emplace_back(NodeStruct(j.first, i, box, centerPoint, j.second));
				}
			}
		}

	}

	std::vector<NodeStruct>& Quadtree::Update()
	{
		if (GlobalSettings::GetMainCamera())
		{
			m_quadObjectsToRender.clear();
			for (auto& i : m_autoPass)
			{
				m_quadObjectsToRender.emplace_back(i);
			}
			//m_quadObjectsToRender.swap(m_autoPass);

			mp_root->PositionTest(m_quadObjectsToRender);

			//m_cameraFrustrum = GlobalSettings::GetMainCamera()->GetComponent<Camera>().GetFrustum();
			//mp_root->FrustumTest(m_cameraFrustrum, m_quadObjectsToRender);

			/*for (auto& i : m_quadObjectsToRender)
			{
				for (auto& j : ComponentHandler::m_meshLayer[i->m_renderLayer])
				{
					if (j.second->GetId() == i->m_ID)
						drawTable[i->m_renderLayer].emplace_back(j.second);
				}
			}*/

			//if (Input::KeyDown(Key::V))
			//{
			/*std::string tempString;
			std::cout << "Objects to render: ";
			for (auto& b : m_QuadObjectsToRender)
			{
				tempString = "(" + std::to_string(b->m_ID) + ", " + std::to_string(b->m_RenderLayer) + ")";
				OutputDebugString(tempString.c_str());
			}
			OutputDebugString("\n");*/
	
			//}
		}
		return m_quadObjectsToRender;
	}

	

}

