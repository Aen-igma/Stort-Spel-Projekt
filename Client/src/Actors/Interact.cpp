#include "Interact.h"

Interact::Interact()
	:mp_object(&Aen::EntityHandler::CreateEntity())
{
	mp_object->AddComponent<Aen::AABoundBox>();
	mp_object->AddComponent<Aen::CharacterController>();
}

Interact::~Interact()
{
}

Aen::Entity*& Interact::GetEntity()
{
	return mp_object;
}
