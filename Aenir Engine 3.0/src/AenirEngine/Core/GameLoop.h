#pragma once
#include"App.h"
#include<chrono>
#include "QuadTree\Quadtree.h"
#include "ThirdParty\fmod\fmod.hpp"
#include "ThirdParty\fmod\fmod_errors.h"
#include "ThirdParty\PhysX\PhysXService.h"

namespace Aen {

	extern App* CreateApp();

	class AEN_DECLSPEC GameLoop {
		public:
		GameLoop();

		void Initialize();
		void Run();

		App* m_app;
		private:
		using ResClock = std::chrono::high_resolution_clock;
		using TimePoint = std::chrono::high_resolution_clock::time_point;
		using DurationLD = std::chrono::duration<long double>;

		//Quadtree
		Quadtree* m_Quadtree;
		DirectX::BoundingBox m_WorldBox;
		DirectX::BoundingBox m_PlayerBox;
		DirectX::XMFLOAT3 m_PlayerCenter;
		DirectX::XMFLOAT3 m_PlayerSizes;

		//Camera frustrum for view frustrum culling
		DirectX::BoundingFrustum m_CameraFrustrum;
		std::vector<int> m_ObjectsToRender;

		
		TimePoint m_start;
		TimePoint m_end;
		DurationLD m_frameTime;
		DurationLD m_deltaTime;

		Renderer* m_renderer;
		PhysXWrap m_PhysX;
	};
}