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

	bool ButtonUI::Intersect(Window& window, int index)
	{
		//Get mouse pos, cuz right now its dumb af
		//POINT P;
		bool intersected = false;
		//GetCursorPos(&P);
		//ScreenToClient(window.GetWHND() ,&P);

		float x = Input::GetMousePos().x;
		float y = Input::GetMousePos().y;

		//OutputDebugStringA(("Mouse X: " + std::to_string(x) + "	Mouse Y: " + std::to_string(y) + "\n").c_str());
		//OutputDebugStringA(("Rect.x: " + std::to_string(buttonData.at(0).rect.right) + "\n").c_str());

		if (x < buttonData.at(index).rect.right && x > buttonData.at(index).rect.left && y > buttonData.at(index).rect.top && y < buttonData.at(index).rect.bottom)
		{
			intersected = true;
		}
		else {
			intersected = false;
		}

		return intersected;
	}

	Aen::ButtonUI::ButtonUI()
	{
	}

	Aen::ButtonUI::~ButtonUI()
	{
		buttonData.clear();
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
	}

	std::vector<ButtonData> ButtonUI::GetData() const
	{
		return buttonData;
	}

	void ButtonUI::SaveData()
	{
		tempData = buttonData;
	}

	void ButtonUI::ClearButtons()
	{
			buttonData.clear();
	}

	void Aen::ButtonUI::Draw(ButtonData& data)
	{
		m_target2D->BeginDraw();

		m_target2D->DrawBitmap(data.bmp, data.rect);

		m_target2D->EndDraw();
	}

	void ButtonUI::Update(Window& window, int indX)
	{
		int addX = 20.f;
		int addY = 5.f;
		ButtonData temp = tempData.at(indX);

		tempBool = Intersect(window, indX);

		if (tempBool)
		{
			float buttonWidth = GetButtonSize(tempData.at(indX).rect).x + addX;
			float buttonHeight = GetButtonSize(tempData.at(indX).rect).y + addY;

			SetButtonSize(buttonWidth, buttonHeight, indX);
		}
		else {

			//Behöver fixa logiken, fungerar bara på en knapp inte på flera eftersom positionen är static. Knapparna som kommer efter sätts till första knappens position
			buttonData.at(indX).rect = temp.rect;
		}
	}
	bool ButtonUI::getBool() const
	{
		return tempBool;
	}
}