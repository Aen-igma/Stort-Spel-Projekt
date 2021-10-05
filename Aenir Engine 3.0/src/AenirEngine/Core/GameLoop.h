#pragma once
#include"App.h"
#include<chrono>
#include "QuadTree/Quadtree.h"
#include "ThirdParty/ImGui/imgui.h"
#include "ThirdParty/ImGui/imgui_impl_win32.h"
#include "ThirdParty/ImGui/imgui_impl_dx11.h"

namespace Aen {

	extern App* CreateApp();

	class AEN_DECLSPEC GameLoop {
		public:
		GameLoop();

		void Initialize();
		void Run();

		/*Quadtree* m_Center;*/
		Quadtree* m_Center;
		Object* object;
		AABB* AxisAlignedBoundBox;

		int cap = 10;
		float m_left;
		float m_right;
		float m_top;
		float m_bot;

		App* m_app;
		private:
		using ResClock = std::chrono::high_resolution_clock;
		using TimePoint = std::chrono::high_resolution_clock::time_point;
		using DurationLD = std::chrono::duration<long double>;
		
		TimePoint m_start;
		TimePoint m_end;
		DurationLD m_frameTime;
		DurationLD m_deltaTime;

		Renderer* m_renderer;
	};
}