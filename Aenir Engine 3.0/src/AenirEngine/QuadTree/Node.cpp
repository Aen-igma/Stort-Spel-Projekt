#include"PCH.h"
#include"Node.h"

ObjeStruct::ObjeStruct()
{
	this->m_ID = 0;
	this->mp_boundBox = nullptr;
}

ObjeStruct::ObjeStruct(int ID, DirectX::BoundingBox* boundingBox)
{
	this->m_ID = ID;
	this->mp_boundBox = boundingBox;
}

ObjeStruct::~ObjeStruct()
{
	this->mp_boundBox = nullptr;
} 


Node::Node()
{
	this->m_DirectXAABB = DirectX::BoundingBox(DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(10, 10, 10));
	this->m_level = 0;
	this->m_maxLevel = 1;
	this->m_capacity = 3;
}

Node::Node(DirectX::BoundingBox& quad, const unsigned& level, const unsigned& max_level, const unsigned& capacity)
{
	//Create node and get quad that was calculated by parent
	this->m_DirectXAABB = quad;
	this->m_level = level;
	this->m_maxLevel = max_level;
	this->m_capacity = capacity;
	std::cout << "I'm child: \nLevel : " << level << std::endl << std::endl; 
}

Node::~Node()
{
	if (m_children[0])
	{
		for (int i = 0; i < 4; i++)
		{
			delete m_children[i];
		}
	}
}

void Node::insert(ObjeStruct* obj)
{
	if(!m_children[0]) //If nullptr then this is a leaf 
	{
		if (m_Objs.size() <= m_capacity) //if there is space in this quad
		{
			this->m_Objs.push_back(obj);
		}
		else
		{
			if(m_level < m_maxLevel)
			{
				subdivide(); //
				insert(obj); //Skickar obj tillbaka för att checka igen om det är ett leaf
			}
		}
	}
	else //If not nullptr then it is not a leaf
	{
		for (int i = 0; i < 4; i++)//Kollar igenom alla children
		{
			if (m_children[i]->m_DirectXAABB.Intersects(*obj->mp_boundBox))
			{
				m_children[i]->insert(obj);
			}
		}

	}
}

bool Node::inside(DirectX::BoundingBox& playerBox)
{

	if (!m_children[0]) // if player is inside a leaf quad
	{
		if (m_DirectXAABB.Intersects(playerBox))
		{
			std::string tempIds = "";
			for(auto &b : m_Objs)
			{
				tempIds = tempIds + std::to_string(b->m_ID) + " ";
			}
			std::cout << "I'm in a leaf quad on level: " << m_level << std::endl <<
				//"m_Objects is this big: " << m_Objects.size() << std::endl <<

				"this quad has these objects: " << tempIds << std::endl <<
				"m_Objs is this big: " << m_Objs.size() << std::endl;
			return true;
		}
	}
	else
	{
		for(int i = 0; i < 4; i++)
			m_children[i]->inside(playerBox);
	}

	return false;
}

void Node::intersectTest(const DirectX::BoundingFrustum& other, std::vector<int>& output)
{
	if (!m_children[0])
	{
		for (auto & obj : m_Objs)
		{
				if(other.Intersects(*obj->mp_boundBox))
				{
					output.push_back(obj->m_ID); //Läger till ID av objekt
				}
		}
	}
	else
	{
		for (int i = 0; i < 4; i++)
			m_children[i]->intersectTest(other, output);
	}
}

void Node::subdivide()
{
	//------------- Make child quads -------------------//
	this->m_level++;
	DirectX::XMFLOAT3 tempCenter = DirectX::XMFLOAT3(m_DirectXAABB.Center.x / 2, m_DirectXAABB.Center.y / 2, m_DirectXAABB.Center.z);
	DirectX::XMFLOAT3 tempExtends = DirectX::XMFLOAT3(m_DirectXAABB.Extents.x / 2, m_DirectXAABB.Extents.y / 2, m_DirectXAABB.Extents.z);
	DirectX::BoundingBox tempQuad = DirectX::BoundingBox(tempCenter, tempExtends);
	m_children[0] = new Node(tempQuad, m_level, m_maxLevel, m_capacity);

	tempCenter = DirectX::XMFLOAT3(m_DirectXAABB.Center.x + m_DirectXAABB.Extents.x/2,
		m_DirectXAABB.Center.y - m_DirectXAABB.Extents.y/2, m_DirectXAABB.Center.z);
	tempQuad = DirectX::BoundingBox(tempCenter, tempExtends);
	m_children[1] = new Node(tempQuad, m_level, m_maxLevel, m_capacity);

	tempCenter = DirectX::XMFLOAT3(m_DirectXAABB.Center.x - m_DirectXAABB.Extents.x/2, 
		m_DirectXAABB.Center.y + m_DirectXAABB.Extents.y/2, m_DirectXAABB.Center.z);
	tempQuad = DirectX::BoundingBox(tempCenter, tempExtends);
	m_children[2] = new Node(tempQuad, m_level, m_maxLevel, m_capacity);

	tempCenter = DirectX::XMFLOAT3(m_DirectXAABB.Center.x + m_DirectXAABB.Extents.x/2, 
		m_DirectXAABB.Center.y + m_DirectXAABB.Extents.y/2, m_DirectXAABB.Center.z);
	tempQuad = DirectX::BoundingBox(tempCenter, tempExtends);
	m_children[3] = new Node(tempQuad, m_level, m_maxLevel, m_capacity);

	//------------- Check which objects is in which quad ---------------//
	for (auto&& box : m_Objs)
	{
		if (m_children[0]->m_DirectXAABB.Intersects(*box->mp_boundBox))
			m_children[0]->insert(box);
		if (m_children[1]->m_DirectXAABB.Intersects(*box->mp_boundBox))
			m_children[1]->insert(box);
		if (m_children[2]->m_DirectXAABB.Intersects(*box->mp_boundBox))
			m_children[2]->insert(box);
		if (m_children[3]->m_DirectXAABB.Intersects(*box->mp_boundBox))
			m_children[3]->insert(box);
	}
	
	/*for (auto && box : m_Objects)
	{
		if (m_children[0]->m_DirectXAABB.Intersects(*box))
			m_children[0]->insert(box);
		if (m_children[1]->m_DirectXAABB.Intersects(*box))
			m_children[1]->insert(box);
		if (m_children[2]->m_DirectXAABB.Intersects(*box))
			m_children[2]->insert(box);
		if (m_children[3]->m_DirectXAABB.Intersects(*box))
			m_children[3]->insert(box);
	}*/

	m_Objs.clear();
	//m_Objects.clear();
}
