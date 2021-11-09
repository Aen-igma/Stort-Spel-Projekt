#pragma once
#include"PhysicsHandler.h"

namespace Aen {

	class AEN_DECLSPEC Raycast {
		public:
		Raycast();
		~Raycast();

		void Update();
		void SetOrigin(const Vec3f& origin);
		void SetOrigin(const float& x, const float& y, const float& z);
		void SetDirection(const Vec3f& dir);
		void SetDirection(const float& x, const float& y, const float& z);
		void SetMaxDist(const float& dist);

		const bool Hit();
		const Vec3f GetHitPos();
		const Vec3f GetOrigin();
		const float GetDistance();
		const Vec3f GetDirection();
		const float GetMaxDistance();

		private:
		bool m_hit;
		Vec3f m_origin;                
		Vec3f m_direction;               
		float m_maxDist;      
		px::PxRaycastBuffer  m_ray;
	};
}