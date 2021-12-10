#pragma once
#include "BuritoMath.h"
#include <DirectXMath.h>
#include <DirectXCollision.h>
#include "Graphics\Component\Drawable\Drawable.h"
#include "Graphics\Component\Entity.h"
#include "Graphics\Component\EntityHandler.h"

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

class Node
{
public:
	Node();
	Node(DirectX::BoundingBox& quad, const unsigned& level = 0,
		const unsigned& max_level = 1, const unsigned& capacity = 3);
	~Node();
	void Insert(const NodeStruct& obj);
	void FrustumTest(const DirectX::BoundingFrustum &other, std::vector<NodeStruct>& output);

	//bool Inside(DirectX::BoundingBox& playerBox); //not needed
	//Problem kan vara att flera noder kan uppstå, behövs fixas senare.
	//void SmartPointer(std::shared_ptr<NodeStruct> ptr);
private:
	Node* mp_children[4] = { nullptr };
	DirectX::BoundingBox m_areaQuad;
	Aen::Entity* mp_aabbDraw;
	std::vector<NodeStruct> m_objs;

	unsigned m_level;
	unsigned m_maxLevel;
	unsigned m_capacity;

	void Subdivide();
};