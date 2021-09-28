#include"PCH.h"
//#include"PhysXWrap.h"
#include"ThirdParty\PhysX\PhysXWrap.h"

PhysXWrap::PhysXWrap()
{
	this->mp_Dispatcher = NULL;
	this->mp_Foundation = NULL;
	this->mp_Material =   NULL;
	this->mp_Scene =		 NULL;
	this->mp_Pvd =		 NULL;
	this->mp_Physics =    NULL;
	this->mp_Cooking =    NULL;
	this->m_StackZ =		 9.0f;
}

PhysXWrap::~PhysXWrap()
{
	this->mp_Dispatcher = NULL;
	this->mp_Foundation = NULL;
	this->mp_Material = NULL;
	this->mp_Scene = NULL;
	this->mp_Pvd = NULL;
	this->mp_Physics = NULL;
	this->mp_Cooking = NULL;
}

void PhysXWrap::CreateStack(const PxTransform& t, PxU32 size, PxReal halfExtent)
{
	
PxShape* shape = mp_Physics->createShape(PxBoxGeometry(halfExtent, halfExtent, halfExtent), *mp_Material);
	//PxShape* shape = m_Physics->createShape(PxSphereGeometry(2), *m_Material);
	for (PxU32 i = 0; i < size; i++)
	{
		for (PxU32 j = 0; j < size - i; j++)
		{
			PxTransform localTm(PxVec3(PxReal(j * 2) - PxReal(size - i), PxReal(i * 2 + 1), 0) * halfExtent);
			PxRigidDynamic* body = mp_Physics->createRigidDynamic(t.transform(localTm));
			body->attachShape(*shape);
			PxRigidBodyExt::updateMassAndInertia(*body, 10.0f);
			mp_Scene->addActor(*body);
		}
	}
	shape->release();
}

PxRigidDynamic* PhysXWrap::CreateDynamic(const PxTransform& t, const PxGeometry& geometry, const float& density, const PxVec3& velocity)
{
	PxRigidDynamic* dynamic = PxCreateDynamic(*mp_Physics, t, geometry, *mp_Material, density);
	dynamic->setAngularDamping(0.1f);
	dynamic->setLinearVelocity(velocity);
	mp_Scene->addActor(*dynamic);

	return dynamic;
}

void PhysXWrap::CreateDemo()
{
	mp_Material = mp_Physics->createMaterial(0.5f, 0.5f, 0.6f);

	PxRigidStatic* groundPlane = PxCreatePlane(*mp_Physics, PxPlane(0, 1, 0, 0), *mp_Material);
	mp_Scene->addActor(*groundPlane);

	CreateStack(PxTransform(PxVec3(0, 0, m_StackZ -= 10.0f)), 10, 2.0f);
	CreateDynamic(PxTransform(PxVec3(0, 40, 120)), PxBoxGeometry(5.f, 5.f, 5.f), 20.f, PxVec3(0, -10, -100));
	CreateStack(PxTransform(PxVec3(100, 0, m_StackZ -= 10.0f)), 10, 2.0f);
	CreateDynamic(PxTransform(PxVec3(100, 40, 120)), PxSphereGeometry(10), 20.f, PxVec3(0, -10, -100));
}

void PhysXWrap::AddActor(PxRigidStatic* staticActor)
{
	mp_Scene->addActor(*staticActor);
}

void PhysXWrap::AddActor(PxRigidDynamic* dynamicActor)
{
	mp_Scene->addActor(*dynamicActor);
}

PxPhysics*& PhysXWrap::GetPxPhysics()
{
	return this->mp_Physics;
}


void PhysXWrap::InitPhysics(int toleranceLength, int toleranceSpeed)
{
	mp_Foundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_DefaultAllocatorCallback, m_DefaultErrorCallback);
	if (!mp_Foundation) throw("PxCreateFoundation Failed!");

	mp_Pvd = PxCreatePvd(*mp_Foundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
	mp_Pvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

	m_ToleranceScale.length = toleranceLength;
	m_ToleranceScale.speed = toleranceSpeed;
	mp_Physics = PxCreatePhysics(PX_PHYSICS_VERSION, *mp_Foundation, m_ToleranceScale, true, mp_Pvd);
	if (!mp_Physics) throw("PxCreatePhysics Failed!");

	mp_Cooking = PxCreateCooking(PX_PHYSICS_VERSION, *mp_Foundation, PxCookingParams(m_ToleranceScale));
	if (!mp_Cooking) throw("PxCreateCooking Failed!");

	PxSceneDesc sceneDesc(mp_Physics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -30.f, 0.0f);
	mp_Dispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = mp_Dispatcher;
	sceneDesc.filterShader = PxDefaultSimulationFilterShader;
	mp_Scene = mp_Physics->createScene(sceneDesc);	

	PxPvdSceneClient* pvdClient = mp_Scene->getScenePvdClient();
	if (pvdClient)
	{
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}
	//m_Material = m_Physics->createMaterial(0.5f, 0.5f, 0.6f);

	//PxRigidStatic* groundPlane = PxCreatePlane(*m_Physics, PxPlane(0, 1, 0, 0), *m_Material);
	//m_Scene->addActor(*groundPlane);

	//CreateStack(PxTransform(PxVec3(0, 0, stackZ -= 10.0f)), 10, 2.0f);
	/*CreateDynamic(PxTransform(PxVec3(0, 40, 100)), PxBoxGeometry(5.f, 3.f, 3.f), 50.f, PxVec3(0, -10, -100));
	CreateStack(PxTransform(PxVec3(100, 0, stackZ -= 10.0f)), 10, 2.0f);
	CreateDynamic(PxTransform(PxVec3(100, 40, 100)), PxSphereGeometry(10), 20.f, PxVec3(0, -30, -100));*/
}


void PhysXWrap::ClosePhysics()
{
	
	mp_Scene->release();
	mp_Dispatcher->release();
	mp_Physics->release();
	mp_Cooking->release();

	if (mp_Pvd)
	{
		physx::PxPvdTransport* transport = mp_Pvd->getTransport();
		mp_Pvd->release();
		mp_Pvd = NULL;
		transport->release();
	}
	mp_Foundation->release(); // Always release last
	
	printf("GoodbyeWorld.\n");
}

void PhysXWrap::RunPhysics(const float& deltaTime)
{
		mp_Scene->simulate(deltaTime);
		mp_Scene->fetchResults(true);
}