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

		GlobalSettings::Initialize(m_app->m_window);

		m_renderer = AEN_NEW Renderer(m_app->m_window);
		m_renderer->Initialize();

		// imGui initialize
		Aen::GlobalSettings::getImGuiHandler()->initialize(m_app->m_window.GetWHND(), GCore::m_device.Get(), GCore::m_dContext.Get());

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

				m_renderer->Render(); // VSync
			}
		}

		// Destroy imGui
		Aen::GlobalSettings::getImGuiHandler()->release();
		delete Aen::GlobalSettings::getImGuiHandler();
		
		Resource::Destroy();
		GCore::Concealed::Release();
		delete m_app;
		delete m_renderer;
	}
}