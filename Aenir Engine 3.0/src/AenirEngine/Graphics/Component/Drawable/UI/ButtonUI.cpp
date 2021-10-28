#include "PCH.h"
#include "ButtonUI.h"

namespace Aen {
	Vec2f ButtonUI::GetButtonSize(D2D1_RECT_F& rect)
	{
		float buttonWidth = rect.right - rect.left;
		float buttonHeight = rect.bottom - rect.top;

		return Vec2f(buttonWidth, buttonHeight);
	}

	Vec2f ButtonUI::GetButtonCenter(D2D1_RECT_F& rect)
	{
		float xCenter = rect.right - (GetButtonSize(rect).x / 2.f);
		float yCenter = rect.bottom - (GetButtonSize(rect).y / 2.f);

		//Den här hämtar knappens locala mittpunkt
		//center.x = GetButtonSize(button).x / 2.f;
		//center.y = GetButtonSize(button).y / 2.f;

		return Vec2f(xCenter, yCenter);
	}

	

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
	}

	void Aen::ButtonUI::AddButton(LPCWSTR texturePath, int indX)
	{
		ButtonData data;
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
		ASSERT_HR(m_target2D->CreateBitmapFromWicBitmap(FormatConverter, NULL, &data.bmp));

		//tempData.name = name;
		data.index = indX;
		buttonData.push_back(data);
		WFactory->Release();
		BCoder->Release();
		FormatConverter->Release();
		FrameDecode->Release();
	}

	void ButtonUI::SetButtonPos(float x, float y, int indX)
	{
		//rect.left = x;
		//rect.top = y;
		//rect.right = x;
		//rect.bottom = y;

		buttonData.at(indX).rect.left = x;
		buttonData.at(indX).rect.top = y;
		buttonData.at(indX).rect.right = x;
		buttonData.at(indX).rect.bottom = y;
	}

	void ButtonUI::SetButtonSize(float width, float height, int indX)
	{
		//left: Top left vertex X Pos
		//top:  Top left vertex Y Pos
		//right: Bottom right vertex X Pos
		//bottom: Bottom right vertex Y Pos

		//Skapa en RECT från mittpunkten
		//Mittpunkten ändras varje gång du ändrar en punkts position så positionerna måste sparas innan du ändrar dem
		float left = GetButtonCenter(buttonData.at(indX).rect).x - (width / 2.f);
		float right = GetButtonCenter(buttonData.at(indX).rect).x + (width / 2.f);
		float top = GetButtonCenter(buttonData.at(indX).rect).y - (height / 2.f);
		float bottom = GetButtonCenter(buttonData.at(indX).rect).y + (height / 2.f);

		buttonData.at(indX).rect.left = left;
		buttonData.at(indX).rect.right = right;
		buttonData.at(indX).rect.top = top;
		buttonData.at(indX).rect.bottom = bottom;

		tempData = buttonData;
	}

	std::vector<ButtonData> ButtonUI::GetData() const
	{
		return buttonData;
	}

	void Aen::ButtonUI::Draw(ButtonData& data)
	{
		m_target2D->BeginDraw();

		m_target2D->DrawBitmap(data.bmp, data.rect);

		m_target2D->EndDraw();
	}

	
}