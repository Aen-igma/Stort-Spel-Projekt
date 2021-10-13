#pragma once
#include "AenDefines.h"
#include "AenMath.h"
#include <algorithm>
#include <vector>
#include <functional>
#include <DirectXMath.h>
#include <DirectXCollision.h>

//Axis aligned bounding box with half dimension and center
struct AABB
{
	Aen::Vec2f minLeft, minRight, maxLeft, maxRight; // top left, top right, bottom left, bottom right
	Aen::Vec2f m_center;

	AABB();
	AABB(const Aen::Vec2f& min, const Aen::Vec2f& max);
	AABB(const double& minX, const double& minY, const double& maxX, const double& maxY);
	bool within(const int& posX, const int& posY) const;
	//bool intersects(const AABB& bound)const;
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
	Node(AABB& quad, const unsigned& level = 0, 
		const unsigned& max_level = 1, const unsigned& capacity = 3);
	Node(DirectX::BoundingBox& quad, const unsigned& level = 0,
		const unsigned& max_level = 1, const unsigned& capacity = 3);
	~Node();
	
	void insert(DirectX::BoundingBox* obj);
	void clear();

private:
	Node* m_children[4] = { nullptr };
	AABB* m_NodeAABB;
	DirectX::BoundingBox m_DirectXAABB;
	std::vector<DirectX::BoundingBox*> m_Objects;
	unsigned m_level;
	unsigned m_maxLevel;
	unsigned m_capacity;

	void subdivide();
}