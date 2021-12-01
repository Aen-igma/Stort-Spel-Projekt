#include "Chest.h"

Chest::Chest()
	:Interact(), m_chest(&Aen::EntityHandler::CreateEntity()), m_near(false)
{
	Aen::Mesh& chest = Aen::Resource::CreateMesh("Chest");
	chest.Load(AEN_MODEL_DIR("chest.fbx"));
	Aen::Material& chestMat = Aen::Resource::CreateMaterial("ChestMaterial");
	chestMat.LoadeAndSetDiffuseMap(AEN_TEXTURE_DIR("chest.png"));
	m_chest->AddComponent<Aen::MeshInstance>();
	m_chest->GetComponent<Aen::MeshInstance>().SetMesh(chest);
	m_chest->GetComponent<Aen::MeshInstance>().SetMaterial(chestMat);
	m_chest->SetScale(0.8f);


	m_chest->AddComponent<Aen::StaticBody>();
	m_chest->GetComponent<Aen::StaticBody>().SetBoundsToMesh(true);
	m_chest->SetRot(0, 0, 0);

	//m_chest->SetParent(*mp_object);

	//mp_object->GetComponent<Aen::AABoundBox>().SetBoundingBox(1.2f, 0.8f, 1.2f);
	//mp_object->SetPos(m_chest->GetPos().x, m_chest->GetPos().y, m_chest->GetPos().z);
}

Chest::~Chest()
{
	m_chest->RemoveParent();
	Aen::EntityHandler::RemoveEntity(*m_chest);
}

void Chest::Update(const float& deltaTime, Aen::Entity*& e)
{
	Aen::Vec3f eDir = e->GetPos() - m_chest->GetPos();
	float dist = eDir.Magnitude();

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

Aen::Entity*& Chest::GetObjectEntity()
{
	return mp_object;
}

Type Chest::GetType()
{
	return m_type;
}

void Chest::SetType(const Type& type)
{
	m_type = type;
}

Aen::Entity*& Chest::GetEntity()
{
	return m_chest;
}

bool& Chest::GetNear()
{
	return m_near;
}
