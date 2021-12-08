#include "Exit.h"

Portal::Portal():m_Mat(Aen::Resource::CreateMaterial("White"))
{
	m_scale = 0;
	m_Opacity = 0.f;
	m_near = false;
	m_exit = &Aen::EntityHandler::CreateEntity();
	Aen::Mesh& white = Aen::Resource::CreateMesh("PLANE");
	white.Load(AEN_MODEL_DIR("plane.fbx"));

	m_Mat.LoadeAndSetDiffuseMap(AEN_TEXTURE_DIR("White.png"));
	m_Mat.LoadeAndSetEmissionMap(AEN_TEXTURE_DIR("White.png"));
	m_Mat["InnerEdgeColor"] = Aen::Color::White;
	m_Mat["OuterEdgeColor"] = Aen::Color::White;
	m_Mat["GlowColor"] = Aen::Color::White;

	m_exit->AddComponent<Aen::MeshInstance>();
	m_exit->GetComponent<Aen::MeshInstance>().SetMesh("PLANE");
	m_exit->GetComponent<Aen::MeshInstance>().SetMaterial("White");
	m_exit->SetScale(0, 1, 100);
	m_type = Type::Open;
}

Portal::~Portal()
{
	Aen::EntityHandler::RemoveEntity(*m_exit);
}

void Portal::Update(const float& deltaTime)
{
	if (m_type == Type::Opening) {

		m_scale += deltaTime * 2.f;
		m_exit->SetScale(m_scale, 1, 100);

		if (m_scale > 10.f) {
			m_type = Type::Locked;
		}
	}
}

void Portal::Update(const float& deltaTime, Aen::Entity*& e)
{}

Type Portal::GetType()
{
	return m_type;
}

bool& Portal::GetNear()
{
	return m_near;
}

Aen::Entity*& Portal::GetEntity()
{
	return m_exit;
}

void Portal::SetType(const Type& type)
{
	m_type = type;
}
