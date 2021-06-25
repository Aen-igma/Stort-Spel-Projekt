#include"GameLoop.h"

namespace Aen {

	GameLoop::GameLoop()
		:app(nullptr) {}

	void GameLoop::Initialize() {
		int ft = (int)(((double)1 / (double)60) * (double)pow(10, 9));
		frameTime = std::chrono::nanoseconds{ft};


		app->Start();
	}

	void GameLoop::Run() {
		
		start = end = ResClock::now();
		while(Aen::WindowHandle::HandleMsg()) {
		
			end = ResClock::now();
			while(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start) > frameTime) {
				deltaTime = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
				start = ResClock::now();
				Aen::Input::Update();
				app->Update(static_cast<float>(deltaTime.count()));
			}
		
		
		}

		delete app;
	}


}