#pragma once
#include"Graphics.h"

namespace Aen {

    #define DEF_SHADER\
            X(VS, VShader)\
            X(HS, HShader)\
            X(CS, CShader)\
            X(DS, DShader)\
            X(GS, GShader)\
            X(PS, PShader)\

    enum class Topology {
        UNDEFINED = 0,
        POINTLIST = 1,
        LINELIST = 2,
        LINESTRIP = 3,
        TRIANGLELIST = 4,
        TRIANGLESTRIP = 5,
        LINELIST_ADJ = 10,
        LINESTRIP_ADJ = 11,
        TRIANGLELIST_ADJ = 12,
        TRIANGLESTRIP_ADJ = 13,
        CONTROL_POINT_PATCHLIST_1 = 33,
        CONTROL_POINT_PATCHLIST_2 = 34,
        CONTROL_POINT_PATCHLIST_3 = 35,
        CONTROL_POINT_PATCHLIST_4 = 36,
        CONTROL_POINT_PATCHLIST_5 = 37,
        CONTROL_POINT_PATCHLIST_6 = 38,
        CONTROL_POINT_PATCHLIST_7 = 39,
        CONTROL_POINT_PATCHLIST_8 = 40,
        CONTROL_POINT_PATCHLIST_9 = 41,
        CONTROL_POINT_PATCHLIST_10 = 42,
        CONTROL_POINT_PATCHLIST_11 = 43,
        CONTROL_POINT_PATCHLIST_12 = 44,
        CONTROL_POINT_PATCHLIST_13 = 45,
        CONTROL_POINT_PATCHLIST_14 = 46,
        CONTROL_POINT_PATCHLIST_15 = 47,
        CONTROL_POINT_PATCHLIST_16 = 48,
        CONTROL_POINT_PATCHLIST_17 = 49,
        CONTROL_POINT_PATCHLIST_18 = 50,
        CONTROL_POINT_PATCHLIST_19 = 51,
        CONTROL_POINT_PATCHLIST_20 = 52,
        CONTROL_POINT_PATCHLIST_21 = 53,
        CONTROL_POINT_PATCHLIST_22 = 54,
        CONTROL_POINT_PATCHLIST_23 = 55,
        CONTROL_POINT_PATCHLIST_24 = 56,
        CONTROL_POINT_PATCHLIST_25 = 57,
        CONTROL_POINT_PATCHLIST_26 = 58,
        CONTROL_POINT_PATCHLIST_27 = 59,
        CONTROL_POINT_PATCHLIST_28 = 60,
        CONTROL_POINT_PATCHLIST_29 = 61,
        CONTROL_POINT_PATCHLIST_30 = 62,
        CONTROL_POINT_PATCHLIST_31 = 63,
        CONTROL_POINT_PATCHLIST_32 = 64
    };

	class RenderSystem : public GCore {
		public:

		static void ClearState() {
			m_dContext->ClearState();
		}

		static void SetPrimitiveTopology(const Topology& pt) {
            m_dContext->IASetPrimitiveTopology((D3D11_PRIMITIVE_TOPOLOGY)pt);
		}

        static void SetDepthStencilState(Stencil& stencil, const UINT& ref) {
            m_dContext->OMSetDepthStencilState(stencil.m_dsState.Get(), ref);
        }

        static void ClearDepthStencilView(Depth& depth, const bool& clearDepth, const bool& clearStencil) {
            UINT flag = 0;
            if(clearDepth && clearStencil)
                flag = D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL;
            else if(clearDepth)
                flag = D3D11_CLEAR_DEPTH;
            else if(clearStencil)
                flag = D3D11_CLEAR_STENCIL;

            m_dContext->ClearDepthStencilView(depth.m_dsView.Get(), flag, 1.f, 0u);
        }

        static void ClearDepthStencilView(DepthMap& depthMap, const bool& clearDepth, const bool& clearStencil) {
            UINT flag = 0;
            if(clearDepth && clearStencil)
                flag = D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL;
            else if(clearDepth)
                flag = D3D11_CLEAR_DEPTH;
            else if(clearStencil)
                flag = D3D11_CLEAR_STENCIL;

            m_dContext->ClearDepthStencilView(depthMap.m_dsView.Get(), flag, 1.f, 0u);
        }

        static void UnBindRenderTargets(const UINT& count) {
            std::vector<ID3D11RenderTargetView*> pRtv(count, nullptr);
            m_dContext->OMSetRenderTargets(count, pRtv.data(), NULL);
        }

        template<class T>
        static void UnBindShaderResources(const UINT& startSlot, const UINT& count);

        template<class T>
        static void UnBindShader();

        static void UnBindUnOrderedAccessViews(const UINT& slot, const UINT& count) {
            std::vector<ID3D11UnorderedAccessView*> uav;
            uav.reserve(count);
            uav.resize(count, nullptr);
            m_dContext->CSSetUnorderedAccessViews(slot, count, uav.data(), NULL);
        }

        static void BindRenderTargetView(Depth& depth) {
            m_dContext->OMSetRenderTargets(0, NULL, depth.m_dsView.Get());
        }

        static void BindRenderTargetView(DepthMap& depthMap) {
            m_dContext->OMSetRenderTargets(0, NULL, depthMap.m_dsView.Get());
        }

        static void BindRenderTargetView(BBuffer& backBuffer, Depth& depth) {
            m_dContext->OMSetRenderTargets(1, backBuffer.m_rtv.GetAddressOf(), depth.m_dsView.Get());
        }

        static void BindRenderTargetView(BBuffer& backBuffer, DepthMap& depthMap) {
            m_dContext->OMSetRenderTargets(1, backBuffer.m_rtv.GetAddressOf(), depthMap.m_dsView.Get());
        }

        static void BindRenderTargetView(BBuffer& backBuffer) {
            m_dContext->OMSetRenderTargets(1, backBuffer.m_rtv.GetAddressOf(), NULL);
        }

        static void BindRenderTargetView(GBuffer& gBuffer, Depth& depth) {
            UINT size = (UINT)gBuffer.m_rtvs.size();
            m_dContext->OMSetRenderTargets(size, gBuffer.m_rtvs.data(), depth.m_dsView.Get());
        }

        static void BindRenderTargetView(GBuffer& gBuffer, DepthMap& depthMap) {
            UINT size = (UINT)gBuffer.m_rtvs.size();
            m_dContext->OMSetRenderTargets(size, gBuffer.m_rtvs.data(), depthMap.m_dsView.Get());
        }

        static void BindRenderTargetView(GBuffer& gBuffer) {
            UINT size = (UINT)gBuffer.m_rtvs.size();
            m_dContext->OMSetRenderTargets(size, gBuffer.m_rtvs.data(), NULL);
        }
        
        static void BindRenderTargetView(ComRenderTargetView rtv) {
            m_dContext->OMSetRenderTargets(1, rtv.GetAddressOf(), NULL);
        }

        static void BindRenderTargetView(ComRenderTargetView rtv, Depth& depth) {
            m_dContext->OMSetRenderTargets(1, rtv.GetAddressOf(), depth.m_dsView.Get());
        }

        static void BindRenderTargetView(ComRenderTargetView rtv, DepthMap& depthMap) {
            m_dContext->OMSetRenderTargets(1, rtv.GetAddressOf(), depthMap.m_dsView.Get());
        }
        
        template<class T>
        static void BindShaderResourceView(const UINT& startSlot, GBuffer& gBuffer);
        
        template<class T>
        static void BindShaderResourceView(const UINT& slot, ShaderResource& shaderResource);

        template<class T>
        static void BindShaderResourceView(const UINT& slot, DepthMap& depthMap);

        template<class T>
        static void BindShaderResourceView(const UINT& slot, ComShaderResourceView srv);

        template<class T>
        static void BindShaderResourceView(const UINT& slot, UAView& uAView);

        template<class T>
        static void BindShaderResourceView(const UINT& slot, RWTexture2D& tex2d);

        template<class T>
        static void BindSamplers(const UINT& slot, Sampler& sampler);

        template<class T>
        static void BindShader(T& shader);
        
        static void BindUnOrderedAccessView(const UINT& slot, UAView& uAView) {
            m_dContext->CSSetUnorderedAccessViews(slot, 1, uAView.m_uav.GetAddressOf(), NULL);
        }

        static void BindUnOrderedAccessView(const UINT& slot, RWTexture2D& tex2d) {
            m_dContext->CSSetUnorderedAccessViews(slot, 1, tex2d.m_uav.GetAddressOf(), NULL);
        }

        static void ClearRenderTargetView(BBuffer& backBuffer, const Color& color) {
            FLOAT fColor[4];
            for(uint32_t i = 0; i < 4; i++)
                fColor[i] = color[i];

            m_dContext->ClearRenderTargetView(backBuffer.m_rtv.Get(), fColor);
        }

        static void ClearRenderTargetView(GBuffer& gBuffer, const Color& color) {
            FLOAT fColor[4];
            for(uint32_t i = 0; i < 4; i++)
                fColor[i] = color[i];

            for(uint32_t i = 0; i < gBuffer.m_rtvs.size(); i++)
                m_dContext->ClearRenderTargetView(gBuffer.m_rtvs[i], fColor);
        }

        static void SetInputLayout(ILayout& layout) {
            m_dContext->IASetInputLayout(layout.m_layout.Get());
        }

        static void SetViewPort(D3D11_VIEWPORT& viewPort) {
            m_dContext->RSSetViewports(1, &viewPort);
        }

        static void SetViewPort(DepthMap& depthMap) {
            m_dContext->RSSetViewports(1, &depthMap.m_viewPort);
        }

        static void SetRasteriserState(RState& rState) {
            m_dContext->RSSetState(rState.m_rState.Get());
        }

        static void Present(UINT SyncIntyerval) {
            m_sChain->Present(SyncIntyerval, NULL);
        }

        static void Dispatch(const uint32_t& x, const uint32_t& y, const uint32_t& z) {
            m_dContext->Dispatch(x, y, z);
        } 
        
        static void Dispatch(const Vec2i& xy, const uint32_t& z) {
            m_dContext->Dispatch(xy.x, xy.y, z);
        }

        static void Dispatch(const uint32_t& x, const Vec2i& xz) {
            m_dContext->Dispatch(x, xz.x, xz.y);
        }

        static void Dispatch(const Vec3i& groups) {
            m_dContext->Dispatch(groups.x, groups.y, groups.z);
        }
	};

    // ---------------------------- UnBindShaderResource ---------------------------------
    
    #define VS(startSlot, count, srv) VSSetShaderResources(startSlot, count, srv)
    #define HS(startSlot, count, srv) HSSetShaderResources(startSlot, count, srv)
    #define CS(startSlot, count, srv) CSSetShaderResources(startSlot, count, srv)
    #define DS(startSlot, count, srv) DSSetShaderResources(startSlot, count, srv)
    #define GS(startSlot, count, srv) GSSetShaderResources(startSlot, count, srv)
    #define PS(startSlot, count, srv) PSSetShaderResources(startSlot, count, srv)

    #define X(sName, lName) template<> inline void RenderSystem::UnBindShaderResources<lName> (const UINT& startSlot, const UINT& count) {\
        static std::vector<ID3D11ShaderResourceView*> pSrv;\
        pSrv.reserve(count);\
        pSrv.resize(count, nullptr);\
        m_dContext->sName(startSlot, count, pSrv.data());\
    }

    DEF_SHADER
    #undef X

    // --------------------------- BindShaderResourceView for GBuffer ------------------------------

    #define X(sName, lName) template<> inline void RenderSystem::BindShaderResourceView<lName> (const UINT& startSlot, GBuffer& gBuffer) {\
        UINT size = (UINT)gBuffer.m_srvs.size();\
        m_dContext->sName(startSlot, size, gBuffer.m_srvs.data());\
    }

    DEF_SHADER
    #undef X

    // --------------------------- BindShaderResourceView for ShaderResource ------------------------------

    #define X(sName, lName) template<> inline void RenderSystem::BindShaderResourceView<lName> (const UINT& slot, ShaderResource& shaderResource) {\
        m_dContext->sName(slot, 1, shaderResource.m_srv.GetAddressOf());\
    }

    DEF_SHADER
    #undef X

    // --------------------------- BindShaderResourceView for DepthMap ------------------------------

    #define X(sName, lName) template<> inline void RenderSystem::BindShaderResourceView<lName> (const UINT& slot, DepthMap& depthMap) {\
        m_dContext->sName(slot, 1, depthMap.m_srv.GetAddressOf());\
    }

    DEF_SHADER
    #undef X

    // ---------------------------- BindShaderResourceView for ComSRV -------------------------------

    #define X(sName, lName) template<> inline void RenderSystem::BindShaderResourceView<lName> (const UINT& slot, ComShaderResourceView srv) {\
        m_dContext->sName(slot, 1, srv.GetAddressOf());\
    }

    DEF_SHADER
    #undef X

    // ---------------------------- BindShaderResourceView for UAView -------------------------------

    #define X(sName, lName) template<> inline void RenderSystem::BindShaderResourceView<lName> (const UINT& slot, UAView& uAView) {\
    m_dContext->sName(slot, 1, uAView.m_srv.GetAddressOf());\
    }

    DEF_SHADER
    #undef X

    // -------------------------- BindShaderResourceView for RWTexture2D ----------------------------

    #define X(sName, lName) template<> inline void RenderSystem::BindShaderResourceView<lName> (const UINT& slot, RWTexture2D& tex2d) {\
    m_dContext->sName(slot, 1, tex2d.m_srv.GetAddressOf());\
    }

    DEF_SHADER
    #undef X

    #undef VS
    #undef HS
    #undef CS
    #undef DS
    #undef GS
    #undef PS
    
    // ------------------------------------- BindSamplers ------------------------------

    #define VS(slot, count, sampler) VSSetSamplers(slot, count, sampler)
    #define HS(slot, count, sampler) HSSetSamplers(slot, count, sampler)
    #define CS(slot, count, sampler) CSSetSamplers(slot, count, sampler)
    #define DS(slot, count, sampler) DSSetSamplers(slot, count, sampler)
    #define GS(slot, count, sampler) GSSetSamplers(slot, count, sampler)
    #define PS(slot, count, sampler) PSSetSamplers(slot, count, sampler)

    #define X(sName, lName) template<> inline void RenderSystem::BindSamplers<lName>(const UINT& slot, Sampler& sampler) {\
        m_dContext->sName(slot, 1, sampler.m_sState.GetAddressOf());\
    }

    DEF_SHADER
    #undef X

    #undef VS
    #undef HS
    #undef CS
    #undef DS
    #undef GS
    #undef PS

    // ------------------------------------- BindShader ------------------------------

    #define VS(shader) VSSetShader(shader, NULL, 0)
    #define HS(shader) HSSetShader(shader, NULL, 0)
    #define CS(shader) CSSetShader(shader, NULL, 0)
    #define DS(shader) DSSetShader(shader, NULL, 0)
    #define GS(shader) GSSetShader(shader, NULL, 0)
    #define PS(shader) PSSetShader(shader, NULL, 0)

    #define X(sName, lName) template<> inline void RenderSystem::BindShader<lName>(lName& shader) {\
        m_dContext->sName(shader.m_shader.Get());\
    }

    DEF_SHADER
    #undef X

    #define X(sName, lName) template<> inline void RenderSystem::UnBindShader<lName>() {\
        m_dContext->sName(NULL);\
    }

    DEF_SHADER
    #undef X

    #undef VS
    #undef HS
    #undef CS
    #undef DS
    #undef GS
    #undef PS
}