#pragma once
#include"..\..\AenWindow.h"
#include"..\..\AenMath.h"
#include"DX11Type.h"



namespace Aen {

	class GCore {
		public:

		struct Concealed {
			static bool Initialize(const Window& window);
		};

		private:
		static std::vector<ComAdapter1> m_adapters;
		static std::vector<DXGI_ADAPTER_DESC1> m_adapterDesc;

		protected:
		static ComDevice m_device;
		static ComDeviceContext m_dContext;
		static ComSwapChain m_sChain;
	};
}