#pragma once
#include "AenDefines.h"
#include "AenMath.h"
#include <algorithm>
#include <vector>
#include <functional>

//Points where we mark on the 2D space. Used to hold details of a point
//struct Point {
//	int m_x;
//	int m_y;
//	int m_z;
//	//int m_z;
//	Point(int x, int y, int z) {
//		m_x = x;
//		m_y = y;
//		m_z = z;
//	}
//	Point()
//	{
//		m_x = 0;
//		m_y = 0;
//		m_z = 0;
//	}
//};

struct XY
{
	int m_x;
	int m_y;
	XY(int x = 0, int y = 0);
};

//Axis aligned bounding box with half dimension and center
struct AABB
{
	XY minLeft, minRight, maxLeft, maxRight; // top left, top right, bottom left, bottom right
	XY m_center;

	AABB();
	AABB(const XY& min, const XY& max);
	AABB(const double& minX, const double& minY, const double& maxX, const double& maxY);
	bool within(const int& posX, const int& posY) const;
	bool intersects(const AABB& bound)const;
};


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
		const unsigned& level = 0, Quadtree* parent = nullptr, Object* objects = nullptr, const unsigned& nrOfElements = 0);
	~Quadtree();

	//New shit
	//void InsertXYZ(XY xy);
	//bool insert(Object* object);
	void initializeTree();
	Object* findActiveQuad(const int& posX, const int& posY);
	//bool insert();
	//bool remove(Object* object);
	void update(Object* object);
	//bool contains(Object* object)const;
	//void search(const AABB& object, const std::function<void(Object*)> &callback)const;
	//unsigned getTotalChildren() const;
	//unsigned getTotalObjects() const;
	void clear();


	//Old shit
	/*Node* Search(Point point);
	bool InBoundery(Point point);
	void Boundery(float x, float y, float z, float h){};*/

private:


	//New
	AABB m_quad;
	Quadtree* m_parent;
	Quadtree* m_children[4];
	Object* m_Objects; //Array of objects that we use
	unsigned m_nrOf;
	unsigned m_level;
	unsigned m_maxLevel;
	unsigned m_capacity;
	bool m_isLeaf;
	//std::vector<Object*> m_Objects; //Old code
	void subdivide();

};