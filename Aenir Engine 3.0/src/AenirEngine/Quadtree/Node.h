#pragma once
#include "BuritoMath.h"
#include "Graphics\Component\EntityHandler.h"

struct NodeStruct
{
	size_t m_ID;
	uint32_t m_renderLayer;
	DirectX::BoundingBox m_boundBox;
	Aen::Drawable* mp_drawable;

	NodeStruct(size_t ID = 0, uint32_t RenderLayer = 0, 
		DirectX::BoundingBox box = DirectX::BoundingBox(), Aen::Drawable* drawable = nullptr);
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
private:
	Node* mp_children[4] = { nullptr };
	DirectX::BoundingBox m_areaQuad;
	Aen::Entity* mp_aabbDraw;
	std::vector<NodeStruct> m_objs;

	unsigned m_level;
	unsigned m_maxLevel;
	unsigned m_capacity;
	bool m_alreadyAdded;

	void Subdivide();
};