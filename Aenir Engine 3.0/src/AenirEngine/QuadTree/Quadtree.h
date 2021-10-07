#pragma once
#include "Node.h"



//MainClass for building quadtree
class Quadtree
{
public:
	Quadtree();
	//Quadtree(const Aen::Vec2f& min, const Aen::Vec2f& max, const unsigned& capacity = 3,
	//	const unsigned& maxLevel = 1, const unsigned& level = 0);
	~Quadtree();
	
	void initNode(const Aen::Vec2f& min, const Aen::Vec2f& max, const unsigned& capacity = 3,
		const unsigned& maxLevel = 1, const unsigned& level = 0);

	void initializeTree();
	Object* findActiveQuad(const int& posX, const int& posY);

	//void update(Object* object);
	//void clear();

private:
	Node* root;
	//New
	//AABB m_quad;
	//Quadtree* m_children[4];
	//Object* m_Objects; //Array of objects that we use
	//unsigned m_nrOf;
	//unsigned m_level;
	//unsigned m_maxLevel;
	//unsigned m_capacity;
	//bool m_isLeaf;
	//Old code
	//void subdivide();
};

/*//Old shit
	Node* Search(Point point);
	bool InBoundery(Point point);
	void Boundery(float x, float y, float z, float h){};
	bool contains(Object* object)const;
    void search(const AABB& object, const std::function<void(Object*)> &callback)const;
	unsigned getTotalChildren() const;
	unsigned getTotalObjects() const;
	bool insert();
	bool remove(Object* object);
//New old shit
	void InsertXYZ(XY xy);
	bool insert(Object* object);
*/