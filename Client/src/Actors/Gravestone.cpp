#include "Gravestone.h"

Gravestone::Gravestone()
	:Interact(), m_graveStone(&Aen::EntityHandler::CreateEntity())
{
	m_near = false;
	m_type = Type::Open;
	Aen::Mesh& door = Aen::Resource::CreateMesh("GraveStone");
	door.Load(AEN_MODEL_DIR("SkeletonGrave.fbx"));
	m_graveStone->AddComponent<Aen::MeshInstance>();
	m_graveStone->GetComponent<Aen::MeshInstance>().SetMesh("GraveStone");
	m_graveStone->SetRot(0, 0, 0);

	m_graveStone->AddComponent<Aen::StaticBody>();
	m_graveStone->GetComponent<Aen::StaticBody>().SetBoundsToMesh(true);

	Aen::Material& graveMat = Aen::Resource::CreateMaterial("graveMat");
	graveMat.LoadeAndSetDiffuseMap(AEN_TEXTURE_DIR("Grave.png"));
	graveMat["InnerEdgeColor"] = Aen::Color(0.2f, 0.26f, 0.37f, 1.f);
	graveMat["OuterEdgeColor"] = Aen::Color(0.2f, 0.26f, 0.37f, 1.f);
	m_graveStone->GetComponent<Aen::MeshInstance>().SetMaterial("graveMat");
}

Gravestone::~Gravestone()
{
	Aen::EntityHandler::RemoveEntity(*m_graveStone);
}

Aen::Entity*& Gravestone::GetEntity()
{
	return m_graveStone;
}

bool& Gravestone::GetNear()
{
	return m_near;
}

void Gravestone::Update(const float& deltaTime, Aen::Entity*& e)
{
	Aen::Vec3f eDir = e->GetPos() - m_graveStone->GetPos();
	float dist = eDir.Magnitude();
	static float timer = 0;

	//player
	if (e->GetTag() == "Player") {
		if (dist < 3.f) {
			m_near = true;
		}
		else {
			m_near = false;
		}
	}
}

Type Gravestone::GetType()
{
	return m_type;
}

void Gravestone::SetType(const Type& type)
{
	m_type = type;
}
