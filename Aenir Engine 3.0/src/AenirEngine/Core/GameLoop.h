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

		//Quadtree
		Quadtree* m_Quadtree;
		AABB* m_AxisAlignedBoundBox;
		Object* m_Object;


		int m_quadCap = 10;
		float m_minX;
		float m_minY;
		float m_maxX;
		float m_maxY;

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