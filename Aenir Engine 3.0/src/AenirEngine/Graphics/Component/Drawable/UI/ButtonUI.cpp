#include "PCH.h"
#include "ButtonUI.h"

namespace Aen {

	Aen::ButtonUI::ButtonUI()
	{
	}

	Aen::ButtonUI::~ButtonUI()
	{
	}

	void Aen::ButtonUI::Initialize()
	{
		//left: Top left vertex X Pos
		//top:  Top left vertex Y Pos
		//right: Bottom right vertex X Pos
		//bottom: Bottom right vertex Y Pos

		rect.left = 0;;
		rect.top = 50;
		rect.right = 500;
		rect.bottom = 500;
	}

	void Aen::ButtonUI::AddButton(LPCWSTR texturePath)
	{
		IWICImagingFactory* WFactory = NULL;
		IWICBitmapDecoder* BCoder = NULL;
		IWICFormatConverter* FormatConverter = NULL;
		IWICBitmapFrameDecode* FrameDecode = NULL;

		ASSERT_HR(CoInitializeEx(NULL, COINIT_MULTITHREADED));
		ASSERT_HR(CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, __uuidof(IWICImagingFactory), (void**)(&WFactory)));
		ASSERT_HR(WFactory->CreateDecoderFromFilename(texturePath, NULL, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &BCoder));
		ASSERT_HR(WFactory->CreateFormatConverter(&FormatConverter));
		ASSERT_HR(BCoder->GetFrame(0, &FrameDecode));
		ASSERT_HR(FormatConverter->Initialize(FrameDecode, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0.f, WICBitmapPaletteTypeMedianCut));
		ASSERT_HR(m_target2D->CreateBitmapFromWicBitmap(FormatConverter, NULL, &testBmp));

		WFactory->Release();
		BCoder->Release();
		FormatConverter->Release();
		FrameDecode->Release();
	}

	void Aen::ButtonUI::Draw()
	{
		m_target2D->BeginDraw();

		m_target2D->DrawBitmap(testBmp, rect);

		m_target2D->EndDraw();
	}
}