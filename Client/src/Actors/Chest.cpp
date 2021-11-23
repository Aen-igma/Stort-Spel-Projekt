#include "Chest.h"

Chest::Chest()
	:Interact(), m_chest(&Aen::EntityHandler::CreateEntity())
{
	Aen::Mesh& chest = Aen::Resource::CreateMesh("Chest");
	chest.Load(AEN_RESOURCE_DIR("chest.fbx"));
	m_chest->AddComponent<Aen::MeshInstance>();
	m_chest->GetComponent<Aen::MeshInstance>().SetMesh("Chest");
	m_chest->SetScale(0.8f, 0.8f, 0.8f);
	m_chest->SetParent(*mp_object);

	mp_object->GetComponent<Aen::AABoundBox>().SetBoundingBox(1.2f, 0.8f, 1.2f);
	mp_object->SetPos(m_chest->GetPos().x, m_chest->GetPos().y, m_chest->GetPos().z);
}

Chest::~Chest()
{}

void Chest::Update(const float& deltaTime, Aen::Entity*& e)
{
	Aen::Vec3f eDir = e->GetPos() - mp_object->GetPos();
	float dist = eDir.Magnitude();

	if (dist < 3.f) {
		m_near = true;
	}
	else
		m_near = false;
}

Aen::Entity*& Chest::GetEntity()
{
	return m_chest;
}

bool& Chest::GetNear()
{
	return m_near;
}
