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

bool AABB::intersects(const AABB& bound) const
{
	if (bound.minRight.x <= minLeft.x && bound.maxRight.y <= maxLeft.y)
		return false;
	if (bound.minLeft.x <= minRight.x && bound.maxLeft.y <= maxRight.y)
		return false;
	if (bound.minLeft.y <= maxRight.y)
		return false;
	if (bound.maxRight.y <= minLeft.y)
		return false;
	return true;
}

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

Node::~Node()
{

}

void Node::insert(Object* obj)
{
	if(!m_children[0]) //If Nullptr then this is a leaf 
	{
		if (m_Objects.size() <= m_capacity) //if there is space in this quad
		{
			this->m_Objects.push_back(obj);
		}
		else
		{
			subdivide(); //
			insert(obj); //Skickar obj tillbaka för att checka igen om det är ett leaf

		}
	}
	else //If not nullptr then it is not a leaf
	{
		for (int i = 0; i < 4; i++)//Kollar igenom alla children
		{
			if(m_children[i]->m_NodeAABB->intersects(obj->m_Objbound))
			{
				m_children[i]->m_Objects.push_back(obj);
			}
		}

	}
}


void Node::clear()
{
	//Delete function for objects
}

void Node::subdivide()
{
	//Make New Children
	m_children[0] = new Node(*m_NodeAABB, m_level, m_maxLevel, m_capacity);
	m_children[1] = new Node(*m_NodeAABB, m_level, m_maxLevel, m_capacity);
	m_children[2] = new Node(*m_NodeAABB, m_level, m_maxLevel, m_capacity);
	m_children[3] = new Node(*m_NodeAABB, m_level, m_maxLevel, m_capacity);

	////------------- Make temp Quads ------------//
	//AABB tempQuad1 = AABB(this->m_NodeAABB.minLeft.m_x, this->m_quad.minLeft.m_y, this->m_quad.maxRight.m_x / 2, this->m_quad.maxRight.m_y / 2); //TopLeft
	//AABB tempQuad2 = AABB(this->m_NodeAABB.minRight.m_x / 2, this->m_quad.minRight.m_y, this->m_quad.maxRight.m_x, this->m_quad.maxRight.m_y / 2); //TopRight
	//AABB tempQuad3 = AABB(this->m_NodeAABB.maxLeft.m_x, this->m_quad.maxLeft.m_y / 2, this->m_quad.maxRight.m_x / 2, this->m_quad.maxRight.m_y); //BotLeft
	//AABB tempQuad4 = AABB(this->m_NodeAABB.maxRight.m_x / 2, this->m_quad.maxRight.m_y / 2, this->m_quad.maxRight.m_x, this->m_quad.maxRight.m_y); //BotRight
	//int tempArrSize1 = 0;
	//int tempArrSize2 = 0;
	//int tempArrSize3 = 0;
	//int tempArrSize4 = 0;
	//Object* tempArr1 = new Object;
	//Object* tempArr2 = new Object;
	//Object* tempArr3 = new Object;
	//Object* tempArr4 = new Object;
	////------------- Check which objects is in which quad ---------------//
	//// Compare all elements in m_Objects[] with the temp quads and put objects belonging in a quad into it's corresponding array
	//int nrOfElements = 10;
	//for (int i = 0; i < nrOfElements; i++)
	//{
	//	if (tempQuad1.intersects(m_Objects[i].m_Objbound))
	//		tempArr1[tempArrSize1++] = m_Objects[i].m_Objbound;
	//	if (tempQuad2.intersects(m_Objects[i].m_Objbound))
	//		tempArr2[tempArrSize2++] = m_Objects[i].m_Objbound;
	//	if (tempQuad3.intersects(m_Objects[i].m_Objbound))
	//		tempArr3[tempArrSize3++] = m_Objects[i].m_Objbound;
	//	if (tempQuad4.intersects(m_Objects[i].m_Objbound))
	//		tempArr4[tempArrSize4++] = m_Objects[i].m_Objbound;
	//};
	////------------- Make child quads -------------------//
	//m_children[0] = new Quadtree(tempQuad1, m_capacity, m_maxLevel, m_level + 1, tempArr1, tempArrSize1);
	//m_children[1] = new Quadtree(tempQuad2, m_capacity, m_maxLevel, m_level + 1, tempArr2, tempArrSize2);
	//m_children[2] = new Quadtree(tempQuad3, m_capacity, m_maxLevel, m_level + 1, tempArr3, tempArrSize3);
	//m_children[3] = new Quadtree(tempQuad4, m_capacity, m_maxLevel, m_level + 1, tempArr4, tempArrSize4);
	//m_Objects = nullptr;


}
