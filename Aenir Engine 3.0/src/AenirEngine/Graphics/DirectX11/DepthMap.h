#pragma once
#include"Shader.h"

namespace Aen {

	class DepthMap : public GCore {
		public:
		DepthMap() = delete;
		DepthMap(const UINT& dimensions);
		
		template<class T>
		void BindSRV(const UINT& slot);

		void SetViewPort();
		void SetDSView();

		friend class RenderSystem;

		private:
		ComDepthStencilView m_dsView;
		ComShaderResourceView m_srv;
		D3D11_VIEWPORT m_viewPort;
	};

	template<>
	inline void DepthMap::BindSRV<VShader>(const UINT& slot) {
		m_dContext->VSSetShaderResources(slot, 1, m_srv.GetAddressOf());
	}

	template<>
	inline void DepthMap::BindSRV<HShader>(const UINT& slot) {
		m_dContext->HSSetShaderResources(slot, 1, m_srv.GetAddressOf());
	}

	template<>
	inline void DepthMap::BindSRV<CShader>(const UINT& slot) {
		m_dContext->CSSetShaderResources(slot, 1, m_srv.GetAddressOf());
	}

	template<>
	inline void DepthMap::BindSRV<DShader>(const UINT& slot) {
		m_dContext->DSSetShaderResources(slot, 1, m_srv.GetAddressOf());
	}

	template<>
	inline void DepthMap::BindSRV<GShader>(const UINT& slot) {
		m_dContext->GSSetShaderResources(slot, 1, m_srv.GetAddressOf());
	}

	template<>
	inline void DepthMap::BindSRV<PShader>(const UINT& slot) {
		m_dContext->PSSetShaderResources(slot, 1, m_srv.GetAddressOf());
	}
}