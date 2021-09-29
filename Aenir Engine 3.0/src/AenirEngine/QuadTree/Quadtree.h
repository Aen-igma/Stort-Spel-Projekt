#pragma once
#include "AenDefines.h"
#include "AenMath.h"
//Points where we mark on the 2D space. Used to hold details of a point
struct Point {
	int m_x;
	int m_y;
	//int m_z;
	Point(int x, int y) {
		m_x = x;
		m_y = y;
	}
	Point()
	{
		m_x = 0;
		m_y = 0;
	}
};

struct XY
{
	int m_x;
	int m_y;
	int m_z;
};

//Axis aligned bounding box with half dimension and center
struct AABB
{
	XY center;
	float halfDimension;
	/*AABB(XY center, float halfDimension);*/
	

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

//The objects that we wanted store in quadtree 
//Need to change it to so it takes in vertex data
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

//typedef struct vertex
//{
//	float x, y, z;
//
//}VERTEX;
//
//typedef struct Nodes
//{
//	int bType; //Tells us which type of node it is
//	VERTEX vBoundCoordinates[4];
//	unsigned int uiBranches[4];
//	unsigned int uiVertexStrip1[8];
//	unsigned int uiVertexStrip2[8];
//	unsigned int uiVertexStrip3[8];
//	unsigned int uiVertexStrip4[8];
//	unsigned int uiID;
//	unsigned int uiParentID;
//
//}NODES;



//MainClass for building quadtree
class Quadtree
{
public:
	Quadtree();
	Quadtree(Point topL, Point botR);
	~Quadtree();

	void Insert(Node* node);
	Node* Search(Point point);
	bool InBoundery(Point point);

	void Boundery(float x, float y, float z, float h){};

	void SubDivide() {};
private:

	//Boundery of Node
	Point m_topLeft;
	Point m_botRight;

	//contains the details of node
	Node* m_pNode;

	//Children of this quadtree
	Quadtree* m_ptopLeftTree;
	Quadtree* m_ptopRightTree;
	Quadtree* m_pbotLeftTree;
	Quadtree* m_pbotRightTree;

	

	int capacity;
	float m_x;
	float m_y;
	float m_z;
	float m_h;
	bool m_active;

};