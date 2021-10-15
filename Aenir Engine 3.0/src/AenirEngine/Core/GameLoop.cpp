#include"PCH.h"
#include"GameLoop.h"

namespace Aen {

	GameLoop::GameLoop()
		:m_app(nullptr), m_start(), m_end(), m_frameTime(), m_deltaTime(), m_renderer(nullptr) {}

	void GameLoop::Initialize() {
		//m_app = CreateApp();                       fix this
		int ft = (int)(((double)1 / (double)60) * (double)pow(10, 9));
		m_frameTime = std::chrono::nanoseconds{ft};

		if(!Input::Initialize())
			exit(-1);

		if(!GCore::Concealed::Initialize(m_app->m_window))
			exit(-1);

		// Initialize physX
		PhysXService::SetInstance(&m_PhysX);
		PhysXService::GetInstance()->InitPhysics(100, 981);

		// Setup First Quad in quadtree
		m_WorldBox = DirectX::BoundingBox(DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3(25.f, 25.f, 25.f));
		//Create Quadtree with new variables
		m_Quadtree = new Quadtree(m_WorldBox, 0, 3, 4);
		//for-loop going through all static objects in the world and inserting them one by one
		for (int i = 0; i < 10; i++)
		{
			DirectX::BoundingBox* tempBox = new DirectX::BoundingBox(DirectX::XMFLOAT3(1.f + i * 4, 1.f, 0),
				DirectX::XMFLOAT3(1.f, 1.f, 1.f));
			m_Quadtree->insertObject(i, tempBox);
		}
		m_PlayerCenter = DirectX::XMFLOAT3(10, 10, 0);
		m_PlayerSizes = DirectX::XMFLOAT3(1, 1, 1);
		m_PlayerBox = DirectX::BoundingBox(m_PlayerCenter, m_PlayerSizes);

		// float3 origin, float4 orientation, float rightslope, float leftslope, float topslope, float bottomslope, float near, float far
		//m_CameraFrustrum = DirectX::BoundingFrustum(GlobalSettings::GetMainCamera()->GetComponent<Camera>().GetProjecton().smMat);


		GlobalSettings::Initialize(m_app->m_window);

		m_renderer = AEN_NEW Renderer(m_app->m_window);
		m_renderer->Initialize();

		m_app->Start();
	}

	void GameLoop::Run() {
		
		m_start = m_end = ResClock::now();
		while(Aen::WindowHandle::HandleMsg()) {
		
			m_end = ResClock::now();
			while(std::chrono::duration_cast<std::chrono::nanoseconds>(m_end - m_start) > m_frameTime) {
				m_deltaTime = std::chrono::duration_cast<std::chrono::nanoseconds>(m_end - m_start);
				m_start = ResClock::now();

				if(m_app->m_window.IsActive()) {
					Input::Update();
					m_app->Update(static_cast<float>(m_deltaTime.count()));
				}

				if (GlobalSettings::GetMainCamera())
				{
					m_CameraFrustrum = DirectX::BoundingFrustum(GlobalSettings::GetMainCamera()->GetComponent<Camera>().GetProjecton().smMat);
					m_Quadtree->getRoot()->intersectTest(m_CameraFrustrum, m_ObjectsToRender);
					//if (Input::KeyDown(Key::B))
					//{
					//	// when updating to new math library, replace this with .smvec
					///*	m_PlayerCenter = DirectX::XMFLOAT3(GlobalSettings::GetMainCamera()->GetPos().x,
					//		GlobalSettings::GetMainCamera()->GetPos().y,
					//		GlobalSettings::GetMainCamera()->GetPos().z);
					//	m_PlayerBox = DirectX::BoundingBox(m_PlayerCenter, m_PlayerSizes);*/
		
					//	m_PlayerBox = DirectX::BoundingBox(Aen::GlobalSettings::GetMainCamera()->GetPos().smVec,
					//		DirectX::XMFLOAT3(1.f, 1.f, 1.f));
					//	m_Quadtree->getRoot()->inside(m_PlayerBox);
					//}

					if (Input::KeyDown(Key::V))
					{
						//m_Quadtree->getRoot()->intersectTest(m_CameraFrustrum, m_ObjectsToRender);
						std::cout << "Objects to render: ";
						for (auto& b : m_ObjectsToRender)
							std::cout << b << ", \n";
					}
					//if (Input::KeyDown(Key::C))
					//{
					//	std::cout << "Camera Position: " << GlobalSettings::GetMainCamera()->GetPos().x << ", " <<
					//		GlobalSettings::GetMainCamera()->GetPos().y << ", " <<
					//		GlobalSettings::GetMainCamera()->GetPos().z << std::endl;
					//}

				}


				PhysXService::GetInstance()->RunPhysics(m_deltaTime.count());
				m_renderer->Render(); // VSync
			}
		}

		// Destroy imGui
		delete Aen::GlobalSettings::GetImGuiHandler();

		PhysXService::GetInstance()->ClosePhysics();
		
		Resource::Destroy();
		GCore::Concealed::Release();
		delete m_app;
		delete m_renderer;
	}
}