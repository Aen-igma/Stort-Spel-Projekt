#include"PCH.h"
#include"Raycast.h"

namespace Aen {
	Raycast::Raycast() :m_hit(false), m_origin(Vec3f::zero), m_direction(0.f, 0.f, -1.f), m_maxDist(100.f), m_ray() {}

	Raycast::~Raycast() {}

	void Raycast::Update() {
		px::PxVec3 origin(m_origin.x, m_origin.y, m_origin.z);
		px::PxVec3 direction(m_direction.x, m_direction.y, m_direction.z);
		px::PxReal maxDist(m_maxDist);

		if(PhysicsHandler::GetInstance()->GetScene()->raycast(origin, direction, maxDist, m_ray)) {
			px::PxGeometryType::Enum t = m_ray.block.shape->getGeometryType();
			if(t != px::PxGeometryType::eCAPSULE) 
				m_hit = true;
		} else
			m_hit = false;
	}

	void Raycast::SetOrigin(const Aen::Vec3f& origin) {
		m_origin = origin;
	}

	void Raycast::SetOrigin(const float& x, const float& y, const float& z) {
		m_origin = Vec3f(x, y, z);
	}

	void Raycast::SetDirection(const Aen::Vec3f& dir) {
		m_direction = dir;
	}

	void Raycast::SetDirection(const float& x, const float& y, const float& z) {
		m_direction = Vec3f(x, y, z);
	}

	void Raycast::SetMaxDist(const float& dist) {
		m_maxDist = dist;
	}

	const bool Raycast::Hit() {
		return m_hit;
	}

	const Vec3f Raycast::GetHitPos() {
		if(m_hit) {
			px::PxVec3 p = m_ray.block.position;
			return Vec3f(p.x, p.y, p.z);
		}

		return m_origin + m_direction * m_maxDist;
	}

	const Vec3f Raycast::GetOrigin() {
		return m_origin;
	}

	const float Raycast::GetDistance() {
		if(m_hit) return m_ray.block.distance;
		return m_maxDist;
	}

	const Vec3f Raycast::GetDirection() {
		return m_direction;
	}

	const float Raycast::GetMaxDistance() {
		return m_maxDist;
	}
}