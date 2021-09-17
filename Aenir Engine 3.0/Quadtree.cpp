#include "PCH.h"
#include "Quadtree.h"

Quadtree::Quadtree()
{
	m_topLeft = Point(0,0);
	m_botRight = Point(0, 0);
	pnode = NULL;
	m_ptopLeftTree = NULL;
	m_ptopRightTree = NULL;
	m_pbotLeftTree = NULL;
	m_pbotLeftTree = NULL;
}

Quadtree::Quadtree(Point topL, Point botR)
{
	pnode = NULL;
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
void Quadtree::insert(Node* node)
{
	if (node == NULL)
	{
		return;
	}
	//quad cant contain 
	if (!inBoundery(node->m_pos))
	{
		return;
	}
	
	if (abs(m_topLeft.m_x - m_botRight.m_y ) <= 1 && 
		abs(m_topLeft.m_y - m_botRight.m_y) <= 1)
	{
		if (pnode = NULL)
		{
			pnode = node;
		}
		return;
	}
	if ((m_topLeft.m_x + m_botRight.m_x) / 2 >= node->m_pos.m_x)
	{
		//Indicates topLeftTree
		if ((m_topLeft.m_y + m_botRight.m_y) / 2 >= node->m_pos.m_y)
		{
			if (m_ptopLeftTree == NULL)
			{
				m_ptopLeftTree = new Quadtree(Point(m_topLeft.m_x, m_botRight.m_y),
					Point((m_topLeft.m_x, m_botRight.m_y) /2, (m_topLeft.m_y + m_botRight.m_y) / 2));
				m_ptopLeftTree->insert(node);
			}
		}
		//BotLeftTree
		else
		{
			if (m_pbotLeftTree == NULL)
			{

			}
		}

	}
	else
	{
		if ((m_topLeft.m_y + m_botRight.m_y) / 2 >= node->m_pos.m_y)
		{
			if (m_ptopRightTree == NULL)
			{
				m_ptopRightTree = new Quadtree(
					Point((m_topLeft.m_x + m_botRight.m_x) / 2,
						(m_topLeft.m_y) / 2),
					Point(m_botRight.m_x, m_botRight.m_y));

			}
			m_ptopRightTree->insert(node);
		}
		else
		{
			if (m_pbotRightTree == NULL)
			{
				m_pbotRightTree = new Quadtree(
					Point((m_topLeft.m_x + m_botRight.m_x) / 2,
						(m_topLeft.m_y + m_botRight.m_y) / 2),
					Point(m_botRight.m_x, m_botRight.m_y));		
			}
			m_pbotRightTree->insert(node);
		}
	}
}

Node* Quadtree::search(Point point)
{
	if (!inBoundery(point))
	{
		return NULL;
	}
	if (pnode != NULL)
	{
		return pnode;
	}
	if ((m_topLeft.m_x + m_botRight.m_x )/ 2 >= point.m_x)
	{
		if ((m_topLeft.m_y + m_botRight.m_y) / 2 >= point.m_y)
		{
			if (m_ptopLeftTree == NULL)
			{
				return NULL;
			}
			return m_ptopLeftTree->search(point);
		}
		else
		{
			if (m_pbotLeftTree == NULL)
			{
				return NULL;
			}
			return m_pbotLeftTree->search(point);
		}
	}
	else
	{
		if ((m_topLeft.m_y + m_botRight.m_y) / 2 >= point.m_y)
		{
			if (m_ptopRightTree == NULL)
			{
				return NULL;
			}
			return m_ptopRightTree->search(point);
		}
		else
		{
			if (m_pbotRightTree == NULL)
			{
				return NULL;
			}
			return m_ptopRightTree->search(point);
		}
		
	}
}

bool Quadtree::inBoundery(Point point)
{
	return (point.m_x >= m_topLeft.m_x &&
		point.m_x <= m_botRight.m_x &&
		point.m_y >= m_topLeft.m_y &&
		point.m_y <= m_botRight.m_y);

}
