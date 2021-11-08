#pragma once
#include"RigidEnums.h"


namespace Aen {

	class AEN_DECLSPEC DynamicBody : public Component {
		public:
		DynamicBody(const size_t& id);

		void SetStaticFriction(const float& value);
		void SetDynamicFriction(const float& value);
		void SetRestitution(const float& value);
		void SetFrictionCombineMode(const CombineMode& mode);
		void SetRestitutionCombineMode(const CombineMode& mode);
		void SetMaterialFlag(const MaterialFlag& flag);
		void SetGeometry(const DynamicGeometryType& geometry, const Vec3f& scale = Vec3f::one);
		void SetAngularDamping(const float& damp);
		void SetSleepThresHold(const float& value);
		void SetMass(const float& mass);

		void AddForce(const Vec3f& force, const ForceMode& mode);
		void AddForce(const float& x, const float& y, const float& z, const ForceMode& mode);
		void AddTorque(const Vec3f& force, const ForceMode& mode);
		void AddTorque(const float& x, const float& y, const float& z, const ForceMode& mode);

		const Vec3f GetPos();
		const Vec3f GetRot();

		private:
		~DynamicBody();

		void RemoveRigid();

		const Mat4f GetTransform();
		const Mat4f GetTranslate();
		const Mat4f GetRotMat();
		void SetPos(const Vec3f& pos);
		void SetPos(const float& x, const float& y, const float& z);
		void SetRot(const Vec3f& rot);
		void SetRot(const float& p, const float& y, const float& r);

		float m_mass;
		float m_sleep;
		float m_aDamp;
		float m_density;
		DynamicGeometryType m_gType;
		DynamicGeometryType m_rigidType;
		px::PxMaterial* mp_Material;
		px::PxPhysics* mp_LocalPhysics;
		px::PxRigidDynamic* mp_DynamicBody;

		Vec3f m_scale;

		friend class ComponentHandler;
		friend class MeshInstance;
		friend class Entity;
		friend class ImGuiImporter;
	};
}