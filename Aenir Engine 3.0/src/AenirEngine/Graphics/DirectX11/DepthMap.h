#pragma once
#include"Shader.h"

namespace Aen {

	class DepthMap : public GCore {
		public:
		DepthMap() = delete;
		DepthMap(const UINT& dimensions);
		
		template<class T>
		void SetSRV(const UINT& slot);

		void SetViewPort();
		void SetDSView();

		private:
		ComDepthStencilView m_dsView;
		ComShaderResourceView m_srv;
		D3D11_VIEWPORT m_viewPort;
	};

	template<>
	inline void DepthMap::SetSRV<VShader>(const UINT& slot) {
		m_dContext->VSSetShaderResources(slot, 1, m_srv.GetAddressOf());
	}

	template<>
	inline void DepthMap::SetSRV<HShader>(const UINT& slot) {
		m_dContext->HSSetShaderResources(slot, 1, m_srv.GetAddressOf());
	}

	template<>
	inline void DepthMap::SetSRV<CShader>(const UINT& slot) {
		m_dContext->CSSetShaderResources(slot, 1, m_srv.GetAddressOf());
	}

	template<>
	inline void DepthMap::SetSRV<DShader>(const UINT& slot) {
		m_dContext->DSSetShaderResources(slot, 1, m_srv.GetAddressOf());
	}

	template<>
	inline void DepthMap::SetSRV<GShader>(const UINT& slot) {
		m_dContext->GSSetShaderResources(slot, 1, m_srv.GetAddressOf());
	}

	template<>
	inline void DepthMap::SetSRV<PShader>(const UINT& slot) {
		m_dContext->PSSetShaderResources(slot, 1, m_srv.GetAddressOf());
	}
}