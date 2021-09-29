#include "PCH.h"
#include "Quadtree.h"

Quadtree::Quadtree()
{
	m_topLeft = Point(0,0);
	m_botRight = Point(0,0);
	m_pNode = NULL;
	m_ptopLeftTree = NULL;
	m_ptopRightTree = NULL;
	m_pbotLeftTree = NULL;
	m_pbotLeftTree = NULL;
}

Quadtree::Quadtree(Point topL, Point botR)
{
	m_pNode = NULL;
	m_ptopLeftTree = NULL;
	m_ptopRightTree = NULL;
	m_pbotLeftTree = NULL;
	m_pbotLeftTree = NULL;
	m_topLeft = topL;
	m_botRight = botR;
}

Quadtree::~Quadtree()
{
}

//Insert Node into quadtree
void Quadtree::Insert(Node* node)
{
	if (node == NULL)
		return;
	//Curren quad cannnot contain it
	if (!InBoundery(node->m_pos))
		return;
	//We are at a quad of unit area
	//We cannot subdivde this quad further
	if (std::abs(m_topLeft.m_x - m_botRight.m_x ) <= 1 && 
		std::abs(m_topLeft.m_y - m_botRight.m_y) <= 1)
	{
		if (m_pNode = NULL)
		{
			m_pNode = node;
		}
		return;
	}
	if ((m_topLeft.m_x + m_botRight.m_x) / 2 >= node->m_pos.m_x)
	{
		// Indicates topLeftTree
		if ((m_topLeft.m_y + m_botRight.m_y) / 2 >= node->m_pos.m_y)
		{
			if (m_ptopLeftTree == NULL)
				m_ptopLeftTree = new Quadtree(
					Point(m_topLeft.m_x, m_topLeft.m_y),
					Point((m_topLeft.m_x + m_botRight.m_x) / 2,
						(m_topLeft.m_y + m_botRight.m_y) / 2));
			m_ptopLeftTree->Insert(node);
		}

		// Indicates botLeftTree
		else
		{
			if (m_pbotLeftTree == NULL)
				m_pbotLeftTree = new Quadtree(
					Point(m_topLeft.m_x,
						(m_topLeft.m_y + m_botRight.m_y) / 2),
					Point((m_topLeft.m_x + m_botRight.m_x) / 2,
						m_botRight.m_y));
			m_pbotLeftTree->Insert(node);
		}
	}
	else
	{
		// Indicates topRightTree
		if ((m_topLeft.m_y + m_botRight.m_y) / 2 >= node->m_pos.m_y)
		{
			if (m_ptopRightTree == NULL)
				m_ptopRightTree = new Quadtree(
					Point((m_topLeft.m_x + m_botRight.m_x) / 2,
						m_topLeft.m_y),
					Point(m_botRight.m_x,
						(m_topLeft.m_y + m_botRight.m_y) / 2));
			m_ptopRightTree->Insert(node);
		}
		// Indicates botRightTree
		else
		{
			if (m_pbotRightTree == NULL)
				m_pbotRightTree = new Quadtree(
					Point((m_topLeft.m_x + m_botRight.m_x) / 2,
						(m_topLeft.m_y + m_botRight.m_y) / 2),
					Point(m_botRight.m_x, m_botRight.m_y));
			m_pbotRightTree->Insert(node);
		}
	}
}

Node* Quadtree::Search(Point point)
{
	//Current quad cannot contain it
	if (!InBoundery(point))
		return NULL;
	//We are at a quad of unit area
	//We cannot subdivde this quad further
	if (m_pNode != NULL)
		return m_pNode;
	if ((m_topLeft.m_x + m_botRight.m_x) / 2 >= point.m_x)
	{
		// Indicates topLeftTree
		if ((m_topLeft.m_y + m_botRight.m_y) / 2 >= point.m_y)
		{
			if (m_ptopLeftTree == NULL)
				return NULL;
			return m_ptopLeftTree->Search(point);
		}
		// Indicates botLeftTree
		else
		{
			if (m_pbotLeftTree == NULL)
				return NULL;
			return m_pbotLeftTree->Search(point);
		}
	}
	else
	{
		// Indicates topRightTree
		if ((m_topLeft.m_y + m_botRight.m_y) / 2 >= point.m_y)
		{
			if (m_ptopRightTree == NULL)
				return NULL;
			return m_ptopRightTree->Search(point);
		}
		// Indicates botRightTree
		else
		{
			if (m_pbotRightTree == NULL)
				return NULL;
			return m_pbotRightTree->Search(point);
		}
	}
}
//check if current quad tree contains a point
bool Quadtree::InBoundery(Point point)
{
	return (point.m_x >= m_topLeft.m_x &&
		point.m_x <= m_botRight.m_x &&
		point.m_y >= m_topLeft.m_y &&
		point.m_y <= m_botRight.m_y);
}


