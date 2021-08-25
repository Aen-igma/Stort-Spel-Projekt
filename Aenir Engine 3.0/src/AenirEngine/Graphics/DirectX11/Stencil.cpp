#include "PCH.h"
#include "Stencil.h"

namespace Aen {

    Stencil::~Stencil() {
        m_dsState.Reset();
    }

    Stencil::Stencil()
		:m_dsState(NULL) {}

	Stencil::Stencil(const bool& useDepth, const StencilType& type) 
		:m_dsState(NULL) {

        D3D11_DEPTH_STENCIL_DESC dsDesc = CD3D11_DEPTH_STENCIL_DESC{CD3D11_DEFAULT{}};

        dsDesc.DepthEnable = useDepth;
        dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

        if(type == StencilType::Write) {
            dsDesc.StencilEnable = TRUE;
            dsDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

            dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
            dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
            dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
            dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_NEVER;

            dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
            dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
            dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
            dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

        } else if(type == StencilType::Mask) {
            dsDesc.StencilEnable = TRUE;
            dsDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;

            dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
            dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
            dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
            dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_NEVER;

            dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
            dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
            dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
            dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_LESS_EQUAL;
        }

        if(FAILED(m_device->CreateDepthStencilState(&dsDesc, m_dsState.GetAddressOf())))
            throw;
	}

	void Stencil::SetStencil(const bool& useDepth, const StencilType& type) {

        D3D11_DEPTH_STENCIL_DESC dsDesc = CD3D11_DEPTH_STENCIL_DESC{CD3D11_DEFAULT{}};

        dsDesc.DepthEnable = useDepth;
        dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

        if(type == StencilType::Write) {
            dsDesc.StencilEnable = TRUE;
            dsDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

            dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
            dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
            dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
            dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_NEVER;

            dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
            dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
            dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
            dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

        } else if(type == StencilType::Mask) {
            dsDesc.StencilEnable = TRUE;
            dsDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;

            dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
            dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
            dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
            dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_NEVER;

            dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
            dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
            dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
            dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_LESS_EQUAL;
        }

        if(FAILED(m_device->CreateDepthStencilState(&dsDesc, m_dsState.GetAddressOf())))
            throw;
	}
}