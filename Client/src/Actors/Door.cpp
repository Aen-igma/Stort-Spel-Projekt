#include "Door.h"

Door::Door()
	:Interact(), m_door(&Aen::EntityHandler::CreateEntity()), m_near(false)
{
	Aen::Mesh& door = Aen::Resource::CreateMesh("Door");
	door.Load(AEN_MODEL_DIR("door.fbx"));
	m_door->AddComponent<Aen::MeshInstance>();
	m_door->GetComponent<Aen::MeshInstance>().SetMesh("Door");
	m_door->SetRot(0, 0, 0);

	m_door->AddComponent<Aen::StaticBody>();
	m_door->GetComponent<Aen::StaticBody>().SetBoundsToMesh(true);

	//mp_object->GetComponent<Aen::AABoundBox>().SetBoundingBox(1.2f, 0.8f, 1.2f);

	Aen::Material& DoorBrown = Aen::Resource::CreateMaterial("DoorBrown");
	DoorBrown["InnerEdgeColor"] = Aen::Color::Brown;
	DoorBrown["OuterEdgeColor"] = Aen::Color::Brown;
	DoorBrown["InnerEdgeThickness"] = 1;
	DoorBrown["OuterEdgeThickness"] = 2;
	DoorBrown["BaseColor"] = Aen::Color(0.036f, 0.02f, 0.002f, 1.f);
	DoorBrown["RimLightColor"] = Aen::Color(0.8f, 0.2f, 0.1f, 1.f);
	DoorBrown["RimLightIntensity"] = 1.f;
	DoorBrown["RimLightSize"] = 0.6f;

	Aen::Material& Gold = Aen::Resource::CreateMaterial("Gold");
	Gold["InnerEdgeColor"] = Aen::Color(0.06f, 0.07f, 0.07f, 1.f);
	Gold["OuterEdgeColor"] = Aen::Color::Yellow ;
	Gold["InnerEdgeThickness"] = 1;
	Gold["OuterEdgeThickness"] = 2;
	Gold["BaseColor"] = Aen::Color(0.923f, 0.895f, 0.036f, 1.f);

	Aen::Material& blinn = Aen::Resource::CreateMaterial("blinn");
	blinn["InnerEdgeColor"] = Aen::Color(0.04f, 0.04f, 0.07f, 1.f);
	blinn["OuterEdgeColor"] = Aen::Color::White;
	blinn["InnerEdgeThickness"] = 1;
	blinn["OuterEdgeThickness"] = 2;
	blinn["BaseColor"] = Aen::Color(0.388f, 0.63f, 0.76f, 1.f);
	blinn["Roughness"] = 0.f;

	Aen::Material& LightDoor = Aen::Resource::CreateMaterial("LightDoor");
	LightDoor["InnerEdgeColor"] = Aen::Color(0.1f, 0.03f, 0.01f, 1.f);
	LightDoor["OuterEdgeColor"] = Aen::Color(0.13f, 0.1f, 0.08f, 1.f);
	LightDoor["InnerEdgeThickness"] = 1;
	LightDoor["OuterEdgeThickness"] = 1;
	LightDoor["BaseColor"] = Aen::Color(0.107f, 0.074f, 0.044f, 1.f);
	LightDoor["ShadowColor"] = Aen::Color(0.3f, 0.2f, 0.2f, 1.f);
	LightDoor["ShadowOffset"] = 1.f;
	LightDoor["SpecularColor"] = Aen::Color::Red;

	m_door->GetComponent<Aen::MeshInstance>().SetMaterial("Brown1", DoorBrown);
	m_door->GetComponent<Aen::MeshInstance>().SetMaterial("DoorMat", LightDoor);
	m_door->GetComponent<Aen::MeshInstance>().SetMaterial("Gold", Gold);
	m_door->GetComponent<Aen::MeshInstance>().SetMaterial("Light", blinn);
}

Door::~Door()
{
	m_door->RemoveParent();
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

		if (m_type == Type::Open && dist > 5.f) {
			m_type = Type::Locking;
		}
	}

	if (m_type == Type::Locking) {
		
		timer += 0.05f * deltaTime;
		m_door->MoveRelative(-timer, 0, 0);

		if (timer > 0.1f) {
			m_type = Type::Locked;
			timer = 0;
		}
	}

	if (m_type == Type::Opening) {
		timer += 0.05f * deltaTime;
		m_door->MoveRelative(timer, 0, 0);

		if (timer > 0.1f) {
			m_type = Type::Open;
			timer = 0;
		}

		cout << timer << endl;
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
