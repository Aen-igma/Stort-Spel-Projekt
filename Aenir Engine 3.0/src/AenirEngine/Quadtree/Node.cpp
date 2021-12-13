#include"PCH.h"
#include"Node.h"

NodeStruct::NodeStruct()
{
	this->m_ID = 0;
	this->m_renderLayer = 0;
	this->m_boundBox = DirectX::BoundingBox();
	this->mp_drawable = nullptr;
}

NodeStruct::NodeStruct(size_t ID, uint32_t RenderLayer, DirectX::BoundingBox box, Aen::Drawable* drawable)
{
	this->m_ID = ID;
	this->m_renderLayer = RenderLayer;
	this->m_boundBox = box;
	this->mp_drawable = drawable;
}

NodeStruct::~NodeStruct()
{
	this->mp_drawable = nullptr;
} 


Node::Node()
{
	this->m_areaQuad = DirectX::BoundingBox(DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(10, 10, 10));
	this->mp_aabbDraw = nullptr;
	this->m_level = 0;
	this->m_maxLevel = 1;
	this->m_capacity = 3;
}

Node::Node(DirectX::BoundingBox& quad, const unsigned& level, const unsigned& max_level, const unsigned& capacity)
{
	//Create node and get quad that was calculated by parent
	this->m_areaQuad = quad;
	this->mp_aabbDraw = &Aen::EntityHandler::CreateEntity();
	this->mp_aabbDraw->AddComponent<Aen::AABoundBox>();
	this->mp_aabbDraw->GetComponent<Aen::AABoundBox>().SetBoundingBox(quad.Extents.x, quad.Extents.y, quad.Extents.z);
	this->mp_aabbDraw->SetPos(quad.Center.x, quad.Center.y, quad.Center.z);

	this->m_level = level;
	this->m_maxLevel = max_level;
	this->m_capacity = capacity;

	string temp = "\nme baby Level: " + std::to_string(m_level) + "\nquadPos: " + std::to_string(quad.Center.x) + ", " + std::to_string(quad.Center.y) + ", " + std::to_string(quad.Center.z);
	OutputDebugString(temp.c_str());
	//std::cout << "I'm child: \nLevel : " << level << std::endl << std::endl; 
}

Node::~Node()
{
	if (mp_children[0])
	{
		for (int i = 0; i < 4; i++)
		{
			delete mp_children[i];
		}
	}
	Aen::EntityHandler::RemoveEntity(*mp_aabbDraw);
}

void Node::Insert(const NodeStruct& obj)
{
	if(!mp_children[0]) //If nullptr then this is a leaf 
	{
		if (m_objs.size() < m_capacity) //if there is space in this quad
		{
			this->m_objs.emplace_back(obj);
		}
		else
		{
			if(m_level < m_maxLevel)
			{
				Subdivide(); 
				Insert(obj); //Skickar obj tillbaka för att checka igen om det är ett leaf
			}
		}
	}
	else //If not nullptr then it is not a leaf
	{
		for (int i = 0; i < 4; i++)//Kollar igenom alla children
		{
			if (mp_children[i]->m_areaQuad.Intersects(obj.m_boundBox))
			{
				mp_children[i]->Insert(obj);
			}
		}
	}
}


//bool Node::Inside(DirectX::BoundingBox& playerBox)
//{
//
//	if (!mp_children[0]) // if player is inside a leaf quad
//	{
//		if (m_DirectXAABB.Intersects(playerBox))
//		{
//			std::string tempIds = "";
//			for(auto &b : m_Objs)
//			{
//				tempIds = tempIds + std::to_string(b->m_ID) + " ";
//			}
//			std::cout << "I'm in a leaf quad on level: " << m_level << std::endl <<
//				//"m_Objects is this big: " << m_Objects.size() << std::endl <<
//
//				"this quad has these objects: " << tempIds << std::endl <<
//				"m_Objs is this big: " << m_Objs.size() << std::endl;
//			return true;
//		}
//	}
//	else
//	{
//		for(int i = 0; i < 4; i++)
//			mp_children[i]->Inside(playerBox);
//	}
//
//	return false;
//}

void Node::FrustumTest(const DirectX::BoundingFrustum& other, std::vector<NodeStruct>& output) //View frustrum culling
{
	if (!mp_children[0])
	{
		if (this->m_areaQuad.Intersects(other))
		{
			this->mp_aabbDraw->GetComponent<Aen::AABoundBox>().ToggleActive(true);
			for (int i = 0; i < m_objs.size(); i++)
			{
				if (other.Intersects(m_objs[i].m_boundBox))
				{
					output.emplace_back(NodeStruct(m_objs[i].m_ID, m_objs[i].m_renderLayer, m_objs[i].m_boundBox, m_objs[i].mp_drawable));
				}
			}
		}
		else
			this->mp_aabbDraw->GetComponent<Aen::AABoundBox>().ToggleActive(false);
	}
	else
	{
		for (int i = 0; i < 4; i++)
			mp_children[i]->FrustumTest(other, output);
	}
}

void Node::Subdivide()
{
	//------------- Make child quads -------------------//
	this->m_level++;
	DirectX::XMFLOAT3 tempCenter = DirectX::XMFLOAT3(m_areaQuad.Center.x / 2.f, m_areaQuad.Center.y, m_areaQuad.Center.z / 2.f);
	DirectX::XMFLOAT3 tempExtends = DirectX::XMFLOAT3(m_areaQuad.Extents.x / 2.f, m_areaQuad.Extents.y, m_areaQuad.Extents.z / 2.f);
	DirectX::BoundingBox tempQuad = DirectX::BoundingBox(tempCenter, tempExtends);
	mp_children[0] = AEN_NEW Node(tempQuad, m_level, m_maxLevel, m_capacity);

	tempCenter = DirectX::XMFLOAT3(m_areaQuad.Center.x + m_areaQuad.Extents.x / 2.f,
		m_areaQuad.Center.y, m_areaQuad.Center.z - m_areaQuad.Extents.z / 2.f);
	tempQuad = DirectX::BoundingBox(tempCenter, tempExtends);
	mp_children[1] = AEN_NEW Node(tempQuad, m_level, m_maxLevel, m_capacity);

	tempCenter = DirectX::XMFLOAT3(m_areaQuad.Center.x - m_areaQuad.Extents.x / 2.f,
		m_areaQuad.Center.y, m_areaQuad.Center.z + m_areaQuad.Extents.z / 2.f);
	tempQuad = DirectX::BoundingBox(tempCenter, tempExtends);
	mp_children[2] = AEN_NEW Node(tempQuad, m_level, m_maxLevel, m_capacity);

	tempCenter = DirectX::XMFLOAT3(m_areaQuad.Center.x + m_areaQuad.Extents.x / 2.f,
		m_areaQuad.Center.y, m_areaQuad.Center.z + m_areaQuad.Extents.z / 2.f);
	tempQuad = DirectX::BoundingBox(tempCenter, tempExtends);
	mp_children[3] = AEN_NEW Node(tempQuad, m_level, m_maxLevel, m_capacity);

	//static bool inserted;
	//------------- Check which objects is in which quad ---------------//
	for (auto& box : m_objs)
	{
		if (mp_children[0]->m_areaQuad.Intersects(box.m_boundBox))
		{
			mp_children[0]->Insert(box);
		}
		if (mp_children[1]->m_areaQuad.Intersects(box.m_boundBox))
		{
			mp_children[1]->Insert(box);
		}
		if (mp_children[2]->m_areaQuad.Intersects(box.m_boundBox))
		{
			mp_children[2]->Insert(box);
		}
		if (mp_children[3]->m_areaQuad.Intersects(box.m_boundBox))
		{
			mp_children[3]->Insert(box);
		}
		/*inserted = false;
		if (mp_children[0]->m_areaQuad.Intersects(box.m_boundBox) && inserted == false)
		{
			mp_children[0]->Insert(box);
			inserted = true;
		}
		if (mp_children[1]->m_areaQuad.Intersects(box.m_boundBox) && inserted == false)
		{
			mp_children[1]->Insert(box);
			inserted = true;
		}
		if (mp_children[2]->m_areaQuad.Intersects(box.m_boundBox) && inserted == false)
		{
			mp_children[2]->Insert(box);
			inserted = true;
		}
		if (mp_children[3]->m_areaQuad.Intersects(box.m_boundBox) && inserted == false)
		{
			mp_children[3]->Insert(box);
			inserted = true;
		}*/
	}

	/*string temp = "\nbaby: " + std::to_string(mp_children[0]->m_level) + "\nsize: " + std::to_string(mp_children[0]->m_objs.size());
	OutputDebugString(temp.c_str());
	temp = "\nbaby: " + std::to_string(mp_children[1]->m_level) + "\nsize: " + std::to_string(mp_children[1]->m_objs.size());
	OutputDebugString(temp.c_str());
	temp = "\nbaby: " + std::to_string(mp_children[2]->m_level) + "\nsize: " + std::to_string(mp_children[2]->m_objs.size());
	OutputDebugString(temp.c_str());
	temp = "\nbaby: " + std::to_string(mp_children[3]->m_level) + "\nsize: " + std::to_string(mp_children[3]->m_objs.size());
	OutputDebugString(temp.c_str());*/
		
	m_objs.clear();
}