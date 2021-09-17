#pragma once
#include "AenWindow.h"
#include "AenGraphics.h"
#include "AenDefines.h"
#include "Graphics/Graphics.h"

//Points where we mark on the 2D space
struct Point {
	float m_x;
	float m_y;
	Point(float x, float y) {
		this->m_x = x;
		this->m_y = y;
	}
	Point()
	{
		m_x = 0;
		m_y = 0;
	}
};

struct XY
{
	float m_x;
	float m_y;
};

//Axis aligned bounding box with hald dimension and center
struct AABB
{

	XY center;
	float halfDimension;

};

//To construct the boundery box, needs to use the window x,y for right cordinates
struct Rectangle
{
	float m_x;
	float m_y;
	float m_z;
	float m_h;
	Rectangle(float  x, float y, float z, float h) {
		this->m_x = x;
		this->m_y = y;
		this->m_z = z;
		this->m_h = h;
	}

};

//The objects that we wanted stord in quadtree
struct Node
{
	Point m_pos;
	int m_data;
	Node(Point pos, int data)
	{
		m_pos = pos;
		m_data = data;
	}
	Node()
	{
		m_data = 0;
	}

};

//MainClass for building quadtree
class Quadtree
{
public:
	Quadtree();
	Quadtree(Point topL, Point botR);
	~Quadtree();

	void insert(Node* node);
	Node* search(Point point);
	bool inBoundery(Point point);

	void boundery(float x, float y, float z, float h){};
	void subDivide() {};
private:

	Point m_topLeft;
	Point m_botRight;

	Node* pnode;

	Quadtree *m_ptopLeftTree;
	Quadtree *m_ptopRightTree;
	Quadtree* m_pbotLeftTree;
	Quadtree* m_pbotRightTree;

	int capacity;
	float m_x;
	float m_y;
	float m_z;
	float m_h;
	bool m_active;

};