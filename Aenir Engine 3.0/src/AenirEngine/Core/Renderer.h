#pragma once
#include"../Graphics/RenderSystem.h"
#include"../Graphics/Component/EntityHandler.h"
#include<thread>

namespace Aen {

	struct CB_Collision
	{
		Vec3f color;
		int switcher;
	};

	struct CB_DispatchInfo {
		Vec2i threadGroups;
		Vec2i numThreads;
		Vec2i windowSize;
		uint32_t avarageLights;
		uint32_t pad;
	};

	struct CB_Transform {
		Mat4f m_vMat;
		Mat4f m_pMat;
		Mat4f m_mdlMat;

		CB_Transform() :m_vMat(Mat4f::identity), m_pMat(Mat4f::identity), m_mdlMat(Mat4f::identity) {}
	};

	struct CB_Camera {
		Vec3f pos;
		float pad;
		Vec3f fDir;
		float pad1;
		Vec3f uDir;
		float pad2;

		CB_Camera() :pos(), pad(0.f), fDir(), pad1(0.f), uDir(), pad2(0.f) {}
	};

	class Renderer {
		public:
		Renderer() = delete;
		Renderer(Window& window);

		friend class GameLoop;
		friend class MeshInstance;
		friend class AABoundBox;
		friend class OBBox;
		friend class Animator;

		private:
		bool m_toggleView;

		void Initialize();
		void Render();

		Window& m_window;
		
		ScreenQuad m_screenQuad;

		CBuffer<Color> m_cbBGColor;
		CBuffer<CB_Transform> m_cbTransform;
		CBuffer<uint32_t> m_cbLightCount;
		CBuffer<CB_Camera> m_cbCamera;
		CBuffer<Vec4i> m_cbUseTexture;
		SBuffer<SB_Light> m_sbLight;
		
		CShader m_postProcessCS;
		Sampler m_wrapSampler;

		Vec2i m_dispatchGroups;
		VShader m_opaqueVS;
		PShader m_collisionPS;
		UAView m_UAVBackBuffer;
		RWTexture2D m_UAVFinal;
		BBuffer m_backBuffer;
		ILayout m_opaqueLayout;

		D3D11_VIEWPORT m_viewPort;
		DepthMap m_depthMap;
		Stencil m_writeStencil;
		Stencil m_maskStencil;
		Stencil m_offStencil;
		RState m_rasterizerState;
		RState m_wireFrameState;

		CBuffer<CB_DispatchInfo> m_dispatchInfo;
		CBuffer<uint32_t> m_heatMap;
		CShader m_lightCullCS;
		UAView m_lIndex;
		RWTexture2D m_lGrid;
		Vec2i m_dispatchCall;
		const uint32_t m_avarageLights;

		CBuffer<CB_Collision> m_collisionBuffer;
	};
}