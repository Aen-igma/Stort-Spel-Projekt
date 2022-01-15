#include "PCH.h"
#include "Quadtree.h"
#include "Core\GlobalSettings.h"

namespace Aen
{
	Quadtree::Quadtree(const unsigned& level, const unsigned& maxLevel, const unsigned& capacity)
	{
		DirectX::BoundingBox quad;

		quad.Center = DirectX::XMFLOAT3(460.f, 10.f, 460.f);
		quad.Extents = DirectX::XMFLOAT3(480.f, 20.f, 480.f);

		mp_root = AEN_NEW Node(quad, level, maxLevel, capacity);
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
			m_cameraFrustrum.Far = 80.f;
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
		}
	}

}

