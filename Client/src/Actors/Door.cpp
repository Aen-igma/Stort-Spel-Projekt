#include "Door.h"

Door::Door()
	:Interact(), m_door(&Aen::EntityHandler::CreateEntity())
{
	m_near = false;
	Aen::Mesh& door = Aen::Resource::CreateMesh("Door");
	door.Load(AEN_MODEL_DIR("Fence.fbx"));
	m_door->AddComponent<Aen::MeshInstance>();
	m_door->GetComponent<Aen::MeshInstance>().SetMesh("Door");
	m_door->SetRot(0, 0, 0);
	m_door->SetScale(1.1f, 1, 1);

	m_door->AddComponent<Aen::StaticBody>();
	m_door->GetComponent<Aen::StaticBody>().SetBoundsToMesh(true);

	Aen::Material& Fence = Aen::Resource::CreateMaterial("Fence");
	Fence["InnerEdgeColor"] = Aen::Color(0.04f, 0.04f, 0.07f, 1.f);
	Fence["OuterEdgeColor"] = Aen::Color(0.04f, 0.04f, 0.07f, 1.f);
	Fence["InnerEdgeThickness"] = 1;
	Fence["OuterEdgeThickness"] = 2;
	Fence["BaseColor"] = Aen::Color(1.f, 1.f, 1.f, 1.f);
	Fence["Roughness"] = 0.f;

	m_door->GetComponent<Aen::MeshInstance>().SetMaterial("lambert1", Fence);
}

Door::~Door()
{
	//m_door->RemoveParent();
	Aen::EntityHandler::RemoveEntity(*m_door);
}

void Door::Update(const float& deltaTime, Aen::Entity*& e)
{
	Aen::Vec3f eDir = e->GetPos() - m_door->GetPos();
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

	if (m_type == Type::Opening) {

		m_door->MoveRelative(0, -5.f * deltaTime, 0);

		if (m_door->GetPos().y <= -8.f) {
			m_type = Type::Open;
		}
	}

	if (m_type == Type::Locking) {

		m_door->MoveRelative(0, 5.f * deltaTime, 0);

		if (m_door->GetPos().y >= 0.f) {

			m_type = Type::Locked;
		}
	}
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
