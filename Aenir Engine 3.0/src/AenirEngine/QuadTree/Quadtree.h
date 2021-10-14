#pragma once
#include "Node.h"


//MainClass for building quadtree
class Quadtree
{
public:
	Quadtree();
	Quadtree(DirectX::BoundingBox &quad, const unsigned& level = 0,
		const unsigned& maxLevel = 1, const unsigned& capacity = 3);
	~Quadtree();
	void insertObject(int ID, DirectX::BoundingBox* obj);
	Node* getRoot();
private:
	Node* root;
	std::vector<DirectX::BoundingBox*> m_boundingVolumes;
	std::vector<ObjeStruct*> m_boundingVolStructs;
};