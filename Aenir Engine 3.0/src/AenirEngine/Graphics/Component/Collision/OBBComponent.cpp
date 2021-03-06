#include "PCH.h"
#include "OBBComponent.h"
#include "../ComponentHandler.h"
#include"Core/Renderer.h"

Aen::OBBox::OBBox(const size_t& id)
	:Drawable(id), m_offset(Vec3f::zero), m_isColliding(false), m_isOn(false)
#ifdef _DEBUG
	,m_canDraw(false), m_isFrustum(false)
#endif
{
	SetBoundsToMesh();

#ifdef _DEBUG
	DirectX::BoundingOrientedBox tBox;
	tBox.Extents = m_obb.Extents;

	const size_t size = tBox.CORNER_COUNT;
	DirectX::XMFLOAT3 points[size];
	tBox.GetCorners(points);
	for (uint32_t i = 0; i < size; i++)
		m_verts[i].pos = Vec3f(points[i].x, points[i].y, points[i].z);

	DWORD ind[]{
		0, 1, 2,
		0, 2, 3,
		1, 5, 6,
		1, 6, 2,
		5, 4, 7,
		5, 7, 6,
		4, 0, 3,
		4, 3, 7,
		6, 3, 2,
		6, 7, 3,
		5, 0, 4,
		5, 1, 0,
	};

	if (!m_vBuffer.Create(m_verts, (UINT)size, D3D11_USAGE_DYNAMIC))
		throw;

	m_iBuffer.Create(ind, 36u);
#endif

	m_canDraw = true;
}

Aen::OBBox::~OBBox() {}

const bool Aen::OBBox::Intersects(OBBox& volume)
{
	if (m_obb.Intersects(volume.m_obb) && m_isOn && volume.m_isOn)
	{
		volume.m_isColliding = true;
		return true;
	}
	volume.m_isColliding = false;
	return false;
}

const bool Aen::OBBox::Intersects(AABoundBox& volume)
{
	if (m_isOn && m_obb.Intersects(volume.m_aabb) && volume.m_isOn)
	{
		volume.m_isColliding = m_isColliding = true;
		return true;
	}
	volume.m_isColliding = m_isColliding = false;
	return false;
}

void Aen::OBBox::SetOffset(const Vec3f& offset)
{
	m_offset = offset;
	m_obb.Extents = m_obb.Extents + m_offset.smVec;
}

void Aen::OBBox::SetOffset(const float& x, const float& y, const float& z)
{
	m_offset = Vec3f(x, y, z);
	m_obb.Extents = m_obb.Extents + m_offset.smVec;
}

void Aen::OBBox::SetBoundsToMesh()
{
	if (ComponentHandler::MeshInstanceExist(m_id))
		if (ComponentHandler::GetMeshInstance(m_id).m_pMesh) {
			m_obb = ComponentHandler::GetMeshInstance(m_id).m_pMesh->m_obb;

#ifdef _DEBUG
			DirectX::BoundingOrientedBox tBox(m_obb);
			tBox.Transform(tBox, EntityHandler::GetEntity(m_id).GetTransformation().smMat);

			const size_t size = tBox.CORNER_COUNT;
			DirectX::XMFLOAT3 points[size];
			tBox.GetCorners(points);
			for (uint32_t i = 0; i < size; i++)
				m_verts[i].pos = Vec3f(points[i].x, points[i].y, points[i].z);
#endif
		}
}

void Aen::OBBox::SetBoundingBox(const Vec3f& extents)
{
	m_obb.Extents = extents.smVec + m_offset.smVec;
	UpdateVerts();
}

void Aen::OBBox::SetBoundingBox(const float& x, const float& y, const float& z)
{
	Vec3f extents = { x,y,z };
	m_obb.Extents = extents.smVec + m_offset.smVec;
#ifdef _DEBUG
	UpdateVerts();
	DirectX::XMFLOAT3 corners[8];
	m_obb.GetCorners(corners);
	for (int i = 0; i < 8; i++)
		m_verts[i].pos = Vec3f(corners[i].x, corners[i].y, corners[i].z);
#endif
}

//void Aen::OBBox::Transform(Aen::Mat4f transform)
//{
//}
//
//void Aen::OBBox::Transform(sm::Matrix transform)
//{
//}

void Aen::OBBox::SetOrientation(Aen::Vec3f v)
{
	m_obb.Orientation = sm::Vector4(DirectX::XMQuaternionRotationRollPitchYaw(v.x, v.y, v.z));
}

void Aen::OBBox::SetOrientation(const float& x, const float& y, const float& z)
{
	m_obb.Orientation = sm::Vector4(DirectX::XMQuaternionRotationRollPitchYaw(x, y, z));
}

void Aen::OBBox::ToggleActive(bool b)
{
	m_isOn = b;
}

void Aen::OBBox::ToggleActive()
{
	m_isOn = !m_isOn;
}

const bool Aen::OBBox::GetIsOn() const
{
	return m_isOn;
}

const void Aen::OBBox::ToggleIsFrustum(bool b)
{
	m_isFrustum = b;
}

void Aen::OBBox::UpdateVerts()
{
#ifdef _DEBUG
	DirectX::XMFLOAT3 corners[8];
	m_obb.GetCorners(corners);
	for (int i = 0; i < 8; i++)
		m_verts[i].pos = Vec3f(corners[i].x, corners[i].y, corners[i].z);
#endif
}

void Aen::OBBox::UpdateCamVerts(const DirectX::BoundingFrustum& cam)
{
#ifdef _DEBUG
	DirectX::XMFLOAT3 corners[8];
	cam.GetCorners(corners);
	for (int i = 0; i < 8; i++)
		m_verts[i].pos = Vec3f(corners[i].x, corners[i].y, corners[i].z);
#endif
}

void Aen::OBBox::Draw(Renderer& renderer, const uint32_t& layer)
{
#ifdef _DEBUG
	if (m_canDraw)
	{
		RenderSystem::SetPrimitiveTopology(Topology::TRIANGLELIST);
		Vec3f p = Vec3f(m_obb.Center.x, m_obb.Center.y, m_obb.Center.z);
		renderer.m_cbTransform.GetData().m_mdlMat = Mat4f::identity;
		renderer.m_cbTransform.UpdateBuffer();
		renderer.m_cbTransform.BindBuffer<VShader>(0u);
		RenderSystem::SetPrimitiveTopology(Topology::TRIANGLELIST);

		renderer.m_collisionBuffer.BindBuffer<PShader>(0);
		if (m_isFrustum) renderer.m_collisionBuffer.GetData().color = { 1.f,0.f,0.f };
		if (!m_isOn) renderer.m_collisionBuffer.GetData().color = { .2f,.2f,.2f };
		else if (m_isColliding) renderer.m_collisionBuffer.GetData().color = { 0.f,1.f,0.f };
		else renderer.m_collisionBuffer.GetData().color = { 1.f,0.f,0.f };
		renderer.m_collisionBuffer.GetData().switcher = 0;
		renderer.m_collisionBuffer.UpdateBuffer();

		RenderSystem::SetRasteriserState(renderer.m_wireFrameState);
		RenderSystem::BindShader(renderer.m_opaqueVS);
		RenderSystem::BindShader(renderer.m_collisionPS);
		RenderSystem::SetInputLayout(renderer.m_opaqueLayout);
		RenderSystem::BindRenderTargetView(renderer.m_backBuffer);
		m_vBuffer.BindBuffer();
		m_iBuffer.BindBuffer();
		m_iBuffer.DrawIndexed();
		RenderSystem::UnBindRenderTargets(1u);
		RenderSystem::UnBindShader<VShader>();
		RenderSystem::UnBindShader<PShader>();
	}
#endif

}

void Aen::OBBox::DepthDraw(Renderer& renderer, const uint32_t& layer)
{

	Vec3f transformation = EntityHandler::GetEntity(m_id).GetTranslation();
	RenderSystem::SetPrimitiveTopology(Topology::TRIANGLELIST);


	Vec3f rot = EntityHandler::GetEntity(m_id).GetRot();

	
	m_obb.Center = transformation.smVec;
	if (ComponentHandler::CharacterControllerExist(m_id))
	{
		m_obb.Center = ComponentHandler::GetCharacterController(m_id).GetPos().smVec + m_offset.smVec;
	}
	else if (ComponentHandler::DynamicBodyExist(m_id))
	{
		m_obb.Center = ComponentHandler::GetDynamicBody(m_id).GetPos().smVec + m_offset.smVec;
		 
	}
	else if (ComponentHandler::StaticBodyExist(m_id))
	{
		m_obb.Center = ComponentHandler::GetStaticBody(m_id).GetPos().smVec + m_offset.smVec;
		
	}
	sm::Vector4 quatRot = DirectX::XMQuaternionRotationRollPitchYaw(DegToRad(rot.x), DegToRad(rot.y), DegToRad(rot.z));


#ifdef _DEBUG
	if(!m_isFrustum)
		UpdateVerts();
	if (m_canDraw) {
		RenderSystem::SetPrimitiveTopology(Topology::TRIANGLELIST);
		m_vBuffer.UpdateBuffer(m_verts, 8);
		Vec3f p = Vec3f(m_obb.Center.x, m_obb.Center.y, m_obb.Center.z);
		renderer.m_cbTransform.GetData().m_mdlMat = Mat4f::identity;
		renderer.m_cbTransform.UpdateBuffer();
		renderer.m_cbTransform.BindBuffer<VShader>(0u);
		RenderSystem::SetInputLayout(renderer.m_opaqueLayout);
		RenderSystem::BindShader<VShader>(renderer.m_opaqueVS);
		RenderSystem::UnBindShader<PShader>();
		m_vBuffer.BindBuffer();
		m_iBuffer.BindBuffer();
		m_iBuffer.DrawIndexed();
	}
#endif
}

bool Aen::OBBox::FrustumCull(Renderer& renderer)
{
	Vec3f transformation = EntityHandler::GetEntity(m_id).GetTranslation();

	m_obb.Center = transformation.smVec;
	if (ComponentHandler::DynamicBodyExist(m_id))
		m_obb.Center = ComponentHandler::GetDynamicBody(m_id).GetPos().smVec + m_offset.smVec;
	else if (ComponentHandler::StaticBodyExist(m_id))
		m_obb.Center = ComponentHandler::GetStaticBody(m_id).GetPos().smVec + m_offset.smVec;
	else if (ComponentHandler::CharacterControllerExist(m_id))
		m_obb.Center = ComponentHandler::GetCharacterController(m_id).GetPos().smVec + m_offset.smVec;
#ifdef _DEBUG
	return true;
#endif
	return false;
}
