#include "PCH.h"
#include "Quadtree.h"

Quadtree::Quadtree()
{
	m_topLeft = Point(0,0);
	m_topRight = Point(0, 0);
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
	m_topRight = botR;
}

Quadtree::~Quadtree()
{
}

//Node into quadtree
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
}

Node* Quadtree::search(Point point)
{
	return nullptr;
}

bool Quadtree::inBoundery(Point)
{
	return false;
}
