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
		//first = ID, second = component,
		//for (std::pair<uint32_t, Aen::Entity*> element : mp_entityHandlerPtr->m_entities)
		//{
		//	if (Aen::ComponentHandler::MeshInstanceExist(element.first))
		//	{
		//		std::cout << "hello :)\n";
		//		
		//		ObjeStruct* tempObj = new ObjeStruct(element.first, element.second->GetComponent<MeshInstance>().GetDrawableAABB());
		//		this->mp_root->Insert(tempObj);
		//		this->m_boundingVolStructs.push_back(tempObj);
		//	}
		//}

		bool valid = true;
		uint32_t layer;
		DirectX::BoundingBox box;
		// Setup First Quad in quadtree
		//first = ID, second = component,
		for(auto & i: ComponentHandler::m_drawables)
		{
			//valid = true;
			if (ComponentHandler::RigidExist(i.first))
			{
				//ComponentHandler::GetRigid(i.first).GetType() == dynamic;
				//ComponentHandler::GetRigid(i.first)
				valid = false;
			}
			if (ComponentHandler::CharacterControllerExist(i.first))
			{
				valid = false;
			}
			
			if (valid)
			{
				layer = i.second->GetLayer() + 3;
				box = i.second->GetAABB();

				box.Extents = i.second->GetAABB().Extents * ComponentHandler::GetScale(i.first).GetScale().smVec;
				box.Center = ComponentHandler::GetTranslation(i.first).GetPos().smVec;

				NodeStruct* tempObj = AEN_NEW NodeStruct(i.first, layer, box);
				this->mp_root->Insert(tempObj);
				this->m_boundingVolStructs.push_back(tempObj);
			}
		}

	}

	void Quadtree::Update()
	{
		if (GlobalSettings::GetMainCamera())
		{

			//m_cameraFrustrum = DirectX::BoundingFrustum(GlobalSettings::GetMainCamera()->GetComponent<Camera>().GetProjecton().smMat);
			//m_cameraFrustrum = DirectX::BoundingFrustum(GlobalSettings::GetMainCamera()->GetComponent<Camera>().GetView().smMat);
			//m_cameraFrustrum = DirectX::BoundingFrustum(GlobalSettings::GetMainCamera()->GetComponent<Camera>().GetVPMatrix().Transposed().smMat);
			m_cameraFrustrum = DirectX::BoundingFrustum(GlobalSettings::GetMainCamera()->GetComponent<Camera>().GetVPMatrix().smMat);
			for (auto& i : ComponentHandler::m_meshLayer)
			{
				i.clear();
			}

			m_QuadObjectsToRender.clear();
			
			DirectX::BoundingFrustum::CreateFromMatrix(m_cameraFrustrum, GlobalSettings::GetMainCamera()->GetComponent<Camera>().GetVPMatrix().smMat);
			this->mp_root->IntersectTest(m_cameraFrustrum, m_QuadObjectsToRender);

			for(auto& i : m_QuadObjectsToRender){
				ComponentHandler::m_meshLayer[i->m_RenderLayer].emplace(i->m_ID, ComponentHandler::m_mesheInstances.at(i->m_ID));
			}




			//if (Input::KeyDown(Key::V))
			//{
			std::string tempString;
			std::cout << "Objects to render: ";
			for (auto& b : m_QuadObjectsToRender)
			{
				tempString = "(" + std::to_string(b->m_ID) + ", " + std::to_string(b->m_RenderLayer) + ")";
				OutputDebugString(tempString.c_str());
			}
			OutputDebugString("\n");
	
			//}
		}
	}

	

}

