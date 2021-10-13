#include "PCH.h"
#include "Quadtree.h"

Quadtree::Quadtree()
{
	this->root = nullptr;
}

//Quadtree::Quadtree(const Aen::Vec2f& min, const Aen::Vec2f& max, const unsigned& capacity, 
//	const unsigned& maxLevel, const unsigned& level)
//{
//	this->root = new Node(min, max, capacity, maxLevel, level);
//	std::cout << "I'm done: \nLevel : " << level << std::endl << std::endl; //Säger hur långt det gick i levels
//}

Quadtree::Quadtree(AABB quad, const unsigned& level, 
	const unsigned& maxLevel, const unsigned& capacity)
{
	this->root = new Node(quad, capacity, maxLevel, level);

	std::cout << "I'm done: \nLevel : " << level << std::endl << std::endl; //Säger hur långt det gick i levels
}

Quadtree::Quadtree(DirectX::BoundingBox &quad, const unsigned& level,
	const unsigned& maxLevel, const unsigned& capacity)
{
	this->root = new Node(quad, level, maxLevel, capacity);

	std::cout << "I'm root: \nLevel : " << level << std::endl << std::endl; //Säger hur långt det gick i levels
}

Quadtree::~Quadtree()
{
	delete this->root;
}

void Quadtree::insertNode(DirectX::BoundingBox* obj)
{
	this->root->insert(obj); //Har en parent realtion med node - detaljerad beskriving senare 
}

Node* Quadtree::getRoot()
{
	return this->root;
}



//void Quadtree::initializeTree()
//{
//	while (m_nrOf > m_capacity)
//	{
//		subdivide();
//		for (int i; i < 4; i++)
//		{
//			m_children[i]->initializeTree();
//		};
//	}
//	std::cout << "I'm done :)\nLevel: " << m_level << std::endl; //Säger hur långt det gick i levels
//}

//Object* Quadtree::findActiveQuad(const int& posX, const int& posY)
//{
//	bool quadFound = false;
//	if (m_isLeaf)
//	{
//		quadFound = m_quad.within(posX, posY);
//	}
//	else
//	{
//		for (int i = 0; i < 4; i++)
//		{
//			m_children[i]->findActiveQuad(posX, posY);
//		}
//	}
//	if (quadFound)
//	{
//		return m_Objects;
//	}
//	return nullptr;
//}

//void Quadtree::subdivide()
//{
//	//------------- Make temp Quads ------------//
//	AABB tempQuad1 = AABB(this->m_quad.minLeft.m_x, this->m_quad.minLeft.m_y, this->m_quad.maxRight.m_x / 2, this->m_quad.maxRight.m_y / 2); //TopLeft
//	AABB tempQuad2 = AABB(this->m_quad.minRight.m_x / 2, this->m_quad.minRight.m_y, this->m_quad.maxRight.m_x, this->m_quad.maxRight.m_y / 2); //TopRight
//	AABB tempQuad3 = AABB(this->m_quad.maxLeft.m_x, this->m_quad.maxLeft.m_y / 2, this->m_quad.maxRight.m_x / 2, this->m_quad.maxRight.m_y); //BotLeft
//	AABB tempQuad4 = AABB(this->m_quad.maxRight.m_x / 2, this->m_quad.maxRight.m_y / 2, this->m_quad.maxRight.m_x, this->m_quad.maxRight.m_y); //BotRight
//	
//	int tempArrSize1 = 0;
//	int tempArrSize2 = 0;
//	int tempArrSize3 = 0;
//	int tempArrSize4 = 0;
//
//	Object* tempArr1 = new Object;
//	Object* tempArr2 = new Object;
//	Object* tempArr3 = new Object;
//	Object* tempArr4 = new Object;
//
//	//------------- Check which objects is in which quad ---------------//
//	// Compare all elements in m_Objects[] with the temp quads and put objects belonging in a quad into it's corresponding array
//	int nrOfElements = 10;
//	for(int i = 0; i < nrOfElements ; i++)
//	{
//		if (tempQuad1.intersects(m_Objects[i].m_Objbound))
//			tempArr1[tempArrSize1++] = m_Objects[i].m_Objbound;
//		if (tempQuad2.intersects(m_Objects[i].m_Objbound))
//			tempArr2[tempArrSize2++] = m_Objects[i].m_Objbound;
//		if (tempQuad3.intersects(m_Objects[i].m_Objbound))
//			tempArr3[tempArrSize3++] = m_Objects[i].m_Objbound;
//		if (tempQuad4.intersects(m_Objects[i].m_Objbound))
//			tempArr4[tempArrSize4++] = m_Objects[i].m_Objbound;
//	};
//
//	//------------- Make child quads -------------------//
//	m_children[0] = new Quadtree(tempQuad1, m_capacity, m_maxLevel, m_level + 1, tempArr1, tempArrSize1);
//	m_children[1] = new Quadtree(tempQuad2, m_capacity, m_maxLevel, m_level + 1, tempArr2, tempArrSize2);
//	m_children[2] = new Quadtree(tempQuad3, m_capacity, m_maxLevel, m_level + 1, tempArr3, tempArrSize3);
//	m_children[3] = new Quadtree(tempQuad4, m_capacity, m_maxLevel, m_level + 1, tempArr4, tempArrSize4);
//
//	m_Objects = nullptr;
//}

//void Quadtree::update(Object* object)
//{
//	//Quadtree* node = object->m_qt;
//	//if (node->m_parent == nullptr || object->m_Objbound.within(node->m_bound))
//	//{
//	//	return;
//	//}
//	//node->remove(object);
//	//do
//	//{
//	//	node->m_parent;
//	//	if (object->m_Objbound.within(node->m_bound))
//	//	{
//	//		break;
//	//	}
//	//} while (node->m_parent != nullptr);
//	//node->insert(object);
//}
//
//void Quadtree::clear()
//{
//	if(m_nrOf > 0)
//	{
//		delete[] m_Objects;
//	}
//	if (!m_isLeaf)
//	{
//		for (int i = 0; i < 4; i++)
//		{
//			delete m_children[i];
//		}
//		m_isLeaf = true;
//	}
//}

//Quadtree* Quadtree::getChild(const AABB &bound) const
//{
//	bool m_bottom = bound.bottom > m_centerY;
//	bool m_left = bound.left < m_centerX;
//	bool m_top = !m_bottom && m_bound.top < m_centerY;
//	if (m_left && bound.right < m_centerX) {
//		if (m_top)    return m_children[1]; // top left
//		if (m_bottom) return m_children[2]; // bottom left
//	}
//	else if (!m_left) {
//		if (m_top)    return m_children[0]; // top right
//		if (m_bottom) return m_children[3]; // bottom right
//	}
//	return nullptr;
//}

//AABB::AABB()
//{
//
//}
//
//AABB::AABB(const XY& min, const XY& max)
//	:minLeft(min), maxRight(max), 
//	maxLeft(XY(min.m_x, max.m_y)), minRight(XY(max.m_x, min.m_y)), 
//	m_center(XY(max.m_x/2, max.m_y/2))
//{
//	
//}
//
//AABB::AABB(const double& minX, const double& minY, const double& maxX, const double& maxY)
//	:minLeft(XY(minX, minY)), maxRight(XY(maxX, maxY))
//{
//	this->minRight.m_x = maxRight.m_x;
//	this->minRight.m_y = minLeft.m_y;
//	this->maxLeft.m_x = minLeft.m_x;
//	this->maxLeft.m_y = maxRight.m_y;
//	this->m_center.m_x = maxRight.m_x / 2;
//	this->m_center.m_y = maxRight.m_y / 2;
//}
//
//bool AABB::within(const int& posX, const int& posY) const
//{
//	// Behöver köras rekursivt tills vi hittar vilken quad spelaren befinner sig i eller tills vi får slut på quads att söka igenom
//	if(posX > minLeft.m_x && posX < maxRight.m_x) //Check on X 
//	{
//		if(posY > minLeft.m_y && posY < maxRight.m_y) //Check on Y
//		{
//			return true; // return array of objects in quad 
//		}
//	}
//	return false; // return nothing
//}
//
//bool AABB::intersects(const AABB& bound) const
//{
//	if (bound.minRight.m_x <= minLeft.m_x && bound.maxRight.m_y <= maxLeft.m_y)
//		return false;
//	if (bound.minLeft.m_x <= minRight.m_x && bound.maxLeft.m_y <= maxRight.m_y)
//		return false;
//	if (bound.minLeft.m_y <= maxRight.m_y)
//		return false;
//	if (bound.maxRight.m_y <= minLeft.m_y)
//		return false;
//	return true;
//}
//
//Object::Object(const AABB& boundery, void* data)
//	:m_Objbound(boundery),m_data(data)
//{
//
//}
//
//void Object::setData(void* data)
//{
//	m_data = data;
//}
//
//void* Object::getData() const
//{
//	return m_data;
//}

//bool Quadtree::insert()
//{
//	m_Objects[];
//	return false;
//}

//bool Quadtree::remove(Object* object)
//{
//	if (object->m_qt == nullptr)
//	{
//		return false;
//	}
//	if (object->m_qt != this)
//	{
//		return object->m_qt->remove(object);
//	}
//
//	m_Objects.erase(std::find(m_Objects.begin(), m_Objects.end(), object));
//	object->m_qt = nullptr;
//	return true;
//}


//bool Quadtree::contains(Object* object) const
//{
//	if (object->m_qt == nullptr)
//	{
//		return false;
//	}
//	if (object->m_qt != this)
//	{
//		return object->m_qt->contains(object);
//	}
//	return std::find(m_Objects.begin(), m_Objects.end(), object) != m_Objects.end();
//}

//void Quadtree::search(const AABB& object, const std::function<void(Object*)>& callback)const
//{
//	if (!m_isLeaf)
//	{
//		if (Quadtree* child = getChild(object))
//		{
//			child->search(object, callback);
//		}
//		else
//		{
//			/*for ( auto && node : m_children)
//			{
//				if (node->m_bound.intersects(object))
//				{
//					node->search(object, callback);
//				}
//			}*/
//		}
//	}
//	//for (auto && node : m_Object)
//	//{
//	//	if (node->m_Objbound.intersects(object))
//	//	{
//	//		/*callback(object);*/
//	//	}
//	//}
//}

//Quadtree* Quadtree::getChild(const AABB &child)const
//{
//	bool bottom = child.bottom > this->m_bound.m_center.;
//	bool left = child.left < m_centerX;
//	bool top = !bottom && child.top < m_centerY;
//	if (left && child.right < m_centerX)
//	{
//		if (top)
//		{
//			return m_children[1]; //topLeft
//		}
//		if (bottom)
//		{
//			return m_children[2]; //bottomLeft
//		}
//	}
//	else if (!left)
//	{
//		if (top)
//		{
//			return m_children[0]; //topRight
//		}
//		if (bottom)
//		{
//			return m_children[3]; //BottomRight
//		}
//	}
//	return nullptr;
//}

//unsigned Quadtree::getTotalChildren() const
//{
//	unsigned count = 0;
//	if (!m_isLeaf)
//	{
//		for (auto && child: m_children)
//		{
//			count += child->getTotalChildren();
//		}
//	}
//	return (m_isLeaf? 0:4) + count;
//}
//
//unsigned Quadtree::getTotalObjects() const
//{
//	unsigned count = 0;
//	if (!m_isLeaf)
//	{
//		for (auto&& node : m_children)
//		{
//			count += node->getTotalObjects();
//		}
//	}
//	return m_Objects.size() + count;
//}