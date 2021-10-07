#pragma once
#include "AenDefines.h"
#include "AenMath.h"
#include <algorithm>
#include <vector>
#include <functional>


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
};

class Node
{
public:
	Node();
	Node(AABB& quad, Object* object = nullptr, const unsigned& level = 0, 
		const unsigned& max_level = 1, const unsigned& capacity = 3);
	Node(const Aen::Vec2f& min, const Aen::Vec2f& max, Object* object = nullptr, 
		const unsigned& level = 0, const unsigned& max_level = 1, const unsigned& capacity = 3);
	~Node();
	
	void initialize();
	void insert();
	void addNode();
	void clear();

private:
	Node* m_children[4] = { nullptr };
	AABB* m_NodeAABB = { nullptr };
	std::vector<Object*> m_Objects;
	std::vector<Node*> m_Node;
	unsigned m_level;
	unsigned m_maxLevel;
	unsigned m_capacity;

	void subdivide();
}