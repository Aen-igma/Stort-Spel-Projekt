#include "Chest.h"

Chest::Chest()
	:Interact()
{
	Aen::Animation& ChestOpen = Aen::Resource::CreateAnimation("ChestOpen");
	ChestOpen.LoadAnimation(AEN_MODEL_DIR("ChestOpen.fbx"));
	Aen::Mesh& chest = Aen::Resource::CreateMesh("ChestMesh");
	chest.Load(AEN_MODEL_DIR("chest.fbx"));
	Aen::Material& chestMat = Aen::Resource::CreateMaterial("ChestMaterial");
	chestMat.LoadeAndSetDiffuseMap(AEN_TEXTURE_DIR("chest.png"));

	mp_object->AddComponent<Aen::MeshInstance>();
	mp_object->GetComponent<Aen::MeshInstance>().SetMesh(chest);
	mp_object->GetComponent<Aen::MeshInstance>().SetMaterial(chestMat);
	mp_object->AddComponent<Aen::Animator>();
	mp_object->GetComponent<Aen::Animator>().AddAnimation(ChestOpen, "Open");
	mp_object->GetComponent<Aen::Animator>().SetLoopAnim(false);
	mp_object->GetComponent<Aen::Animator>().SetPaused(true);
	mp_object->SetScale(0.8f);


	mp_object->AddComponent<Aen::StaticBody>();
	mp_object->GetComponent<Aen::StaticBody>().SetBoundsToMesh();
	mp_object->SetRot(0, 0, 0);

	Aen::Material& ChestMaterial = Aen::Resource::CreateMaterial("ChestMat");
	ChestMaterial.LoadeAndSetDiffuseMap(AEN_TEXTURE_DIR("chest.png"));
	ChestMaterial["InnerEdgeColor"] = Aen::Color(0.2f, 0.26f, 0.37f, 1.f);
	ChestMaterial["OuterEdgeColor"] = Aen::Color(0.2f, 0.26f, 0.37f, 1.f);
	mp_object->GetComponent<Aen::MeshInstance>().SetMaterial("ChestMat");

	//mp_object->SetParent(*mp_object);
	//mp_object->GetComponent<Aen::AABoundBox>().SetBoundingBox(1.2f, 0.8f, 1.2f);
	//mp_object->SetPos(mp_object->GetPos().x, mp_object->GetPos().y, mp_object->GetPos().z);
}

Chest::~Chest()
{
	mp_object->RemoveParent();
	Aen::EntityHandler::RemoveEntity(*mp_object);
}

void Chest::Update(const float& deltaTime, Aen::Entity*& e)
{
	Aen::Vec3f eDir = e->GetPos() - mp_object->GetPos();
	float dist = eDir.Magnitude();
	
	if (m_type == Type::Locked)
		mp_object->GetComponent<Aen::Animator>().SetPaused(false);

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
	return mp_object;
}
