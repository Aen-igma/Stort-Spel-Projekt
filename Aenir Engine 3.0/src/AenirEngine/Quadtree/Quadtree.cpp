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
		/*quad.Center = DirectX::XMFLOAT3(460.f, 0.f, 460.f);
		quad.Extents = DirectX::XMFLOAT3(460.f, 10.f, 460.f);*/

		quad.Center = DirectX::XMFLOAT3(0.f, 0.f, 0.f);
		quad.Extents = DirectX::XMFLOAT3(50.f, 10.f, 50.f);

		mp_root = new Node(quad, level, maxLevel, capacity);
		//std::cout << "I'm root: \nLevel : " << level << std::endl << std::endl; //Säger hur långt det gick i levels
		
	}

	Quadtree::~Quadtree()
	{
		delete mp_root;
	}

	Node* Quadtree::GetRoot() const
	{
		return mp_root;
	}

	void Quadtree::RebuildAutoPass()
	{
		m_autoPass.clear();
		DirectX::BoundingBox box;
		for (uint32_t i = 0u; i < 7u; i++)
		{
			for (auto& j : ComponentHandler::m_meshLayer[i])
			{
				m_autoPass.emplace_back(NodeStruct(j.first, i, box, j.second));
			}
		}
	}

	void Quadtree::Initialize()
	{
		DirectX::BoundingBox box;
		// Setup tree structure
		//first = ID, second = component,
		for (uint32_t i = 0u; i < 7u; i++)
		{
			for(auto & j: ComponentHandler::m_meshLayer[i])
			{
				if (ComponentHandler::StaticBodyExist(j.first)) // if object has a static body put it in the quadtree
				{
					box = ComponentHandler::GetMeshInstance(j.first).GetBox();
					NodeStruct tempObj(j.first, i, box, j.second);
					mp_root->Insert(tempObj);
				}
				else
				{	
					m_autoPass.emplace_back(NodeStruct(j.first, i, box, j.second));
				}
			}
		}

	}

	std::vector<NodeStruct>& Quadtree::Update(Renderer& renderer)
	{
		if (GlobalSettings::GetMainCamera())
		{
			m_quadObjectsToRender.clear();

			for (int i = 0; i < m_autoPass.size(); i++)
			{
				if (m_autoPass[i].mp_drawable->FrustumCull(renderer))
					m_quadObjectsToRender.emplace_back(m_autoPass[i]);
			}

			m_cameraFrustrum = GlobalSettings::GetMainCamera()->GetComponent<Camera>().GetFrustum();
			mp_root->FrustumTest(m_cameraFrustrum, m_quadObjectsToRender);


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

