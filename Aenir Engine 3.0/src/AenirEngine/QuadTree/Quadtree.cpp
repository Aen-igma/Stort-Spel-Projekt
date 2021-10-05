#include "PCH.h"
#include "Quadtree.h"

Quadtree::Quadtree()
{
	//m_topLeft = Point(0,0);
	//m_botRight = Point(0,0);
	//m_pNode = NULL;
	m_northWest = NULL;
	m_northEast = NULL;
	m_southWest = NULL;
	m_southEast = NULL;
}

Quadtree::Quadtree(const AABB& quad, const unsigned& capacity, const unsigned& maxLevel,
	const unsigned& level, Quadtree* parent)
	:m_capacity(capacity),
	m_maxLevel(maxLevel),
	m_level(level),
	m_parent(parent),
	m_quad(quad)
{
	//m_pNode = NULL;
	m_northWest = NULL;
	m_northEast = NULL;
	m_southWest = NULL;
	m_southEast = NULL;

}

Quadtree::~Quadtree()
{

}

////Insert Node into quadtree
//void Quadtree::Insert(Node* node)
//{
//	if (node == NULL)
//		return;
//	//Curren quad cannnot contain it
//	if (!InBoundery(node->m_pos))
//		return;
//	//We are at a quad of unit area
//	//We cannot subdivde this quad further
//	if (std::abs(m_topLeft.m_x - m_botRight.m_x ) <= 1 && 
//		std::abs(m_topLeft.m_y - m_botRight.m_y) <= 1)
//	{
//		if (m_pNode = NULL)
//		{
//			m_pNode = node;
//		}
//		return;
//	}
//	if ((m_topLeft.m_x + m_botRight.m_x) / 2 >= node->m_pos.m_x)
//	{
//		// Indicates topLeftTree
//		if ((m_topLeft.m_y + m_botRight.m_y) / 2 >= node->m_pos.m_y)
//		{
//			if (m_ptopLeftTree == NULL)
//				m_ptopLeftTree = new Quadtree(
//					Point(m_topLeft.m_x, m_topLeft.m_y),
//					Point((m_topLeft.m_x + m_botRight.m_x) / 2,
//						(m_topLeft.m_y + m_botRight.m_y) / 2));
//			m_ptopLeftTree->Insert(node);
//		}
//
//		// Indicates botLeftTree
//		else
//		{
//			if (m_pbotLeftTree == NULL)
//				m_pbotLeftTree = new Quadtree(
//					Point(m_topLeft.m_x,
//						(m_topLeft.m_y + m_botRight.m_y) / 2),
//					Point((m_topLeft.m_x + m_botRight.m_x) / 2,
//						m_botRight.m_y));
//			m_pbotLeftTree->Insert(node);
//		}
//	}
//	else
//	{
//		// Indicates topRightTree
//		if ((m_topLeft.m_y + m_botRight.m_y) / 2 >= node->m_pos.m_y)
//		{
//			if (m_ptopRightTree == NULL)
//				m_ptopRightTree = new Quadtree(
//					Point((m_topLeft.m_x + m_botRight.m_x) / 2,
//						m_topLeft.m_y),
//					Point(m_botRight.m_x,
//						(m_topLeft.m_y + m_botRight.m_y) / 2));
//			m_ptopRightTree->Insert(node);
//		}
//		// Indicates botRightTree
//		else
//		{
//			if (m_pbotRightTree == NULL)
//				m_pbotRightTree = new Quadtree(
//					Point((m_topLeft.m_x + m_botRight.m_x) / 2,
//						(m_topLeft.m_y + m_botRight.m_y) / 2),
//					Point(m_botRight.m_x, m_botRight.m_y));
//			m_pbotRightTree->Insert(node);
//		}
//	}
//}

void Quadtree::InsertXYZ(XY xy)
{

}


bool Quadtree::insert(Object* object)
{
	if (object->m_qt != nullptr)
	{
		return false;
	}
	if (!m_isLeaf)
	{
		if (Quadtree* child = getChild(object->m_Objbound))
		{
			return child->insert(object);
		}
	}
	m_Object.push_back(object);
	if (m_isLeaf && m_level < m_maxLevel && m_Object.size() >= m_capacity)
	{
		subdivide();
	}
	for (unsigned i = 0; i < m_Object.size(); i++)
	{
		Object* object = m_Object[i];
		if (Quadtree* child = getChild(object->m_Objbound))
		{
			m_Object.erase(m_Object.begin() + i);
			object->m_qt = nullptr;
			child->insert(object);
		}
		else i++;
	}
	return true;
}

bool Quadtree::remove(Object* object)
{
	if (object->m_qt == nullptr)
	{
		return false;
	}
	if (object->m_qt != this)
	{
		return object->m_qt->remove(object);
	}

	m_Object.erase(std::find(m_Object.begin(), m_Object.end(), object));
	object->m_qt = nullptr;
	return true;
}

void Quadtree::update(Object* object)
{
	//Quadtree* node = object->m_qt;
	//if (node->m_parent == nullptr || object->m_Objbound.within(node->m_bound))
	//{
	//	return;
	//}
	//node->remove(object);
	//do
	//{
	//	node->m_parent;
	//	if (object->m_Objbound.within(node->m_bound))
	//	{
	//		break;
	//	}
	//} while (node->m_parent != nullptr);
	//node->insert(object);
}

bool Quadtree::contains(Object* object) const
{
	if (object->m_qt == nullptr)
	{
		return false;
	}
	if (object->m_qt != this)
	{
		return object->m_qt->contains(object);
	}
	return std::find(m_Object.begin(), m_Object.end(), object) != m_Object.end();
}

void Quadtree::search(const AABB& object, const std::function<void(Object*)>& callback)const
{
	if (!m_isLeaf)
	{
		if (Quadtree* child = getChild(object))
		{
			child->search(object, callback);
		}
		else
		{
			/*for ( auto && node : m_children)
			{
				if (node->m_bound.intersects(object))
				{
					node->search(object, callback);
				}
			}*/
		}
	}
	//for (auto && node : m_Object)
	//{
	//	if (node->m_Objbound.intersects(object))
	//	{
	//		/*callback(object);*/
	//	}
	//}
}


void Quadtree::subdivide()
{
	for(int i = 0; i < 4; i++)
		m_children[i] = new Quadtree(this->m_quad, m_capacity, m_maxLevel, m_level + 1, this); 


	//m_children[0] = new Quadtree({ m_centerX, m_centerY, m_bound.right ,m_bound.bottom }, m_capacity, m_maxLevel, m_level +1, this); //BotRight
	//m_children[1] = new Quadtree({ m_bound.left, m_centerY, m_centerX ,m_bound.bottom }, m_capacity, m_maxLevel, m_level + 1, this); //BotLeft
	//m_children[2] = new Quadtree({ m_bound.left, m_bound.top, m_centerX ,m_centerY }, m_capacity, m_maxLevel, m_level + 1, this); //TopLeft
	//m_children[3] = new Quadtree({ m_centerX, m_bound.top, m_bound.right ,m_centerY }, m_capacity, m_maxLevel, m_level + 1, this); //TopRight
	m_isLeaf = false;
}

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

unsigned Quadtree::getTotalChildren() const
{
	unsigned count = 0;
	if (!m_isLeaf)
	{
		for (auto && child: m_children)
		{
			count += child->getTotalChildren();
		}
	}
	return (m_isLeaf? 0:4) + count;
}

unsigned Quadtree::getTotalObjects() const
{
	unsigned count = 0;
	if (!m_isLeaf)
	{
		for (auto&& node : m_children)
		{
			count += node->getTotalObjects();
		}
	}
	return m_Object.size() + count;
}

//Node* Quadtree::Search(Point point)
//{
//	//Current quad cannot contain it
//	if (!InBoundery(point))
//		return NULL;
//	//We are at a quad of unit area
//	//We cannot subdivde this quad further
//	if (m_pNode != NULL)
//		return m_pNode;
//	if ((m_topLeft.m_x + m_botRight.m_x) / 2 >= point.m_x)
//	{
//		// Indicates topLeftTree
//		if ((m_topLeft.m_y + m_botRight.m_y) / 2 >= point.m_y)
//		{
//			if (m_ptopLeftTree == NULL)
//				return NULL;
//			return m_ptopLeftTree->Search(point);
//		}
//		// Indicates botLeftTree
//		else
//		{
//			if (m_pbotLeftTree == NULL)
//				return NULL;
//			return m_pbotLeftTree->Search(point);
//		}
//	}
//	else
//	{
//		// Indicates topRightTree
//		if ((m_topLeft.m_y + m_botRight.m_y) / 2 >= point.m_y)
//		{
//			if (m_ptopRightTree == NULL)
//				return NULL;
//			return m_ptopRightTree->Search(point);
//		}
//		// Indicates botRightTree
//		else
//		{
//			if (m_pbotRightTree == NULL)
//				return NULL;
//			return m_pbotRightTree->Search(point);
//		}
//	}
//}

void Quadtree::clear()
{
	if (!m_Object.empty())
	{
		for (auto && obj : m_Object)
		{
			remove(obj);
		}
		m_Object.clear();
	}
	if (! m_isLeaf)
	{
		for (int i = 0; i < 4; i++)
		{
			delete m_children[i];
		}
		m_isLeaf = true;
	}
}

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

XY::XY(int x, int y)
	:m_x(x), m_y(y)
{

}

AABB::AABB()
{
}

AABB::AABB(const XY& min, const XY& max)
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

bool AABB::within() const
{
	/*
	player pos
	if player pos is inside m_bound
	return reference to array of objects inside this quad
	*/
	




	/*return minLeft > bound.minLeft && bottom > bound.bottom &&
		right < bound.right&& top < bound.top;*/
	// Kolla om ett objekt ligger inom denna AABB
	return false;
}

//bool AABB::intersects(const AABB& bound) const
//{
//	if (bound.right <= left)
//		return false;
//	if (bound.left <= right)
//		return false;
//	if (bound.top <= bottom)
//		return false;
//	if (bound.bottom <= top)
//		return false;
//	return true;
//}

Object::Object(const AABB& boundery, void* data)
	:m_Objbound(boundery),m_data(data)
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


