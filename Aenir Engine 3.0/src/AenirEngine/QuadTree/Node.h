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

struct NodeStruct
{
	int m_ID;
	int m_RenderLayer;
	DirectX::BoundingBox mp_boundBox;


	NodeStruct();
	NodeStruct(int ID, int RenderLayer, DirectX::BoundingBox boundingBox);
	~NodeStruct();

};

struct QuadOutput
{
	int m_ID;
	uint32_t m_RenderLayer;

	QuadOutput();
	QuadOutput(int ID, uint32_t Layer);
	~QuadOutput();
};


class Node
{
public:
	Node();
	Node(DirectX::BoundingBox& quad, const unsigned& level = 0,
		const unsigned& max_level = 1, const unsigned& capacity = 3);
	~Node();
	void Insert(NodeStruct* obj);
	bool Inside(DirectX::BoundingBox& playerBox); //not needed
	void IntersectTest(const DirectX::BoundingFrustum &other, std::vector<QuadOutput*>& output); //Output = Id for objects
	//Problem kan vara att flera noder kan uppstå, behövs fixas senare.

	void SmartPointer(std::shared_ptr<NodeStruct> ptr);
private:
	Node* mp_children[4] = { nullptr };
	DirectX::BoundingBox m_DirectXAABB;
	std::vector<NodeStruct*> m_Objs;

	unsigned m_level;
	unsigned m_maxLevel;
	unsigned m_capacity;

	void Subdivide();
};