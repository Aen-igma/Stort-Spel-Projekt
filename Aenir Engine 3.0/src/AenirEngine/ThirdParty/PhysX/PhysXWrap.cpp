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

void PhysXWrap::createStack(const PxTransform& t, PxU32 size, PxReal halfExtent)
{
	
	PxShape* shape = m_Physics->createShape(PxBoxGeometry(halfExtent, halfExtent, halfExtent), *m_Material);
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

PxRigidDynamic* PhysXWrap::createDynamic(const PxTransform& t, const PxGeometry& geometry, const PxVec3& velocity)
{
	PxRigidDynamic* dynamic = PxCreateDynamic(*m_Physics, t, geometry, *m_Material, 10.0f);
	dynamic->setAngularDamping(0.5f);
	dynamic->setLinearVelocity(velocity);
	m_Scene->addActor(*dynamic);

	return dynamic;
}

void PhysXWrap::initPhysics()
{
	m_Foundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_DefaultAllocatorCallback, m_DefaultErrorCallback);
	if (!m_Foundation) throw("PxCreateFoundation Failed!");

	m_Pvd = PxCreatePvd(*m_Foundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
	m_Pvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

	m_ToleranceScale.length = 100;
	m_ToleranceScale.speed = 981;
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

	//if (!PxInitExtensions(*m_Physics, m_Pvd)) throw ("PxInitExtensions Failed!");
		

	PxPvdSceneClient* pvdClient = m_Scene->getScenePvdClient();
	if (pvdClient)
	{
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}
	m_Material = m_Physics->createMaterial(0.5f, 0.5f, 0.6f);

	PxRigidStatic* groundPlane = PxCreatePlane(*m_Physics, PxPlane(0, 1, 0, 0), *m_Material);
	m_Scene->addActor(*groundPlane);

	createStack(PxTransform(PxVec3(0, 1, stackZ -= 10.0f)), 10, 2.0f);
	createDynamic(PxTransform(PxVec3(0, 40, 100)), PxSphereGeometry(10), PxVec3(0, -50, -100));
}


void PhysXWrap::closePhysics()
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

void PhysXWrap::runPhysics()
{
		m_Scene->simulate(1.0f / 60.0f);
		m_Scene->fetchResults(true);
}