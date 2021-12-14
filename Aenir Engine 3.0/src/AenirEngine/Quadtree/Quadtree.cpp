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

	Quadtree::Quadtree(const Aen::Vec3f& MinPos, const Aen::Vec3f& MaxPos,
		const unsigned& level, const unsigned& maxLevel, const unsigned& capacity)
	{
		Aen::Vec3f center, extents;
		DirectX::BoundingBox quad;
		center = (MinPos + MaxPos) * 0.5f;
		extents = (MaxPos - MinPos) * 0.5f;
		quad.Center = DirectX::XMFLOAT3(center.x, center.y, center.z);
		quad.Extents = DirectX::XMFLOAT3(extents.x, extents.y + 20.f, extents.z);

		/*quad.Center = DirectX::XMFLOAT3(460.f, 5.f, 460.f);
		quad.Extents = DirectX::XMFLOAT3(460.f, 20.f, 460.f);*/

		/*quad.Center = DirectX::XMFLOAT3(0.f, 0.f, 0.f);
		quad.Extents = DirectX::XMFLOAT3(50.f, 10.f, 50.f);*/

		mp_root = AEN_NEW Node(quad, level, maxLevel, capacity);
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

	void Quadtree::Update(Renderer& renderer, std::array<std::vector<Drawable*>, 7>& drawtable)
	{
		if (GlobalSettings::GetMainCamera())
		{
			m_quadObjectsToRender.clear();

			m_cameraFrustrum = GlobalSettings::GetMainCamera()->GetComponent<Camera>().GetFrustum();
			m_cameraFrustrum.Far = 30.f;
			mp_root->FrustumTest(m_cameraFrustrum, m_quadObjectsToRender);

			for (int i = 0; i < m_quadObjectsToRender.size(); i++)
			{
				drawtable[m_quadObjectsToRender[i].m_renderLayer].emplace_back(m_quadObjectsToRender[i].mp_drawable);
			}

			for (int i = 0; i < m_autoPass.size(); i++)
			{
				if (m_autoPass[i].mp_drawable->FrustumCull(renderer))
					drawtable[m_autoPass[i].m_renderLayer].emplace_back(m_autoPass[i].mp_drawable);
			}



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

