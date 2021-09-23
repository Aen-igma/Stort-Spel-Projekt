#include"PCH.h"
//#include"PhysXWrap.h"
#include"ThirdParty\PhysX\PhysXWrap.h"

PhysXWrap::PhysXWrap()
{
	this->m_Dispatcher = NULL;
	this->m_Foundation = NULL;
	this->m_Material =   NULL;
	this->m_Scene =		 NULL;
	this->m_Pvd =		 NULL;
	this->m_Physics =    NULL;
	this->m_Cooking =    NULL;
	this->stackZ =		 10.0f;
}

PhysXWrap::~PhysXWrap()
{
	this->m_Dispatcher = NULL;
	this->m_Foundation = NULL;
	this->m_Material = NULL;
	this->m_Scene = NULL;
	this->m_Pvd = NULL;
	this->m_Physics = NULL;
	this->m_Cooking = NULL;
}

void PhysXWrap::CreateStack(const PxTransform& t, PxU32 size, PxReal halfExtent)
{
	
PxShape* shape = m_Physics->createShape(PxBoxGeometry(halfExtent, halfExtent, halfExtent), *m_Material);
	//PxShape* shape = m_Physics->createShape(PxSphereGeometry(2), *m_Material);
	for (PxU32 i = 0; i < size; i++)
	{
		for (PxU32 j = 0; j < size - i; j++)
		{
			PxTransform localTm(PxVec3(PxReal(j * 2) - PxReal(size - i), PxReal(i * 2 + 1), 0) * halfExtent);
			PxRigidDynamic* body = m_Physics->createRigidDynamic(t.transform(localTm));
			body->attachShape(*shape);
			PxRigidBodyExt::updateMassAndInertia(*body, 10.0f);
			m_Scene->addActor(*body);
		}
	}
	shape->release();
}

PxRigidDynamic* PhysXWrap::CreateDynamic(const PxTransform& t, const PxGeometry& geometry, const float& density, const PxVec3& velocity)
{
	PxRigidDynamic* dynamic = PxCreateDynamic(*m_Physics, t, geometry, *m_Material, density);
	dynamic->setAngularDamping(0.5f);
	dynamic->setLinearVelocity(velocity);
	m_Scene->addActor(*dynamic);

	return dynamic;
}

void PhysXWrap::AddActor(PxRigidStatic* staticActor)
{
	m_Scene->addActor(*staticActor);
}

void PhysXWrap::AddActor(PxRigidDynamic* dynamicActor)
{
	m_Scene->addActor(*dynamicActor);
}

PxPhysics* PhysXWrap::GetPxPhysics() const
{
	return this->m_Physics;
}

void PhysXWrap::InitPhysics(int toleranceLength, int toleranceSpeed)
{
	m_Foundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_DefaultAllocatorCallback, m_DefaultErrorCallback);
	if (!m_Foundation) throw("PxCreateFoundation Failed!");

	m_Pvd = PxCreatePvd(*m_Foundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
	m_Pvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

	m_ToleranceScale.length = toleranceLength;
	m_ToleranceScale.speed = toleranceSpeed;
	m_Physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_Foundation, m_ToleranceScale, true, m_Pvd);
	if (!m_Physics) throw("PxCreatePhysics Failed!");

	m_Cooking = PxCreateCooking(PX_PHYSICS_VERSION, *m_Foundation, PxCookingParams(m_ToleranceScale));
	if (!m_Cooking) throw("PxCreateCooking Failed!");

	PxSceneDesc sceneDesc(m_Physics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -30.f, 0.0f);
	m_Dispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = m_Dispatcher;
	sceneDesc.filterShader = PxDefaultSimulationFilterShader;
	m_Scene = m_Physics->createScene(sceneDesc);	

	PxPvdSceneClient* pvdClient = m_Scene->getScenePvdClient();
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
	
	m_Scene->release();
	m_Dispatcher->release();
	m_Physics->release();
	m_Cooking->release();

	if (m_Pvd)
	{
		physx::PxPvdTransport* transport = m_Pvd->getTransport();
		m_Pvd->release();
		m_Pvd = NULL;
		transport->release();
	}
	m_Foundation->release(); // Always release last
	
	printf("GoodbyeWorld.\n");
}

void PhysXWrap::RunPhysics(const float& deltaTime)
{
		m_Scene->simulate(deltaTime);
		m_Scene->fetchResults(true);
}