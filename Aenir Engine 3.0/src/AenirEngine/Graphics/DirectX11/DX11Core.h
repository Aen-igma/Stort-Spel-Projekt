#pragma once
#include"..\..\AenDefines.h"
#include"..\..\AenWindow.h"
#include"..\..\BuritoMath.h"
#include"DX11Type.h"
#include"..\..\ThirdParty\SimpleMath\SimpleMath.h"
#include <assert.h>
#include <dxgidebug.h>

#ifdef _DEBUG
#define ASSERT_HR(hr) assert(SUCCEEDED(hr))
#else
#define ASSERT_HR(hr) hr
#endif

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
		static Com2DFactory m_factory;
		static Com2DTarget m_target2D;
	};
}