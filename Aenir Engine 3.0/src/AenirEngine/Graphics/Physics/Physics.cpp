#include"PCH.h"
#include"Physics.h"

namespace Aen {

	PhysXWrap::PhysXWrap() :m_DefaultAllocatorCallback(), m_DefaultErrorCallback(), m_ToleranceScale(), mp_Dispatcher(nullptr),
		mp_Foundation(nullptr), mp_Material(nullptr), mp_Physics(nullptr), mp_Cooking(nullptr), mp_Scene(nullptr),
		m_StackZ(9.0f), mp_Pvd(nullptr) {}

	PhysXWrap::~PhysXWrap() {

		mp_Scene->release();
		mp_Dispatcher->release();
		mp_Physics->release();
		mp_Cooking->release();

		if (mp_Pvd) {
			physx::PxPvdTransport* transport = mp_Pvd->getTransport();
			mp_Pvd->release();
			mp_Pvd = NULL;
			transport->release();
		}
		mp_Foundation->release(); // Always release last

		mp_Dispatcher = nullptr;
		mp_Foundation = nullptr;
		mp_Material =   nullptr;
		mp_Scene =      nullptr;
		mp_Pvd =	    nullptr;
		mp_Physics =    nullptr;
		mp_Cooking =    nullptr;
	}

	void PhysXWrap::InitPhysics(const int& toleranceLength, const int& toleranceSpeed) {
		mp_Foundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_DefaultAllocatorCallback, m_DefaultErrorCallback);
		if (!mp_Foundation) throw("PxCreateFoundation Failed!");

		mp_Pvd = PxCreatePvd(*mp_Foundation);
		px::PxPvdTransport* transport = px::PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
		mp_Pvd->connect(*transport, px::PxPvdInstrumentationFlag::eALL);

		m_ToleranceScale.length = toleranceLength;
		m_ToleranceScale.speed = toleranceSpeed;
		mp_Physics = PxCreatePhysics(PX_PHYSICS_VERSION, *mp_Foundation, m_ToleranceScale, true, mp_Pvd);
		if (!mp_Physics) throw("PxCreatePhysics Failed!");

		px::PxCookingParams params(m_ToleranceScale);
		//params.meshPreprocessParams.set(px::PxMeshPreprocessingFlag::eWELD_VERTICES);
		params.meshPreprocessParams.set(px::PxMeshPreprocessingFlag::eDISABLE_CLEAN_MESH);


		mp_Cooking = PxCreateCooking(PX_PHYSICS_VERSION, *mp_Foundation, params);
		if (!mp_Cooking) throw("PxCreateCooking Failed!");


		px::PxSceneDesc sceneDesc(mp_Physics->getTolerancesScale());
		sceneDesc.gravity = px::PxVec3(0.0f, -30.f, 0.0f);
		mp_Dispatcher = px::PxDefaultCpuDispatcherCreate(2);
		sceneDesc.cpuDispatcher = mp_Dispatcher;
		sceneDesc.filterShader = px::PxDefaultSimulationFilterShader;
		mp_Scene = mp_Physics->createScene(sceneDesc);	

		px::PxPvdSceneClient* pvdClient = mp_Scene->getScenePvdClient();
		if (pvdClient) {
			pvdClient->setScenePvdFlag(px::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
			pvdClient->setScenePvdFlag(px::PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
			pvdClient->setScenePvdFlag(px::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
		}
	}

	void PhysXWrap::RunPhysics(const float& deltaTime) {
		mp_Scene->simulate(deltaTime);
		mp_Scene->fetchResults(true);
	}

	void PhysXWrap::CreateStack(const px::PxTransform& t, px::PxU32 size, px::PxReal halfExtent) {

		px::PxShape* shape = mp_Physics->createShape(px::PxBoxGeometry(halfExtent, halfExtent, halfExtent), *mp_Material);

		for (px::PxU32 i = 0; i < size; i++)
		{
			for (px::PxU32 j = 0; j < size - i; j++)
			{
				px::PxTransform localTm(px::PxVec3(px::PxReal(j * 2) - px::PxReal(size - i), px::PxReal(i * 2 + 1), 0) * halfExtent);
				px::PxRigidDynamic* body = mp_Physics->createRigidDynamic(t.transform(localTm));
				body->attachShape(*shape);
				px::PxRigidBodyExt::updateMassAndInertia(*body, 10.0f);
				mp_Scene->addActor(*body);
			}
		}
		shape->release();
	}

	px::PxRigidDynamic* PhysXWrap::CreateDynamic(const px::PxTransform& t, const px::PxGeometry& geometry, const float& density, const px::PxVec3& velocity) {
		px::PxRigidDynamic* dynamic = PxCreateDynamic(*mp_Physics, t, geometry, *mp_Material, density);
		dynamic->setAngularDamping(0.1f);
		dynamic->setLinearVelocity(velocity);
		mp_Scene->addActor(*dynamic);

		return dynamic;
	}

	void PhysXWrap::AddActor(px::PxActor* dynamicActor) {
		mp_Scene->addActor(*dynamicActor);
	}

	void PhysXWrap::RemoveActor(px::PxActor* dynamicActor) {
		mp_Scene->removeActor(*dynamicActor);
	}

	px::PxPhysics*& PhysXWrap::GetPxPhysics() {
		return mp_Physics;
	}

	px::PxScene*& PhysXWrap::GetScene() {
		return mp_Scene;
	}

	px::PxCooking*& PhysXWrap::GetCooking() {
		return mp_Cooking;
	}

	void PhysXWrap::SetGravity(const Vec3f& force) {
		mp_Scene->setGravity(px::PxVec3(force.x, force.y, force.z));
	}

	void PhysXWrap::SetGravity(const float& x, const float& y, const float& z) {
		mp_Scene->setGravity(px::PxVec3(x, y, z));
	}
}