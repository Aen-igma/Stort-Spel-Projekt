#pragma once
#include"Shader.h"

namespace Aen {

	struct AEN_DECLSPEC ShaderResource : public GCore {
		public:
		ShaderResource();

		void LoadResource(const std::string& dir);

		template<class T>
		void BindSRV(const UINT& slot);
		
		friend class RenderSystem;

		private:
		ComShaderResourceView m_srv;
	};

	template<>
	inline void ShaderResource::BindSRV<VShader>(const UINT& slot) {
		m_dContext->VSSetShaderResources(slot, 1, m_srv.GetAddressOf());
	}

	template<>
	inline void ShaderResource::BindSRV<HShader>(const UINT& slot) {
		m_dContext->HSSetShaderResources(slot, 1, m_srv.GetAddressOf());
	}

	template<>
	inline void ShaderResource::BindSRV<CShader>(const UINT& slot) {
		m_dContext->CSSetShaderResources(slot, 1, m_srv.GetAddressOf());
	}

	template<>
	inline void ShaderResource::BindSRV<DShader>(const UINT& slot) {
		m_dContext->DSSetShaderResources(slot, 1, m_srv.GetAddressOf());
	}

	template<>
	inline void ShaderResource::BindSRV<GShader>(const UINT& slot) {
		m_dContext->GSSetShaderResources(slot, 1, m_srv.GetAddressOf());
	}

	template<>
	inline void ShaderResource::BindSRV<PShader>(const UINT& slot) {
		m_dContext->PSSetShaderResources(slot, 1, m_srv.GetAddressOf());
	}
}
