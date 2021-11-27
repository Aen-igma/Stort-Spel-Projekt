#pragma once
#include"App.h"
#include<omp.h>
#include "ThirdParty\fmod\fmod.hpp"
#include "ThirdParty\fmod\fmod_errors.h"

namespace Aen {

	extern App* CreateApp();

	class AEN_DECLSPEC GameLoop {
		public:
		GameLoop();

		void Initialize();
		void Run();

		App* m_app;
		private:
		//using ResClock = std::chrono::high_resolution_clock;
		//using TimePoint = std::chrono::high_resolution_clock::time_point;
		using DurationLD = std::chrono::duration<long double>;
		
		//TimePoint m_start;
		//TimePoint m_end;
		//DurationLD m_frameTime;
		//DurationLD m_deltaTime;
			double m_frametime;

		Renderer* m_renderer;
	};
}