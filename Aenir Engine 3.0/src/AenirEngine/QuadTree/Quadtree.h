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

struct XY
{
	int m_x;
	int m_y;
	XY(int x = 0, int y = 0);

};

//Axis aligned bounding box with half dimension and center
struct AABB
{
	XY minLeft, minRight, maxLeft, maxRight;
	XY m_center;

	AABB();
	AABB(const XY& min, const XY& max);
	AABB(const double& minX, const double& minY, const double& maxX, const double& maxY);
	bool within() const; // skicka in spelarens position och kolla om spelaren befiner sig i denna delen av quadtree
	//bool intersects(const AABB& bound)const;

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
//struct Node
//{
//	XYZ m_xyz;
//	int m_data;
//	Node(XYZ xyz, int data)
//	{
//		m_xyz = xyz;
//		m_data = data;
//	}
//	Node()
//	{
//		m_data = 0;
//	}
//};

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

//New object class. Meant to handle data of object
struct Object
{
	friend class Quadtree;
public:
	AABB m_Objbound;
	Object() { };
	Object(const AABB&, void* data = nullptr);
	void setData(void* data);
	void* getData() const;
private:
	void* m_data = nullptr;
	Quadtree* m_qt = nullptr;
};

//MainClass for building quadtree
class Quadtree
{
public:
	Quadtree();
	Quadtree(const AABB& quad, const unsigned& capacity, const unsigned& maxLevel,
		const unsigned& level = 0, Quadtree* parent = nullptr);
	~Quadtree();

	//void Insert(Node* node);


	//New shit
	void InsertXYZ(XY xy);
	bool insert(Object* object);
	bool remove(Object* object);
	void update(Object* object);
	bool contains(Object* object)const;
	void search(const AABB& object, const std::function<void(Object*)> &callback)const;
	unsigned getTotalChildren() const;
	unsigned getTotalObjects() const;
	void clear();


	//Old shit
	/*Node* Search(Point point);
	bool InBoundery(Point point);
	void Boundery(float x, float y, float z, float h){};*/

private:

	//Boundery of Node
	/*Point m_topLeft;
	Point m_botRight;*/

	//contains the details of node
	//Node* m_pNode;


	//New
	const int QT_NODE_CAPACITY = 4;
	AABB m_quad;
	Quadtree* m_parent;
	Quadtree* m_children[4];
	bool m_isLeaf = true;
	//double m_centerX;
	//double m_centerY;
	unsigned m_level;
	unsigned m_maxLevel;
	unsigned m_capacity;
	std::vector<Object*> m_Object;
	void subdivide();

	//Quadtree* getChild(const AABB&) const;

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