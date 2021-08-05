#pragma once
#include"../Graphics/RenderSystem.h"
#include"../Graphics/Component/Entity.h"

namespace Aen {

	struct CB_Transform {
		Mat4f m_vMat;
		Mat4f m_pMat;
		Mat4f m_mdlMat;
	};

	struct VB_Camera {
		Vec3f pos;
		float pad;
		Vec3f fDir;
		float pad1;
		Vec3f uDir;
		float pad2;
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
		
		ScreenQuad m_screenQuad;

		CBuffer<CB_Transform> m_cbTransform;
		CBuffer<uint32_t> m_cbLightCount;
		CBuffer<VB_Camera> m_cbCamera;
		CBuffer<Vec4i> m_cbUseTexture;
		SBuffer<SB_Light> m_sbLight;

		GBuffer m_postProcessBuffer;
		BBuffer m_backBuffer;
		
		VShader m_postProcessVS;
		PShader m_postProcessPS;
		Sampler m_clampSampler;
		ILayout m_postLayout;

		D3D11_VIEWPORT m_viewPort;
		Depth m_depth;
		Stencil m_writeStencil;
		Stencil m_maskStencil;
		Stencil m_offStencil;
		RState m_rasterizerState;
	};
}