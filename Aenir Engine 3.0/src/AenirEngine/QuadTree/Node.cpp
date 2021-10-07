#include"PCH.h"
#include"Node.h"


XY::XY(int x, int y)
	:m_x(x), m_y(y)
{

}
AABB::AABB()
{

}

AABB::AABB(const XY& min, const XY& max)
	:minLeft(min), maxRight(max),
	maxLeft(XY(min.m_x, max.m_y)), minRight(XY(max.m_x, min.m_y)),
	m_center(XY(max.m_x / 2, max.m_y / 2))
{

}

AABB::AABB(const double& minX, const double& minY, const double& maxX, const double& maxY)
	:minLeft(XY(minX, minY)), maxRight(XY(maxX, maxY))
{
	this->minRight.m_x = maxRight.m_x;
	this->minRight.m_y = minLeft.m_y;
	this->maxLeft.m_x = minLeft.m_x;
	this->maxLeft.m_y = maxRight.m_y;
	this->m_center.m_x = maxRight.m_x / 2;
	this->m_center.m_y = maxRight.m_y / 2;
}

bool AABB::within(const int& posX, const int& posY) const
{
	// Behöver köras rekursivt tills vi hittar vilken quad spelaren befinner sig i eller tills vi får slut på quads att söka igenom
	if (posX > minLeft.m_x && posX < maxRight.m_x) //Check on X 
	{
		if (posY > minLeft.m_y && posY < maxRight.m_y) //Check on Y
		{
			return true; // return array of objects in quad 
		}
	}
	return false; // return nothing
}

bool AABB::intersects(const AABB& bound) const
{
	if (bound.minRight.m_x <= minLeft.m_x && bound.maxRight.m_y <= maxLeft.m_y)
		return false;
	if (bound.minLeft.m_x <= minRight.m_x && bound.maxLeft.m_y <= maxRight.m_y)
		return false;
	if (bound.minLeft.m_y <= maxRight.m_y)
		return false;
	if (bound.maxRight.m_y <= minLeft.m_y)
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
}

Node::Node(AABB& quad, Object* object, const unsigned& level, const unsigned& max_level, const unsigned& capacity)
{
	//Create node and get quad that was calculated by parent
	this->m_NodeAABB = &quad;
	this->m_level = level;
	this->m_maxLevel = max_level;
	this->m_capacity = capacity;
}

Node::Node(const Aen::Vec2f& min, const Aen::Vec2f& max, Object* object, const unsigned& level, const unsigned& max_level, const unsigned& capacity)
{
	//Create node and build quad from min and max
	this->m_level = level;
	this->m_maxLevel = max_level;
	this->m_capacity = capacity;
}

Node::~Node()
{

}

void Node::initialize()
{

}

void Node::insert()
{
}

void Node::addNode()
{
}

void Node::clear()
{
}

void Node::subdivide()
{

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
