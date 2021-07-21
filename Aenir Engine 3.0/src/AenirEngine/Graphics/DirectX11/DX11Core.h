#pragma once
#include"..\..\AenDefines.h"
#include"..\..\AenWindow.h"
#include"..\..\AenMath.h"
#include"DX11Type.h"


namespace Aen {

	class GCore {
		public:

		struct Concealed {
			static bool Initialize(const Window& window);
			static void Release();
		};

		friend class GameLoop;

		protected:
		static ComDevice m_device;
		static ComDeviceContext m_dContext;
		static ComSwapChain m_sChain;
	};
}