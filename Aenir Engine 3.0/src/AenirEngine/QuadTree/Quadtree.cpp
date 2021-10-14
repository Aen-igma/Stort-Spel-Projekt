#include "PCH.h"
#include "Quadtree.h"

Quadtree::Quadtree()
{
	this->root = nullptr;
}

Quadtree::Quadtree(DirectX::BoundingBox &quad, const unsigned& level,
	const unsigned& maxLevel, const unsigned& capacity)
{
	this->root = new Node(quad, level, maxLevel, capacity);
	std::cout << "I'm root: \nLevel : " << level << std::endl << std::endl; //Säger hur långt det gick i levels
}

Quadtree::~Quadtree()
{
	delete this->root;
	for (auto& b : m_boundingVolStructs)
	{
		delete b;
	}
	/*for (auto& b : m_boundingVolumes)
	{
		delete b;
	}*/
}

void Quadtree::insertObject(int ID, DirectX::BoundingBox* obj)
{
	ObjeStruct *tempObj = new ObjeStruct(ID, obj);
	this->root->insert(tempObj); //Har en parent realtion med node - detaljerad beskriving senare 
	this->m_boundingVolStructs.push_back(tempObj);
	//this->m_boundingVolumes.push_back(obj);
}

Node* Quadtree::getRoot()
{
	return this->root;
}
