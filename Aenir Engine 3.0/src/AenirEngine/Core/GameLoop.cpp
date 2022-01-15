#include"PCH.h"
#include"GameLoop.h"

namespace Aen {

	GameLoop::GameLoop()
		:m_app(nullptr), m_renderer(nullptr) {}

	void GameLoop::Initialize() {

		if(!Input::Initialize())
			exit(-1);

		if(!GCore::Concealed::Initialize(m_app->m_window))
			exit(-1);

		t1 = chrono::high_resolution_clock::now();
		t2 = chrono::high_resolution_clock::now();
		ms_double = t2 - t1;

		// Initialize physX
		PhysicsHandler::Initialize(100, 981);
		
		m_renderer = AEN_NEW Renderer(m_app->m_window);
		m_renderer->Initialize();

		GlobalSettings::Initialize(m_app->m_window, m_renderer);
		GlobalSettings::SetVSync(true);

		m_app->Start();
	}
	
	void GameLoop::Run() {
		bool useVsync = GlobalSettings::GetVSync();

		double sStart = 0, deltaTime = 0;
		while (Aen::WindowHandle::HandleMsg()) {

			sStart = omp_get_wtime();
			if (m_app->m_window.IsActive()) {

				Input::Update();
				m_app->Update(static_cast<float>(deltaTime));
				ComponentHandler::UpdatePS(deltaTime);
			}
			PhysicsHandler::Update(static_cast<float>(deltaTime));
	
			m_renderer->Culling();

			t1 = chrono::high_resolution_clock::now();

			m_renderer->Render();

			if (Aen::Input::KeyDown(Aen::Key::T))
			{
				t2 = chrono::high_resolution_clock::now();
				ms_double = t2 - t1;
				std::string test = std::to_string(ms_double.count() / 1.f) + "\n";
				OutputDebugStringA(test.c_str());
			}

			deltaTime = (omp_get_wtime() - sStart);
		}
		
		delete m_app;
		Resource::Destroy();
		EntityHandler::Destroy();
		GCore::Concealed::Release();
		GlobalSettings::Destroy();
		PhysicsHandler::Destroy();
		delete m_renderer;
	}

	void GameLoop::InitApp(Aen::App* app)
	{
		m_app = app;
	}
}