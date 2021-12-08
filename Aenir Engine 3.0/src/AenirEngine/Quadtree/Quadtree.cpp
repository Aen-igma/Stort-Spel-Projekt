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
		for (auto& b : m_boundingVolStructs)
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
		bool valid = true;
		uint32_t layer;
		DirectX::BoundingBox box;
		// Setup First Quad in quadtree
		//first = ID, second = component,
		for (uint32_t i = 0u; i < 7u; i++)
		{
			for(auto & j: ComponentHandler::m_meshLayer[i])
			{
				if (ComponentHandler::StaticBodyExist(j.first)) // if object has a static body put it in the quadtree
				{
					layer = j.second->GetLayer() + 3;
					box = j.second->GetAABB();

					box.Extents = j.second->GetAABB().Extents * ComponentHandler::GetScale(j.first).GetScale().smVec;
					box.Center = ComponentHandler::GetTranslation(j.first).GetPos().smVec;

					NodeStruct* tempObj = AEN_NEW NodeStruct(j.first, layer, box);
					mp_root->Insert(tempObj);
					m_boundingVolStructs.emplace_back(tempObj);
				}
			}
		}

	}

	void Quadtree::Update()
	{
		if (GlobalSettings::GetMainCamera())
		{
			m_quadObjectsToRender.clear();
			
			m_cameraFrustrum = GlobalSettings::GetMainCamera()->GetComponent<Camera>().GetFrustum();
			mp_root->IntersectTest(m_cameraFrustrum, m_quadObjectsToRender);


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
		//return m_quadObjectsToRender;
	}

	

}

