#pragma once
#include "../../Graphics.h"
namespace dx = DirectX;


class AEN_DECLSPEC CamClass
{
public:
	CamClass();

	void setProjMatrix(float fovDegrees, float aspectRatio, float nearZ, float farZ);

	const sm::Matrix getView() const;
	const sm::Matrix getProj() const;

	const sm::Vector3 getPosition() const;
	const sm::Vector3 getRotation() const;

	const sm::Vector3 getForwardV() const;
	const sm::Vector3 getLeftV() const;
	const sm::Vector3 getRightV() const;
	const sm::Vector3 getBackV() const;
	const sm::Vector3 getUpV() const;

	void setPosition(float x, float y, float z);
	void setPosition(sm::Vector3 xyz);
	void setRotation(float x, float y, float z);
	void setRotation(sm::Vector3 xyz);
	void move(float x, float y, float z);
	void move(sm::Vector3 xyz);
	void rotate(float x, float y, float z);
	void rotate(sm::Vector3 xyz);

private:
	void updateViewM();
	sm::Vector3 m_pos;
	sm::Vector3 m_rot;

	sm::Matrix m_viewMatrix;
	sm::Matrix m_projMatrix;

	const sm::Vector4 DEFAULT_FORWARD = { 0,0,1,0 };
	const sm::Vector4 DEFAULT_BACKWARD{ 0,0,-1,0 };
	const sm::Vector4 DEFAULT_UP = { 0,1,0,0 };
	const sm::Vector4 DEFAULT_DOWN = { 0,-1,0,0 };
	const sm::Vector4 DEFAULT_LEFT = { -1,0,0,0 };
	const sm::Vector4 DEFAULT_RIGHT = { 1,0,0,0 };

	sm::Vector3 m_forwardV;
	sm::Vector3 m_leftV;
	sm::Vector3 m_rightV;
	sm::Vector3 m_backV;
	sm::Vector3 m_upV;
};

