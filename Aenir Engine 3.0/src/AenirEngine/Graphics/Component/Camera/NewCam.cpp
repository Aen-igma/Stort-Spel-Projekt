#include "NewCam.h"

pt::CamClass::CamClass():
	m_pos(0,0,0), m_rot(0,0,0)
{
	updateViewM();
}

void pt::CamClass::setProjMatrix(float fovDegrees, float aspectRatio, float nearZ, float farZ)
{
	float fovRad = (fovDegrees / 360.f) * DirectX::XM_2PI;
	m_projMatrix = DirectX::XMMatrixPerspectiveFovRH(fovDegrees, aspectRatio, nearZ, farZ);
}

void pt::CamClass::rotate(sm::Vector3 xyz)
{
	m_rot += xyz;
	updateViewM();
}

void pt::CamClass::updateViewM()
{
	sm::Matrix camRotMat = DirectX::XMMatrixRotationRollPitchYawFromVector(m_rot);
	DirectX::XMVECTOR camTarget = DirectX::XMVector3TransformCoord(DEFAULT_FORWARD, camRotMat);
	camTarget += m_pos;
	dx::XMVECTOR upDir = dx::XMVector2TransformCoord(DEFAULT_UP, dx::XMMatrixRotationRollPitchYawFromVector(m_rot));

	m_viewMatrix = dx::XMMatrixLookAtRH(m_pos, camTarget, upDir);

	sm::Matrix vecRotationMat = dx::XMMatrixRotationRollPitchYaw(m_rot.x, m_rot.y, m_rot.z);
	this->m_forwardV = XMVector3TransformCoord(this->DEFAULT_FORWARD, vecRotationMat);
	this->m_backV = XMVector3TransformCoord(this->DEFAULT_BACKWARD, vecRotationMat);
	this->m_leftV = XMVector3TransformCoord(this->DEFAULT_LEFT, vecRotationMat);
	this->m_rightV = XMVector3TransformCoord(this->DEFAULT_RIGHT, vecRotationMat);
}

const sm::Matrix pt::CamClass::getView() const
{
	return m_viewMatrix;
}

const sm::Matrix pt::CamClass::getProj() const
{
	return m_projMatrix;
}

const sm::Vector3 pt::CamClass::getPosition() const
{
	return m_pos;
}

const sm::Vector3 pt::CamClass::getRotation() const
{
	return m_rot;
}

void pt::CamClass::setPosition(float x, float y, float z)
{
	m_pos = { x,y,z };
	updateViewM();
}

void pt::CamClass::setPosition(sm::Vector3 xyz)
{
	m_pos = xyz;
	updateViewM();
}

void pt::CamClass::setRotation(float x, float y, float z)
{
	m_rot = { x,y,z };
	updateViewM();
}

void pt::CamClass::setRotation(sm::Vector3 xyz)
{
	m_rot = xyz;
	updateViewM();
}

void pt::CamClass::move(float x, float y, float z)
{
	m_pos += {x, y, z};
	updateViewM();
}

void pt::CamClass::move(sm::Vector3 xyz)
{
	m_pos += xyz;
	updateViewM();
}

void pt::CamClass::rotate(float x, float y, float z)
{
	m_rot += {x,y,z};
	updateViewM();
}
