#include "PCH.h"
#include "Picture.h"
#include "Core\GlobalSettings.h"

namespace Aen {

    Aen::Picture::Picture():mp_WFactory(nullptr), mp_BCoder(nullptr), mp_FormatConverter(nullptr), mp_FrameDecode(nullptr), m_nr(-1)
    {}

    Aen::Picture::~Picture()
    {
        //Release stuff
        for (int i = 0; i < m_pictureData.size(); i++)
        {
            m_pictureData.at(i).bmp->Release();
        }
        m_pictureData.clear();

        if (mp_WFactory)
            mp_WFactory->Release();
        if (mp_BCoder)
            mp_BCoder->Release();
        if (mp_FormatConverter)
            mp_FormatConverter->Release();
        if (mp_FrameDecode)
            mp_FrameDecode->Release();
    }

    void Aen::Picture::Initialize()
    {
        ASSERT_HR(CoInitializeEx(NULL, COINIT_MULTITHREADED));
        ASSERT_HR(CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, __uuidof(IWICImagingFactory), (void**)(&mp_WFactory)));

    }

    void Aen::Picture::AddPicture(LPCWSTR path)
    {
        PictureData data;

        ASSERT_HR(mp_WFactory->CreateDecoderFromFilename(path, NULL, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &mp_BCoder));
        ASSERT_HR(mp_WFactory->CreateFormatConverter(&mp_FormatConverter));
        ASSERT_HR(mp_BCoder->GetFrame(0, &mp_FrameDecode));
        ASSERT_HR(mp_FormatConverter->Initialize(mp_FrameDecode, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0.f, WICBitmapPaletteTypeMedianCut));
        ASSERT_HR(m_target2D->CreateBitmapFromWicBitmap(mp_FormatConverter, NULL, &data.bmp));
        m_nr++;

        m_pictureData.push_back(data);
    }

    void Aen::Picture::SetPicPos(float x, float y)
    {
        m_pictureData.at(m_nr).rect.left    = x;
        m_pictureData.at(m_nr).rect.right   = x;
        m_pictureData.at(m_nr).rect.top     = y;
        m_pictureData.at(m_nr).rect.bottom  = y;
    }

    void Picture::SetPicPos(float x, float y, int indX)
    {
        m_pictureData.at(indX).rect.left = x;
        m_pictureData.at(indX).rect.right = x;
        m_pictureData.at(indX).rect.top = y;
        m_pictureData.at(indX).rect.bottom = y;
    }

    void Aen::Picture::SetPicSize(float width, float height)
    {
        float left = GetPicCenter(m_pictureData.at(m_nr).rect).x - (width / 2.f);
        float right = GetPicCenter(m_pictureData.at(m_nr).rect).x + (width / 2.f);
        float top = GetPicCenter(m_pictureData.at(m_nr).rect).y - (height / 2.f);
        float bottom = GetPicCenter(m_pictureData.at(m_nr).rect).y + (height / 2.f);

        m_pictureData.at(m_nr).rect.left = left;
        m_pictureData.at(m_nr).rect.right = right;
        m_pictureData.at(m_nr).rect.top = top;
        m_pictureData.at(m_nr).rect.bottom = bottom;
    }

    void Picture::SetPicSize(float width, float height, int indX)
    {
        float left = GetPicCenter(m_pictureData.at(indX).rect).x - (width / 2.f);
        float right = GetPicCenter(m_pictureData.at(indX).rect).x + (width / 2.f);
        float top = GetPicCenter(m_pictureData.at(indX).rect).y - (height / 2.f);
        float bottom = GetPicCenter(m_pictureData.at(indX).rect).y + (height / 2.f);

        m_pictureData.at(indX).rect.left = left;
        m_pictureData.at(indX).rect.right = right;
        m_pictureData.at(indX).rect.top = top;
        m_pictureData.at(indX).rect.bottom = bottom;
    }

    void Picture::UpdatePicture(float width, int indX)
    {
        //change bmp
        m_pictureData.at(indX).rect.right -= width;
        if (m_pictureData.at(indX).rect.right < 0.f) {
            m_pictureData.at(indX).rect.right = m_pictureData.at(indX).rect.left;
        }

    }

    bool Picture::Intersect(int index)
    {
        //Get mouse pos, cuz right now its dumb af
        POINT P;
        GetCursorPos(&P);
        ScreenToClient(Aen::GlobalSettings::GetWindow()->GetWHND(), &P);

        m_gameSize.x = (float)Aen::GlobalSettings::GetWindow()->GetSize().x;
        m_gameSize.y = (float)Aen::GlobalSettings::GetWindow()->GetSize().y;

        int X = GetSystemMetrics(SM_CXSCREEN);
        int Y = GetSystemMetrics(SM_CYSCREEN);
        float xSize = m_gameSize.x / X;
        float ySize = m_gameSize.y / Y;

        float x = P.x / xSize;
        float y = P.y / ySize;

        //OutputDebugStringA(("Mouse X: " + std::to_string(x) + "	Mouse Y: " + std::to_string(y) + "\n").c_str());
        //OutputDebugStringA(("Rect.x: " + std::to_string(GetButtonCenter(m_buttonData.at(0).rect).y) + "\n").c_str());

        if (x < m_pictureData.at(index).rect.right && x > m_pictureData.at(index).rect.left && y > m_pictureData.at(index).rect.top && y < m_pictureData.at(index).rect.bottom)
        {
            return true;
        }
        else {
            return false;
        }
    }

    Vec2f Aen::Picture::GetPicSize(D2D1_RECT_F& rect)
    {
        float width = rect.right - rect.left;
        float height = rect.bottom - rect.top;

        return Vec2f(width, height);
    }

    Vec2f Aen::Picture::GetPicCenter(D2D1_RECT_F& rect)
    {
        float xCenter = rect.right - (GetPicSize(rect).x / 2.f);
        float yCenter = rect.bottom - (GetPicSize(rect).y / 2.f);

        return Vec2f(xCenter, yCenter);
    }

    std::vector<PictureData> Picture::GetData() const
    {
        return m_pictureData;
    }

    void Aen::Picture::Draw(PictureData& data)
    {
        m_target2D->BeginDraw();

        m_target2D->DrawBitmap(data.bmp, data.rect);

        m_target2D->EndDraw();
    }

    void Aen::Picture::Update(int index)
    {
    }

}