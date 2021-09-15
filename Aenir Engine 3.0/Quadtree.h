#pragma once
#include "AenWindow.h"
#include "AenGraphics.h"
#include "AenDefines.h"
#include "Graphics/Graphics.h"

//Points where we mark on the 2D space
struct point {
	float m_x;
	float m_y;
	point(float x, float y) {
		this->m_x = x;
		this->m_y = y;
	}
};

//To construct the boundery box, needs to use the window x,y for right cordinates
struct rectangle
{
	float m_x;
	float m_y;
	float m_z;
	float m_h;
	rectangle(float  x, float y, float z, float h) {
		this->m_x = x;
		this->m_y = y;
		this->m_z = z;
		this->m_h = h;
	}

};

//MainClass for building quadtree
class Quadtree
{
public:
	Quadtree();
	~Quadtree();

	void boundery(float x, float y, float z, float h){};
	void subDivide() {};
private:
	int capacity;

	float m_x;
	float m_y;
	float m_z;
	float m_h;
	bool m_active;

};