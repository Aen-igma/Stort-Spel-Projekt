#include"PCH.h"
#include"Node.h"

NodeStruct::NodeStruct()
{
	this->m_ID = 0;
	this->m_renderLayer = 0;
}

NodeStruct::NodeStruct(size_t ID, uint32_t RenderLayer, DirectX::BoundingBox boundingBox)
{
	this->m_ID = ID;
	this->m_renderLayer = RenderLayer;
	this->m_boundBox = boundingBox;
}

NodeStruct::~NodeStruct()
{
	//delete this->mp_boundBox;
} 

QuadOutput::QuadOutput()
{
	m_ID = 0;
	m_renderLayer = 0;
}

QuadOutput::QuadOutput(size_t ID, uint32_t Layer)
{
	m_ID = ID;
	m_renderLayer = Layer;
}

QuadOutput::~QuadOutput()
{

}


Node::Node()
{
	this->m_areaQuad = DirectX::BoundingBox(DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(10, 10, 10));
	this->m_level = 0;
	this->m_maxLevel = 1;
	this->m_capacity = 3;
}

Node::Node(DirectX::BoundingBox& quad, const unsigned& level, const unsigned& max_level, const unsigned& capacity)
{
	//Create node and get quad that was calculated by parent
	this->m_areaQuad = quad;
	this->m_level = level;
	this->m_maxLevel = max_level;
	this->m_capacity = capacity;
	std::cout << "I'm child: \nLevel : " << level << std::endl << std::endl; 
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
}

void Node::Insert(NodeStruct* obj)
{
	if(!mp_children[0]) //If nullptr then this is a leaf 
	{
		if (m_objs.size() <= m_capacity) //if there is space in this quad
		{
			this->m_objs.push_back(obj);
		}
		else
		{
			if(m_level < m_maxLevel)
			{
				Subdivide(); //
				Insert(obj); //Skickar obj tillbaka för att checka igen om det är ett leaf
			}
		}
	}
	else //If not nullptr then it is not a leaf
	{
		for (int i = 0; i < 4; i++)//Kollar igenom alla children
		{
			if (mp_children[i]->m_areaQuad.Intersects(obj->m_boundBox))
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

void Node::IntersectTest(const DirectX::BoundingFrustum& other, std::vector<QuadOutput>& output) //View frustrum culling
{
	if (!mp_children[0])
	{
		for (auto & obj : m_objs)
		{
				if(other.Intersects(obj->m_boundBox))
				{
					/*this->m_tempQuadObj = QuadOutput(obj->m_ID, obj->m_renderLayer);
					output.emplace_back(&m_tempQuadObj);*/
					output.emplace_back(QuadOutput(obj->m_ID, obj->m_renderLayer));
				}
		}
	}
	else
	{
		for (int i = 0; i < 4; i++)
			mp_children[i]->IntersectTest(other, output);
	}
}

//void Node::SmartPointer(std::shared_ptr<NodeStruct> ptr)
//{
//	std::shared_ptr<NodeStruct> localPtr = ptr;
//	static std::mutex io_mutex; //used to protect shared data from being simultaneously accessed by multiple threads.
//	std::cout << "localPtr in thread:" << "localPtr.get()" << localPtr.get() << std::endl;
//	std::cout << "localPtr in thread:" << "localPtr.usecount()" << localPtr.use_count() << "\n" << std::endl;
//}

void Node::Subdivide()
{
	//------------- Make child quads -------------------//
	this->m_level++;
	DirectX::XMFLOAT3 tempCenter = DirectX::XMFLOAT3(m_areaQuad.Center.x / 2, m_areaQuad.Center.y / 2, m_areaQuad.Center.z);
	DirectX::XMFLOAT3 tempExtends = DirectX::XMFLOAT3(m_areaQuad.Extents.x / 2, m_areaQuad.Extents.y / 2, m_areaQuad.Extents.z);
	DirectX::BoundingBox tempQuad = DirectX::BoundingBox(tempCenter, tempExtends);
	mp_children[0] = new Node(tempQuad, m_level, m_maxLevel, m_capacity);

	tempCenter = DirectX::XMFLOAT3(m_areaQuad.Center.x + m_areaQuad.Extents.x/2,
		m_areaQuad.Center.y - m_areaQuad.Extents.y/2, m_areaQuad.Center.z);
	tempQuad = DirectX::BoundingBox(tempCenter, tempExtends);
	mp_children[1] = new Node(tempQuad, m_level, m_maxLevel, m_capacity);

	tempCenter = DirectX::XMFLOAT3(m_areaQuad.Center.x - m_areaQuad.Extents.x/2,
		m_areaQuad.Center.y + m_areaQuad.Extents.y/2, m_areaQuad.Center.z);
	tempQuad = DirectX::BoundingBox(tempCenter, tempExtends);
	mp_children[2] = new Node(tempQuad, m_level, m_maxLevel, m_capacity);

	tempCenter = DirectX::XMFLOAT3(m_areaQuad.Center.x + m_areaQuad.Extents.x/2,
		m_areaQuad.Center.y + m_areaQuad.Extents.y/2, m_areaQuad.Center.z);
	tempQuad = DirectX::BoundingBox(tempCenter, tempExtends);
	mp_children[3] = new Node(tempQuad, m_level, m_maxLevel, m_capacity);

	//------------- Check which objects is in which quad ---------------//
	for (auto&& box : m_objs)
	{
		if (mp_children[0]->m_areaQuad.Intersects(box->m_boundBox))
			mp_children[0]->Insert(box);
		if (mp_children[1]->m_areaQuad.Intersects(box->m_boundBox))
			mp_children[1]->Insert(box);
		if (mp_children[2]->m_areaQuad.Intersects(box->m_boundBox))
			mp_children[2]->Insert(box);
		if (mp_children[3]->m_areaQuad.Intersects(box->m_boundBox))
			mp_children[3]->Insert(box);
	}
	

	m_objs.clear();
}
