#include "PCH.h"
#include "Component.h"

namespace Aen {

    Component::Component(const size_t& id)
        :m_id(id) {}

    const size_t& Component::GetId() {
        return m_id;
    }
}
