#include "Interact.h"


Interact::Interact()
	:mp_object(&Aen::EntityHandler::CreateEntity())
{
}

Interact::~Interact()
{
	Aen::EntityHandler::RemoveEntity(*mp_object);
}
