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
		CBuffer<Vec3i> m_cbLightCount;
		CBuffer<Vec3f> m_cbCamera;
		CBuffer<Vec4i> m_cbUseTexture;
		SBuffer<SB_SpotLight> m_sbSpotLight;
		SBuffer<SB_PointLight> m_sbPointLight;
		SBuffer<SB_DirectionalLight> m_sbDirectionalLight;

		BBuffer m_backBuffer;
		
		D3D11_VIEWPORT m_viewPort;
		Depth m_depth;
		Stencil m_writeStencil;
		Stencil m_maskStencil;
		RState m_rasterizerState;
	};
}