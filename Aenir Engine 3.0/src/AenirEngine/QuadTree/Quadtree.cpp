#include "PCH.h"
#include "Quadtree.h"

Quadtree::Quadtree()
{
	//m_topLeft = Point(0,0);
	//m_botRight = Point(0,0);
	m_pNode = NULL;
	m_northWest = NULL;
	m_northEast = NULL;
	m_southWest = NULL;
	m_southEast = NULL;
}

Quadtree::Quadtree(Point topL, Point botR)
{
	m_pNode = NULL;
	m_northWest = NULL;
	m_northEast = NULL;
	m_southWest = NULL;
	m_southEast = NULL;
	m_topLeft = topL;
	m_botRight = botR;
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

void Quadtree::InsertXYZ(XYZ xyz)
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
		if (Quadtree* child = getChild(object->m_bound))
		{
			return child->insert(object);
		}
	}
	m_Object.push_back(object);

	for (unsigned i = 0; i < m_Object.size(); i++)
	{
		Object* object = m_Object[i];
		if (Quadtree* child = getChild(object->m_bound))
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
	Quadtree* node = object->m_qt;
	if (node->m_parent == nullptr || object->m_bound.within(node->m_boundery))
	{
		return;
	}
	node->remove(object);
	do
	{
		node->m_parent;
		if (object->m_bound.within(node->m_boundery))
		{
			break;
		}
	} while (node->m_parent != nullptr);
	node->insert(object);
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

void Quadtree::search(const AABB& object, const std::function<void(Object*)>& callback) const
{
	if (!m_isLeaf)
	{
		if (Quadtree* child = getChild(object))
		{
			child->search(object, callback);
		}
		else
		{
			for ( auto && node : m_children)
			{
				if (node->m_boundery.intersects(object))
				{
					node->search(object, callback);
				}
			}
		}
	}
	for (auto && node : m_Object)
	{
		if (node->m_bound.intersects(object))
		{
			/*callback(object);*/
		}
	}
}


void Quadtree::subDivide()
{
	m_children[0] = new Quadtree(); //BotRight
	m_children[1] = new Quadtree(); //BotLeft
	m_children[2] = new Quadtree(); //TopLeft
	m_children[3] = new Quadtree(); //TopRight
}

Quadtree* Quadtree::getChild(const AABB &child)const
{
	bool bottom = child.bottom > m_centerY;
	bool left = child.left < m_centerX;
	bool top = !bottom && child.top < m_centerY;
	if (left && child.right < m_centerX)
	{
		if (top)
		{
			return m_children[1]; //topLeft
		}
		if (bottom)
		{
			return m_children[2]; //bottomLeft
		}
	}
	else if (!left)
	{
		if (top)
		{
			return m_children[0]; //topRight
		}
		if (bottom)
		{
			return m_children[3]; //BottomRight
		}
	}
	return nullptr;
}

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
Node* Quadtree::Search(Point point)
{
	return nullptr;
}
//check if current quad tree contains a point
bool Quadtree::InBoundery(Point point)
{
	return (point.m_x >= m_topLeft.m_x &&
		point.m_x <= m_botRight.m_x &&
		point.m_y >= m_topLeft.m_y &&
		point.m_y <= m_botRight.m_y);
}

AABB::AABB(const double& l, const double& t, const double& r, const double b)
	:left(l),top(t),right(r),bottom(b)
{
}

bool AABB::within(const AABB& bound) const
{
	return left > bound.left && bottom > bound.bottom &&
		right < bound.right&& top < bound.top;
}

bool AABB::intersects(const AABB& bound) const
{
	if (bound.right <= left)
		return false;
	if (bound.left <= right)
		return false;
	if (bound.top <= bottom)
		return false;
	if (bound.bottom <= top)
		return false;
	return true;
}
