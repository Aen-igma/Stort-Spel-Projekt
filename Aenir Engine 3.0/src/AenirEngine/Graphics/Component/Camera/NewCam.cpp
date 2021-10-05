#include "PCH.h"
#include "NewCam.h"



CamClass::CamClass() :
	m_pos(0, 0, 0), m_rot(0, 0, 0)
{
	updateViewM();
}

void CamClass::setProjMatrix(float fovDegrees, float aspectRatio, float nearZ, float farZ)
{
	float fovRad = (fovDegrees / 360.f) * DirectX::XM_2PI;
	m_projMatrix = DirectX::XMMatrixPerspectiveFovRH(fovDegrees, aspectRatio, nearZ, farZ);
}

void CamClass::rotate(sm::Vector3 xyz)
{
	m_rot += xyz;
	updateViewM();
}

void CamClass::updateViewM()
{
	sm::Matrix camRotMat = DirectX::XMMatrixRotationRollPitchYawFromVector(m_rot);
	DirectX::XMVECTOR camTarget = DirectX::XMVector3TransformCoord(DEFAULT_FORWARD, camRotMat);
	camTarget += m_pos;
	dx::XMVECTOR upDir = dx::XMVector2TransformCoord(DEFAULT_UP, dx::XMMatrixRotationRollPitchYawFromVector(m_rot));

	m_viewMatrix = dx::XMMatrixLookAtRH(m_pos, camTarget, upDir);

	sm::Matrix vecRotationMat = dx::XMMatrixRotationRollPitchYaw(m_rot.x, m_rot.y, m_rot.z);
	m_forwardV = XMVector3TransformCoord(this->DEFAULT_FORWARD, vecRotationMat);
	m_backV = XMVector3TransformCoord(this->DEFAULT_BACKWARD, vecRotationMat);
	m_leftV = XMVector3TransformCoord(this->DEFAULT_LEFT, vecRotationMat);
	m_rightV = XMVector3TransformCoord(this->DEFAULT_RIGHT, vecRotationMat);
	m_upV = XMVector3TransformCoord({ 0,1,0,0 }, vecRotationMat);
}

const sm::Matrix CamClass::getView() const
{
	return m_viewMatrix;
}

const sm::Matrix CamClass::getProj() const
{
	return m_projMatrix;
}

const sm::Vector3 CamClass::getPosition() const
{
	return m_pos;
}

const sm::Vector3 CamClass::getRotation() const
{
	return m_rot;
}

const sm::Vector3 CamClass::getForwardV() const
{
	return m_forwardV;
}

const sm::Vector3 CamClass::getLeftV() const
{
	return m_leftV;
}

const sm::Vector3 CamClass::getRightV() const
{
	return m_rightV;
}

const sm::Vector3 CamClass::getBackV() const
{
	return m_backV;
}

const sm::Vector3 CamClass::getUpV() const
{
	return m_upV;
}

void CamClass::setPosition(float x, float y, float z)
{
	m_pos = { x,y,z };
	updateViewM();
}

void CamClass::setPosition(sm::Vector3 xyz)
{
	m_pos = xyz;
	updateViewM();
}

void CamClass::setRotation(float x, float y, float z)
{
	m_rot = { x,y,z };
	updateViewM();
}

void CamClass::setRotation(sm::Vector3 xyz)
{
	m_rot = xyz;
	updateViewM();
}

void CamClass::move(float x, float y, float z)
{
	m_pos += {x, y, z};
	updateViewM();
}

void CamClass::move(sm::Vector3 xyz)
{
	m_pos += xyz;
	updateViewM();
}

void CamClass::rotate(float x, float y, float z)
{
	m_rot += {x, y, z};
	updateViewM();
}


