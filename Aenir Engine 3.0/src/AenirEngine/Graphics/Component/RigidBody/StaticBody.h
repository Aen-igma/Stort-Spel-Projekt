#pragma once
#include"RigidEnums.h"


namespace Aen {

	class AEN_DECLSPEC StaticBody : public Component {
		public:
		StaticBody(const size_t& id);

		void SetStaticFriction(const float& value);
		void SetDynamicFriction(const float& value);
		void SetRestitution(const float& value);
		void SetFrictionCombineMode(const CombineMode& mode);
		void SetRestitutionCombineMode(const CombineMode& mode);
		void SetMaterialFlag(const MaterialFlag& flag);
		void SetGeometry(const StaticGeometryType& geometry, const Vec3f& scale = Vec3f::one);
		/* MakeTriangleMesh = cook the rigidbody of this mesh, insert = do real time cooking */
		void SetBoundsToMesh(const bool& MakeTriangleMesh = false, const bool& insert = false);

		const Vec3f GetPos();
		const Vec3f GetRot();

		private:
		~StaticBody();

		void RemoveRigid();

		const Mat4f GetTransform();
		const Mat4f GetTranslate();
		const Mat4f GetRotMat();
		void SetPos(const Vec3f& pos);
		void SetPos(const float& x, const float& y, const float& z);
		void Move(const Vec3f& pos);
		void Move(const float& x, const float& y, const float& z);
		void MoveRelative(const Vec3f& pos);
		void MoveRelative(const float& x, const float& y, const float& z);
		void SetRot(const Vec3f& rot);
		void SetRot(const float& p, const float& y, const float& r);
		void Rotate(const Vec3f& rot);
		void Rotate(const float& p, const float& y, const float& r);
		px::PxTriangleMesh* CookMesh(const bool& insert);

		StaticGeometryType m_gType;
		px::PxMaterial* mp_Material;
		px::PxPhysics* mp_LocalPhysics;
		px::PxRigidStatic* mp_StaticBody;

		Vec3f m_scale;

		friend class ComponentHandler;
		friend class MeshInstance;
		friend class Entity;
		friend class ImGuiImporter;
	};
}