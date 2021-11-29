#include "Door.h"

Door::Door()
	:Interact(), m_door(&Aen::EntityHandler::CreateEntity()), m_near(false)
{
	Aen::Mesh& door = Aen::Resource::CreateMesh("Door");
	door.Load(AEN_RESOURCE_DIR("door.fbx"));
	m_door->AddComponent<Aen::MeshInstance>();
	m_door->GetComponent<Aen::MeshInstance>().SetMesh("Door");
	m_door->SetScale(0.4f, 0.4f, 0.4f);
	
	m_door->AddComponent<Aen::StaticBody>();
	m_door->GetComponent<Aen::StaticBody>().SetBoundsToMesh(true);

	//m_door->SetParent(*mp_object);

	//mp_object->AddComponent<Aen::StaticBody>();
	//mp_object->GetComponent<Aen::StaticBody>().set
	//mp_object->GetComponent<Aen::StaticBody>().SetBoundsToMesh(true);
	//mp_object->SetPos(m_door->GetPos().x, m_door->GetPos().y, m_door->GetPos().z);
}

Door::~Door()
{
	m_door->RemoveParent();
	Aen::EntityHandler::RemoveEntity(*m_door);
}

void Door::Update(const float& deltaTime, Aen::Entity*& e)
{
	Aen::Vec3f eDir = e->GetPos() - mp_object->GetPos();
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

Aen::Entity*& Door::GetObjectEntity()
{
	return mp_object;
}

Type Door::GetType()
{
	return m_type;
}

void Door::SetType(const Type& type)
{
	m_type = type;
}

Aen::Entity*& Door::GetEntity()
{
	return m_door;
}

bool& Door::GetNear()
{
	return m_near;
}
