#pragma once
#include"..\..\AenDefines.h"
#include"..\..\AenWindow.h"
#include"..\..\AenMath.h"
#include"DX11Type.h"
#include"..\..\ThirdParty\SimpleMath\SimpleMath.h"

namespace sm = DirectX::SimpleMath;

namespace Aen {

	class GCore {
		private:
			
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