#pragma once
#include"App.h"
#include<chrono>

//extern Aen::App* CreateApp();

namespace Aen {

	class AEN_DECLSPEC GameLoop {
		public:
		GameLoop();

		void Initialize();
		void Run();

		App* app;
		private:
		using ResClock = std::chrono::high_resolution_clock;
		using TimePoint = std::chrono::high_resolution_clock::time_point;
		using DurationLD = std::chrono::duration<long double>;

		TimePoint start;
		TimePoint end;
		DurationLD frameTime;
		DurationLD deltaTime;
	};
}