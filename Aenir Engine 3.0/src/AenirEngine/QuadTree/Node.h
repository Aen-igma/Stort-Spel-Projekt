#pragma once
#include "AenDefines.h"
#include "BuritoMath.h"
#include <algorithm>
#include <vector>
#include <functional>
#include <DirectXMath.h>
#include <DirectXCollision.h>
#include <memory>
#include <mutex>

struct ObjeStruct
{
	int m_ID;
	DirectX::BoundingBox* mp_boundBox;

	ObjeStruct();
	ObjeStruct(int ID, DirectX::BoundingBox* boundingBox);
	~ObjeStruct();

};

class Node
{
public:
	Node();
	Node(DirectX::BoundingBox& quad, const unsigned& level = 0,
		const unsigned& max_level = 1, const unsigned& capacity = 3);
	~Node();
	void Insert(ObjeStruct* obj);
	bool Inside(DirectX::BoundingBox& playerBox); //not needed
	void IntersectTest(const DirectX::BoundingFrustum &other, std::vector<int>& output); //Output = Id for objects
	//Problem kan vara att flera noder kan uppstå, behövs fixas senare.

	void SmartPointer(std::shared_ptr<ObjeStruct> ptr);
private:
	Node* mp_children[4] = { nullptr };
	DirectX::BoundingBox m_DirectXAABB;
	std::vector<ObjeStruct*> m_Objs;
	//std::vector<DirectX::BoundingBox*> m_Objects;

	unsigned m_level;
	unsigned m_maxLevel;
	unsigned m_capacity;

	void Subdivide();
};