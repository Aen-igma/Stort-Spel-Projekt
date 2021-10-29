#include"PCH.h"
#include"DX11Core.h"

namespace Aen {

    ComDevice GCore::m_device{nullptr};
    ComDeviceContext GCore::m_dContext{nullptr};
    ComSwapChain GCore::m_sChain{nullptr};
    Com2DFactory GCore::m_factory{ nullptr };
    Com2DTarget GCore::m_target2D{ nullptr };

	bool GCore::Concealed::Initialize(const Window& window) {
    
        DXGI_SWAP_CHAIN_DESC sChainDesc;
        ZeroMemory(&sChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

        sChainDesc.BufferDesc.Width = (UINT)window.GetSize().x;
        sChainDesc.BufferDesc.Height = (UINT)window.GetSize().y;
        sChainDesc.BufferDesc.RefreshRate.Numerator = 60;
        sChainDesc.BufferDesc.RefreshRate.Denominator = 1;
        sChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        sChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
        sChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
        
        sChainDesc.SampleDesc.Count = 1;
        sChainDesc.SampleDesc.Quality = 0;
        
        sChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_UNORDERED_ACCESS;
        sChainDesc.BufferCount = 1;
        sChainDesc.OutputWindow = window.m_hwnd;
        sChainDesc.Windowed = true;
        sChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
        sChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
        
        ComAdapter1 pAdapter = NULL;
        ComFactory2 pFactory2 = NULL;
        ComFactory6 pFactory6 = NULL;

        UINT fFlag = 0;
        #ifdef _DEBUG
        fFlag = DXGI_CREATE_FACTORY_DEBUG;
        #endif

        if(FAILED(CreateDXGIFactory2(fFlag, __uuidof(IDXGIFactory2), reinterpret_cast<void**>(pFactory2.GetAddressOf()))))
            return false;

        if(FAILED(pFactory2->QueryInterface(pFactory6.GetAddressOf())))
            return false;

        if(FAILED(pFactory6->EnumAdapterByGpuPreference(0, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(pAdapter.GetAddressOf())))) {
            pFactory2->EnumAdapters1(0, pAdapter.GetAddressOf());
       }

        DXGI_ADAPTER_DESC1 desc;
        pAdapter->GetDesc1(&desc);

        const UINT featureLvls = 2;
        D3D_FEATURE_LEVEL featureLvl[featureLvls] = {
            D3D_FEATURE_LEVEL_11_1,
            D3D_FEATURE_LEVEL_11_0
        };

        UINT flags = 0;
        #ifdef _DEBUG
        flags = D3D11_CREATE_DEVICE_DEBUG | D3D11_CREATE_DEVICE_BGRA_SUPPORT;
        #endif
        
        //#ifdef NDEBUG
        //flags = D3D11_CREATE_DEVICE_DEBUG | D3D11_CREATE_DEVICE_BGRA_SUPPORT;
        //#endif // RELEASE


        HRESULT hr = D3D11CreateDeviceAndSwapChain(
            pAdapter.Get(),
            D3D_DRIVER_TYPE_UNKNOWN,
            NULL,
            flags,
            featureLvl,
            featureLvls,
            D3D11_SDK_VERSION,
            &sChainDesc,
            m_sChain.GetAddressOf(),
            m_device.GetAddressOf(),
            NULL,
            m_dContext.GetAddressOf());

        pAdapter.Reset();
        pFactory2.Reset();
        pFactory6.Reset();


        //----------------------------------    Direct 2D   ---------------------------------//
        ASSERT_HR(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, m_factory.GetAddressOf()));

        IDXGISurface* IXSurface;
        if (SUCCEEDED(m_sChain->GetBuffer(0, IID_PPV_ARGS(&IXSurface))))
        {
            Vec2f dpi;
            dpi = static_cast<FLOAT>(GetDpiForWindow(window.m_hwnd));
            D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT, D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED), dpi.x, dpi.y);
            ASSERT_HR(m_factory->CreateDxgiSurfaceRenderTarget(IXSurface, props, m_target2D.GetAddressOf()));
        }

        return SUCCEEDED(hr);
	}

    void GCore::Concealed::Release() {
        m_device.Reset();
        m_dContext.Reset();
        m_sChain.Reset();
        m_target2D.Reset();
        m_factory.Reset();
    }
}