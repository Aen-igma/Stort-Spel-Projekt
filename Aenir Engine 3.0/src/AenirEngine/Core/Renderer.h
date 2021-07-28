#pragma once
#include"../Graphics/RenderSystem.h"
#include"../Graphics/Component/Entity.h"

namespace Aen {

	struct CB_Transform {
		Mat4f m_vMat;
		Mat4f m_pMat;
		Mat4f m_mdlMat;
	};

	class Renderer {
		public:
		Renderer() = delete;
		Renderer(Window& window);

		friend class GameLoop;

		private:
		void Initialize();
		void Draw();

		Window& m_window;

		GBuffer m_gBuffer;
		VShader m_postVS;
		PShader m_postPS;
		ScreenQuad m_screenQuad;
		ILayout m_postLayout;
		Sampler m_borderSampler;

		CBuffer<CB_Transform> m_cbTransform;
		CBuffer<uint32_t> m_cbLightCount;
		CBuffer<Vec3f> m_cbCamera;
		CBuffer<Vec4i> m_cbUseTexture;
		SBuffer<SB_Light> m_sbLight;

		BBuffer m_backBuffer;
		
		D3D11_VIEWPORT m_viewPort;
		Depth m_depth;
		Stencil m_writeStencil;
		Stencil m_maskStencil;
		RState m_rasterizerState;
	};
}