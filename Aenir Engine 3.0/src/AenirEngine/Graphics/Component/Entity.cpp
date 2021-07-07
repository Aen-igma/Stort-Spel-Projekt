#include "PCH.h"
#include "Entity.h"

namespace Aen {

	uint32_t Entity::m_iDs(0);

	Entity::~Entity() {
		TranformHandler::RemoveTranform(m_id);
		CameraHandler::RemoveCamera(m_id);
		MeshIHandler::RemoveMeshInstance(m_id);
	}

	Entity::Entity()
		:m_id(m_iDs) {
		m_iDs++;
	}
}