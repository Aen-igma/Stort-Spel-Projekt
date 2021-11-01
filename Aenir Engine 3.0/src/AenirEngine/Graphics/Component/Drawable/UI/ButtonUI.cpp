#include "PCH.h"
#include "ButtonUI.h"
#include "Core\GlobalSettings.h"

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

	bool  ButtonUI::Intersect(int index)
	{
		//Get mouse pos, cuz right now its dumb af
		POINT P;
		GetCursorPos(&P);
		ScreenToClient(Aen::GlobalSettings::GetWindow()->GetWHND() ,&P);

		m_gameSize.x = Aen::GlobalSettings::GetWindow()->GetSize().x;
		m_gameSize.y = Aen::GlobalSettings::GetWindow()->GetSize().y;

		int X = GetSystemMetrics(SM_CXSCREEN);
		int Y = GetSystemMetrics(SM_CYSCREEN);
		float xSize = m_gameSize.x / X;
		float ySize = m_gameSize.y / Y;

		float x = P.x / xSize;
		float y = P.y / ySize;

		//OutputDebugStringA(("Mouse X: " + std::to_string(x) + "	Mouse Y: " + std::to_string(y) + "\n").c_str());
		//OutputDebugStringA(("Rect.x: " + std::to_string(GetButtonCenter(m_buttonData.at(0).rect).y) + "\n").c_str());

		if (x < m_buttonData.at(index).rect.right && x > m_buttonData.at(index).rect.left && y > m_buttonData.at(index).rect.top && y < m_buttonData.at(index).rect.bottom)
		{
			return true;
		}
		else {
			return false;
		}
	}

	Aen::ButtonUI::ButtonUI(): mp_WFactory (nullptr), mp_BCoder (nullptr),	mp_FormatConverter(nullptr), mp_FrameDecode(nullptr)
	{}

	Aen::ButtonUI::~ButtonUI()
	{
		m_buttonData.clear();
		mp_WFactory->Release();
		mp_BCoder->Release();
		mp_FormatConverter->Release();
		mp_FrameDecode->Release();
	}

	void Aen::ButtonUI::Initialize()
	{
		//left: Top left vertex X Pos
		//top:  Top left vertex Y Pos
		//right: Bottom right vertex X Pos
		//bottom: Bottom right vertex Y Pos

		ASSERT_HR(CoInitializeEx(NULL, COINIT_MULTITHREADED));
		ASSERT_HR(CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, __uuidof(IWICImagingFactory), (void**)(&mp_WFactory)));
	}

	void Aen::ButtonUI::AddButton(LPCWSTR texturePath, int indX)
	{
		ButtonData data;

		ASSERT_HR(mp_WFactory->CreateDecoderFromFilename(texturePath, NULL, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &mp_BCoder));
		ASSERT_HR(mp_WFactory->CreateFormatConverter(&mp_FormatConverter));
		ASSERT_HR(mp_BCoder->GetFrame(0, &mp_FrameDecode));
		ASSERT_HR(mp_FormatConverter->Initialize(mp_FrameDecode, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0.f, WICBitmapPaletteTypeMedianCut));
		ASSERT_HR(m_target2D->CreateBitmapFromWicBitmap(mp_FormatConverter, NULL, &data.bmp));

		//tempData.name = name;
		data.index = indX;
		m_buttonData.push_back(data);
	}

	void ButtonUI::SetButtonPos(float x, float y, int indX)
	{
		m_buttonData.at(indX).rect.left = x;
		m_buttonData.at(indX).rect.top = y;
		m_buttonData.at(indX).rect.right = x;
		m_buttonData.at(indX).rect.bottom = y;
	}

	void ButtonUI::SetButtonSize(float width, float height, int indX)
	{
		//left: Top left vertex X Pos
		//top:  Top left vertex Y Pos
		//right: Bottom right vertex X Pos
		//bottom: Bottom right vertex Y Pos

		//Skapa en RECT från mittpunkten
		//Mittpunkten ändras varje gång du ändrar en punkts position så positionerna måste sparas innan du ändrar dem
		float left = GetButtonCenter(m_buttonData.at(indX).rect).x - (width / 2.f);
		float right = GetButtonCenter(m_buttonData.at(indX).rect).x + (width / 2.f);
		float top = GetButtonCenter(m_buttonData.at(indX).rect).y - (height / 2.f);
		float bottom = GetButtonCenter(m_buttonData.at(indX).rect).y + (height / 2.f);

		m_buttonData.at(indX).rect.left = left;
		m_buttonData.at(indX).rect.right = right;
		m_buttonData.at(indX).rect.top = top;
		m_buttonData.at(indX).rect.bottom = bottom;
	}

	std::vector<ButtonData> ButtonUI::GetData() const
	{
		return m_buttonData;
	}

	void ButtonUI::SaveData()
	{
		m_tempData = m_buttonData;
	}

	void Aen::ButtonUI::Draw(ButtonData& data)
	{
		m_target2D->BeginDraw();

		m_target2D->DrawBitmap(data.bmp, data.rect);

		m_target2D->EndDraw();
	}

	void ButtonUI::Update(int index)
	{
		ButtonData temp = m_tempData.at(index);

		if (Intersect(index)) {
			int addX = 20.f;
			int addY = 5.f;

			float buttonWidth = GetButtonSize(m_tempData.at(index).rect).x + addX;
			float buttonHeight = GetButtonSize(m_tempData.at(index).rect).y + addY;

			SetButtonSize(buttonWidth, buttonHeight, index);
		}
		else {
			m_buttonData.at(index).rect = temp.rect;
		}
	}
}