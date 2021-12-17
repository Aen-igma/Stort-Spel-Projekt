#include"PCH.h"
#include"Node.h"

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
	this->m_alreadyAdded = false;
}

Node::Node(DirectX::BoundingBox& quad, const unsigned& level, const unsigned& max_level, const unsigned& capacity)
{
	//Create node and get quad that was calculated by parent
	this->m_areaQuad = quad;
	this->mp_aabbDraw = &Aen::EntityHandler::CreateEntity();
	this->mp_aabbDraw->AddComponent<Aen::AABoundBox>();
	this->mp_aabbDraw->GetComponent<Aen::AABoundBox>().SetBoundingBox(quad.Extents.x, quad.Extents.y, quad.Extents.z);
	this->mp_aabbDraw->SetPos(quad.Center.x, quad.Center.y, quad.Center.z);
	this->mp_aabbDraw->GetComponent<Aen::AABoundBox>().ToggleActive(false);

	this->m_level = level;
	this->m_maxLevel = max_level;
	this->m_capacity = capacity;
	this->m_alreadyAdded = false;

#ifdef _DEBUG
	string temp = "\nme baby Level: " + std::to_string(m_level) + "\nquadPos: " + std::to_string(quad.Center.x) + ", " + std::to_string(quad.Center.y) + ", " + std::to_string(quad.Center.z);
	OutputDebugString(temp.c_str());
#endif

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
	if (m_objs.size() > 0)
		m_objs.clear();
	if(mp_aabbDraw)
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

void Node::FrustumTest(const DirectX::BoundingFrustum& other, std::vector<NodeStruct>& output) //View frustrum culling
{
	if (!mp_children[0])
	{
		if (this->m_areaQuad.Intersects(other) && m_objs.size() > 0)
		{
#ifdef _DEBUG
			this->mp_aabbDraw->GetComponent<Aen::AABoundBox>().ToggleActive(true);
#endif
			for (int i = 0; i < m_objs.size(); i++)
			{
				if (other.Intersects(m_objs[i].m_boundBox))
				{
					this->m_alreadyAdded = false;
					if (output.size() > 0)
					{
						for (int j = 0; j < output.size() && this->m_alreadyAdded == false; j++)
						{
							if (m_objs[i].m_ID == output[j].m_ID)
								this->m_alreadyAdded = true;
						}
					}
					else
					{
						output.emplace_back(NodeStruct(m_objs[i].m_ID, m_objs[i].m_renderLayer, m_objs[i].m_boundBox, m_objs[i].mp_drawable));
						this->m_alreadyAdded = true;
					}
					if(!this->m_alreadyAdded)
						output.emplace_back(NodeStruct(m_objs[i].m_ID, m_objs[i].m_renderLayer, m_objs[i].m_boundBox, m_objs[i].mp_drawable));
				}
			}
		}
#ifdef _DEBUG
		else
			this->mp_aabbDraw->GetComponent<Aen::AABoundBox>().ToggleActive(false);
#endif
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
	DirectX::XMFLOAT3 Center0(m_areaQuad.Center.x - m_areaQuad.Extents.x / 2.f,
		m_areaQuad.Center.y, m_areaQuad.Center.z - m_areaQuad.Extents.z / 2.f);
	DirectX::XMFLOAT3 Extends(m_areaQuad.Extents.x / 2.f, m_areaQuad.Extents.y, m_areaQuad.Extents.z / 2.f);
	DirectX::BoundingBox Quad0(Center0, Extends);
	this->mp_children[0] = AEN_NEW Node(Quad0, m_level, m_maxLevel, m_capacity);

	DirectX::XMFLOAT3 Center1(m_areaQuad.Center.x + m_areaQuad.Extents.x / 2.f,
		m_areaQuad.Center.y, m_areaQuad.Center.z - m_areaQuad.Extents.z / 2.f);
	DirectX::BoundingBox Quad1(Center1, Extends);
	this->mp_children[1] = AEN_NEW Node(Quad1, m_level, m_maxLevel, m_capacity);

	DirectX::XMFLOAT3 Center2(m_areaQuad.Center.x - m_areaQuad.Extents.x / 2.f,
		m_areaQuad.Center.y, m_areaQuad.Center.z + m_areaQuad.Extents.z / 2.f);
	DirectX::BoundingBox Quad2(Center2, Extends);
	this->mp_children[2] = AEN_NEW Node(Quad2, m_level, m_maxLevel, m_capacity);

	DirectX::XMFLOAT3 Center3(m_areaQuad.Center.x + m_areaQuad.Extents.x / 2.f,
		m_areaQuad.Center.y, m_areaQuad.Center.z + m_areaQuad.Extents.z / 2.f);
	DirectX::BoundingBox Quad3(Center3, Extends);
	this->mp_children[3] = AEN_NEW Node(Quad3, m_level, m_maxLevel, m_capacity);

	
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
	}
	
	/*
	bool inserted;
	int counter = 0;
	for (auto& box : m_objs)
	{
		inserted = false;
		if (mp_children[0]->m_areaQuad.Intersects(box.m_boundBox))
		{
			mp_children[0]->Insert(box);
			inserted = true;
		}
		if (mp_children[1]->m_areaQuad.Intersects(box.m_boundBox))
		{
			mp_children[1]->Insert(box);
			inserted = true;
		}
		if (mp_children[2]->m_areaQuad.Intersects(box.m_boundBox))
		{
			mp_children[2]->Insert(box);
			inserted = true;
		}
		if (mp_children[3]->m_areaQuad.Intersects(box.m_boundBox))
		{
			mp_children[3]->Insert(box);
			inserted = true;
		}
		if (inserted)
		{
			counter++;
		}
	}
	assert(counter == m_objs.size()); 
	*/
	m_objs.clear();
	Aen::EntityHandler::RemoveEntity(*mp_aabbDraw);
}