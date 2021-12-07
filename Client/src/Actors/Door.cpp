#include "Door.h"

Door::Door()
	:Interact()
{
	m_near = false;
	Aen::Mesh& door = Aen::Resource::CreateMesh("Door");
	door.Load(AEN_MODEL_DIR("door.fbx"));
	mp_object->AddComponent<Aen::MeshInstance>();
	mp_object->GetComponent<Aen::MeshInstance>().SetMesh("Door");
	mp_object->SetRot(0, 0, 0);

	mp_object->AddComponent<Aen::StaticBody>();
	mp_object->GetComponent<Aen::StaticBody>().SetBoundsToMesh(true);

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

	mp_object->GetComponent<Aen::MeshInstance>().SetMaterial("Brown1", DoorBrown);
	mp_object->GetComponent<Aen::MeshInstance>().SetMaterial("DoorMat", LightDoor);
	mp_object->GetComponent<Aen::MeshInstance>().SetMaterial("Gold", Gold);
	mp_object->GetComponent<Aen::MeshInstance>().SetMaterial("Light", blinn);
}

Door::~Door()
{
	//m_door->RemoveParent();
	Aen::EntityHandler::RemoveEntity(*mp_object);
}

void Door::Update(const float& deltaTime, Aen::Entity*& e)
{
	Aen::Vec3f eDir = e->GetPos() - mp_object->GetPos();
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
		mp_object->SetPos(0.f, -100, 0);
		m_type = Type::Open;
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
	return mp_object;
}

bool& Door::GetNear()
{
	return m_near;
}
