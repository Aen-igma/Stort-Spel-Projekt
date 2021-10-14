#pragma once
#include "AenDefines.h"
#include "AenMath.h"
#include <algorithm>
#include <vector>
#include <functional>
#include <DirectXMath.h>
#include <DirectXCollision.h>


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
	void insert(ObjeStruct* obj);
	bool inside(DirectX::BoundingBox& playerBox);
	// Builds vector that is view frustrum culled
	void intersectTest(const DirectX::BoundingFrustum &other, std::vector<int>& output); //Output = Id for objects
	//Problem kan vara att flera noder kan uppstå, behövs fixas senare.
private:
	Node* m_children[4] = { nullptr };
	DirectX::BoundingBox m_DirectXAABB;
	std::vector<DirectX::BoundingBox*> m_Objects;
	std::vector<ObjeStruct*> m_Objs;
	unsigned m_level;
	unsigned m_maxLevel;
	unsigned m_capacity;
	void subdivide();
};