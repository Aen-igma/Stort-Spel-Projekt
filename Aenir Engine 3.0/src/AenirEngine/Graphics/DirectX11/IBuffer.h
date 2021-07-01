#pragma once
#include"DX11Core.h"

namespace Aen {

	class IBuffer : public GCore {
		public:
		IBuffer();

		void Create(DWORD* vId, const UINT& bufferSize);
		void BindBuffer();
		void DrawIndexed();
		const UINT GetBufferSize();

		private:
		ComBuffer m_buffer;
		UINT m_bufferSize;
	};
}
