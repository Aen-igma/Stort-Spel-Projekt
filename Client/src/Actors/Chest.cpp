#include "Chest.h"

Chest::Chest()
	:Interact(), m_chest(&Aen::EntityHandler::CreateEntity())
{
	Aen::Animation& ChestOpen = Aen::Resource::CreateAnimation("ChestOpen");
	ChestOpen.LoadAnimation(AEN_MODEL_DIR("ChestOpen.fbx"));
	Aen::Mesh& chest = Aen::Resource::CreateMesh("ChestMesh");
	chest.Load(AEN_MODEL_DIR("chest.fbx"));
	Aen::Material& chestMat = Aen::Resource::CreateMaterial("ChestMaterial");
	chestMat.LoadeAndSetDiffuseMap(AEN_TEXTURE_DIR("chest.png"));

	m_chest->AddComponent<Aen::MeshInstance>();
	m_chest->GetComponent<Aen::MeshInstance>().SetMesh(chest);
	m_chest->GetComponent<Aen::MeshInstance>().SetMaterial(chestMat);
	m_chest->AddComponent<Aen::Animator>();
	m_chest->GetComponent<Aen::Animator>().AddAnimation(ChestOpen, "Open");
	m_chest->GetComponent<Aen::Animator>().SetLoopAnim(false);
	m_chest->GetComponent<Aen::Animator>().SetPaused(true);
	m_chest->SetScale(0.8f);


	m_chest->AddComponent<Aen::StaticBody>();
	m_chest->GetComponent<Aen::StaticBody>().SetBoundsToMesh();
	m_chest->SetRot(0, 0, 0);

	Aen::Material& ChestMaterial = Aen::Resource::CreateMaterial("ChestMat");
	ChestMaterial.LoadeAndSetDiffuseMap(AEN_TEXTURE_DIR("chest.png"));
	ChestMaterial["InnerEdgeColor"] = Aen::Color(0.2f, 0.26f, 0.37f, 1.f);
	ChestMaterial["OuterEdgeColor"] = Aen::Color(0.2f, 0.26f, 0.37f, 1.f);
	m_chest->GetComponent<Aen::MeshInstance>().SetMaterial("ChestMat");

	//m_chest->SetParent(*m_chest);
	//m_chest->GetComponent<Aen::AABoundBox>().SetBoundingBox(1.2f, 0.8f, 1.2f);
	//m_chest->SetPos(m_chest->GetPos().x, m_chest->GetPos().y, m_chest->GetPos().z);
}

Chest::~Chest()
{
	Aen::EntityHandler::RemoveEntity(*m_chest);
}

void Chest::Update(const float& deltaTime, Aen::Entity*& e)
{
	Aen::Vec3f eDir = e->GetPos() - m_chest->GetPos();
	float dist = eDir.Magnitude();
	
	if (m_type == Type::Locked)
		m_chest->GetComponent<Aen::Animator>().SetPaused(false);

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

Type Chest::GetType()
{
	return m_type;
}

bool& Chest::GetNear()
{
	return m_near;
}

void Chest::SetType(const Type& type)
{
	m_type = type;
}

Aen::Entity*& Chest::GetEntity()
{
	return m_chest;
}
