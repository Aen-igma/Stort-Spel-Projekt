#pragma once
#include "AenDefines.h"
#include "AenMath.h"
#include <algorithm>
#include <vector>
#include <functional>

//Points where we mark on the 2D space. Used to hold details of a point
struct Point {
	int m_x;
	int m_y;
	int m_z;
	//int m_z;
	Point(int x, int y, int z) {
		m_x = x;
		m_y = y;
		m_z = z;
	}
	Point()
	{
		m_x = 0;
		m_y = 0;
		m_z = 0;
	}
};

struct XYZ
{
	int m_x;
	int m_y;
	int m_z;
};

//Axis aligned bounding box with half dimension and center
struct AABB
{
	double left, top, right, bottom;

	AABB(){}
	AABB(const double &l, const double &t, const double &r, const double b);
	bool within(const AABB& bound) const;
	bool intersects(const AABB& bound)const;

	XYZ m_center;
	float m_halfDimension;
	AABB(XYZ center, float halfDimension)
	{
		m_center = center;
		m_halfDimension = halfDimension;
	}
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
	XYZ m_xyz;
	int m_data;
	Node(XYZ xyz, int data)
	{
		m_xyz = xyz;
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

//New
struct Object
{
	friend class Quadtree;
public:
	AABB _bound;
	Object() { };
	Object(const AABB&, void* data = nullptr);
	void setData(void* data);
	void* getData() const;
private:
	void* _data = nullptr;
	Quadtree* _qt = nullptr;
};

//MainClass for building quadtree
class Quadtree
{
public:
	Quadtree();
	Quadtree(Point topL, Point botR);
	~Quadtree();

	//void Insert(Node* node);


	//New shit
	void InsertXYZ(XYZ xyz);
	bool insert(Object* object);
	bool remove(Object* object);
	void update(Object* object);
	bool contains(Object* object)const;
	void search(const AABB& object, const std::function<void(Object*)> &callback)const;
	void subDivide();


	unsigned getTotalChildren() const;
	unsigned getTotalObjects() const;
	void clear();



	Node* Search(Point point);
	bool InBoundery(Point point);
	void Boundery(float x, float y, float z, float h){};

private:

	//Boundery of Node
	Point m_topLeft;
	Point m_botRight;

	//contains the details of node
	Node* m_pNode;


	//New
	const int QT_NODE_CAPACITY = 4;
	AABB m_boundery;
	Quadtree* m_parent;
	Quadtree* m_children[4];
	bool m_isLeaf = true;
	double m_centerX;
	double m_centerY;
	unsigned m_level;
	unsigned m_capacity;
	std::vector<Object*> m_Object;
	void subDivide();
	Quadtree* getChild(const AABB& child) const;


	//Children of this quadtree
	Quadtree* m_northEast;
	Quadtree* m_northWest;
	Quadtree* m_southWest;
	Quadtree* m_southEast;

	

	int capacity;
	float m_x;
	float m_y;
	float m_z;
	float m_h;
	bool m_active;

};