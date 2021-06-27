#pragma once
#include"DX11Core.h"


namespace Aen {
	struct IBuffer : public GCore {
		ComBuffer buffer;
		UINT iSize;

		~IBuffer() = default;

		bool Initialize(DWORD* vId, UINT iCount);
	};
}
