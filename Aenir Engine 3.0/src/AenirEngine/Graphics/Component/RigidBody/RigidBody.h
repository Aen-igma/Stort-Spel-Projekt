#pragma once
#include"../Component.h"
#include"../../Physics/PhysicsHandler.h"

namespace Aen {

	enum class CombineMode {
		AVERAGE = 0,
		MIN = 1,
		MULTIPLY = 2,
		MAX = 3
	};

	enum class MaterialFlag {
		DISABLE_FRICTION = 1 << 0, 
		DISABLE_STRONG_FRICTION = 1 << 1, 
		IMPROVED_PATCH_FRICTION = 1 << 2 
	};

	enum class RigidType {
		STATIC,
		DYNAMIC
	};

	enum class GeometryType {
		SPHERE,
		CAPSULE,
		PLANE,
		CUBE
	};

	enum class ForceMode {
		FORCE,			
		IMPULSE,		
		VELOCITY_CHANGE,
		ACCELERATION	
	};

	class AEN_DECLSPEC RigidBody : public Component {
	public:
		RigidBody(const size_t& id);

		void SetStaticFriction(const float& value);
		void SetDynamicFriction(const float& value);
		void SetRestitution(const float& value);
		void SetFrictionCombineMode(const CombineMode& mode);
		void SetRestitutionCombineMode(const CombineMode& mode);
		void SetMaterialFlag(const MaterialFlag& flag);
		void SetRigidType(const RigidType& type);
		void SetGeometry(const GeometryType& geometry, const Vec3f& scale = Vec3f::one);
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
	~RigidBody();

	void RemoveRigid();

	const Mat4f GetTransform();
	void SetPos(const Vec3f& pos);
	void SetPos(const float& x, const float& y, const float& z);
	void SetRot(const Vec3f& rot);
	void SetRot(const float& p, const float& y, const float& r);



	float m_mass;
	float m_sleep;
	float m_aDamp;
	float m_density;
	GeometryType m_gType;
	RigidType m_rigidType;
	px::PxMaterial* mp_Material;
	px::PxPhysics* mp_LocalPhysics;
	px::PxRigidStatic* mp_StaticBody;
	px::PxRigidDynamic* mp_DynamicBody;

	Vec3f m_scale;

	friend class ComponentHandler;
	friend class MeshInstance;
	friend class Entity;
	};
}