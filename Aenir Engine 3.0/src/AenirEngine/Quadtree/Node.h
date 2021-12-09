#pragma once
#include "AenDefines.h"
#include "BuritoMath.h"
#include <algorithm>
#include <vector>
#include <functional>
#include <DirectXMath.h>
#include <DirectXCollision.h>
#include "Graphics\Component\Drawable\Drawable.h"
//#include <memory>
//#include <mutex>

struct NodeStruct
{
	size_t m_ID;
	uint32_t m_renderLayer;
	DirectX::BoundingBox m_boundBox;
	Aen::Drawable* mp_drawable;


	NodeStruct();
	NodeStruct(size_t ID, uint32_t RenderLayer, DirectX::BoundingBox box, Aen::Drawable* drawable);
	~NodeStruct();

};

//struct QuadOutput
//{
//	size_t m_ID;
//	uint32_t m_renderLayer;
//
//	QuadOutput();
//	QuadOutput(size_t ID, uint32_t Layer);
//	~QuadOutput();
//};


class Node
{
public:
	Node();
	Node(DirectX::BoundingBox& quad, const unsigned& level = 0,
		const unsigned& max_level = 1, const unsigned& capacity = 3);
	~Node();
	void Insert(NodeStruct* obj);
	void FrustumTest(const DirectX::BoundingFrustum &other, std::vector<NodeStruct>& output);
	void PositionTest(std::vector<NodeStruct>& output);

	//bool Inside(DirectX::BoundingBox& playerBox); //not needed
	//Problem kan vara att flera noder kan uppstå, behövs fixas senare.

	//void SmartPointer(std::shared_ptr<NodeStruct> ptr);
private:
	Node* mp_children[4] = { nullptr };
	DirectX::BoundingBox m_areaQuad;
	std::vector<NodeStruct*> m_objs;
	//QuadOutput m_tempQuadObj;

	unsigned m_level;
	unsigned m_maxLevel;
	unsigned m_capacity;

	void Subdivide();
};