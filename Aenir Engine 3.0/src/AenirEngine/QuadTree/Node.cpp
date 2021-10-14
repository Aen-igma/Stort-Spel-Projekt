#include"PCH.h"
#include"Node.h"


AABB::AABB()
{

}

AABB::AABB(const Aen::Vec2f& min, const Aen::Vec2f& max)
	:minLeft(min), maxRight(max),
	maxLeft(Aen::Vec2f(min.x, max.y)), minRight(Aen::Vec2f(max.x, min.y)),
	m_center(Aen::Vec2f(max.x / 2, max.y / 2))
{
	
}

AABB::AABB(const double& minX, const double& minY, const double& maxX, const double& maxY)
	:minLeft(Aen::Vec2f(minX, minY)), maxRight(Aen::Vec2f(maxX, maxY))
{
	this->minRight.x = maxRight.x;
	this->minRight.y = minLeft.y;
	this->maxLeft.x = minLeft.x;
	this->maxLeft.y = maxRight.y;
	this->m_center.x = maxRight.x / 2;
	this->m_center.y = maxRight.y / 2;
}

bool AABB::within(const int& posX, const int& posY) const
{
	// Behöver köras rekursivt tills vi hittar vilken quad spelaren befinner sig i eller tills vi får slut på quads att söka igenom
	if (posX > minLeft.x && posX < maxRight.x) //Check on X 
	{
		if (posY > minLeft.y && posY < maxRight.y) //Check on Y
		{
			return true; // return array of objects in quad 
		}
	}
	return false; // return nothing
}

//bool AABB::intersects(const AABB& bound) const
//{
//
//	if (bound.minRight.x <= minLeft.x && bound.maxRight.y <= maxLeft.y)
//		return false;
//	if (bound.minLeft.x <= minRight.x && bound.maxLeft.y <= maxRight.y)
//		return false;
//	if (bound.minLeft.y <= maxRight.y)
//		return false;
//	if (bound.maxRight.y <= minLeft.y)
//		return false;
//	return true;
//}

Object::Object(const AABB& boundery, void* data)
	:m_Objbound(boundery), m_data(data)
{
	
}

void Object::setData(void* data)
{
	m_data = data;
}

void* Object::getData() const
{
	return m_data;
}


Node::Node()
{
	this->m_NodeAABB = nullptr;
	this->m_level = 0;
	this->m_maxLevel = 1;
	this->m_capacity = 3;
}

Node::Node(AABB& quad, const unsigned& level, const unsigned& max_level, const unsigned& capacity)
{
	//Create node and get quad that was calculated by parent
	this->m_NodeAABB = &quad;
	this->m_level = level;
	this->m_maxLevel = max_level;
	this->m_capacity = capacity;
	
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
	if (!m_children[0])
	{
		for (int i = 0; i < m_Objects.size(); i++)
		{
			delete m_Objects[i];	
		}
		m_Objects.clear();
	}
}

void Node::insert(DirectX::BoundingBox* obj)
{
	if(!m_children[0]) //If nullptr then this is a leaf 
	{
		if (m_Objects.size() <= m_capacity) //if there is space in this quad
		{
			this->m_Objects.push_back(obj);
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
			if (m_children[i]->m_DirectXAABB.Intersects(*obj))
			{
				m_children[i]->insert(obj);
			}
			/*if(m_children[i]->m_NodeAABB->intersects(obj->m_Objbound))
			{
				m_children[i]->m_Objects.push_back(obj);
			}*/
		}

	}
}

bool Node::inside(DirectX::BoundingBox& playerBox)
{

	if (!m_children[0]) // if player is inside a leaf quad
	{
		if (m_DirectXAABB.Intersects(playerBox))
		{
			std::cout << "I'm in a leaf quad on level: " << m_level << std::endl;
			std::cout << "This Quad has this many objects: " << m_Objects.size() << std::endl;
			return true;
		}
	}
	else
	{
		m_children[0]->inside(playerBox);
		m_children[1]->inside(playerBox);
		m_children[2]->inside(playerBox);
		m_children[3]->inside(playerBox);
	}

	return false;
}


void Node::clear()
{
	/*if (m_children[0])
	{
		for (int i = 0; i < 4; i++)
			m_children[i]->clear();
	}*/
	
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
	for (auto && box : m_Objects)
	{
		if (m_children[0]->m_DirectXAABB.Intersects(*box))
			m_children[0]->insert(box);
		if (m_children[1]->m_DirectXAABB.Intersects(*box))
			m_children[1]->insert(box);
		if (m_children[2]->m_DirectXAABB.Intersects(*box))
			m_children[2]->insert(box);
		if (m_children[3]->m_DirectXAABB.Intersects(*box))
			m_children[3]->insert(box);
	}

	m_Objects.clear();
}
