#pragma once
#include"../Graphics/RenderSystem.h"
#include"../Graphics/Component/Entity.h"

namespace Aen {

	struct CB_Transform {
		Mat4f m_vpMat;
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

		CBuffer<CB_Transform> m_cbTransform;
		CBuffer<Vec3i> m_cbLightCount;
		SBuffer<SB_SpotLight> m_sbSpotLight;
		SBuffer<SB_PointLight> m_sbPointLight;
		SBuffer<SB_DirectionalLight> m_sbDirectionalLight;

		BBuffer m_backBuffer;
		
		D3D11_VIEWPORT m_viewPort;
		DepthStencil m_depthStencil;
		RState m_rasterizerState;
	};
}