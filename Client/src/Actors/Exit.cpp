#include "Exit.h"

Portal::Portal()
{
	m_scale = 0;
	m_near = false;
	m_exit = &Aen::EntityHandler::CreateEntity();
	Aen::Mesh& white = Aen::Resource::CreateMesh("PLANE");
	white.Load(AEN_MODEL_DIR("plane.fbx"));

	Aen::Material& whiteMat = Aen::Resource::CreateMaterial("White");
	whiteMat.LoadeAndSetDiffuseMap(AEN_TEXTURE_DIR("White.png"));
	whiteMat.LoadeAndSetEmissionMap(AEN_TEXTURE_DIR("White.png"));
	whiteMat["InnerEdgeColor"] = Aen::Color::White;
	whiteMat["OuterEdgeColor"] = Aen::Color::White;
	whiteMat["GlowColor"] = Aen::Color::White;

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
