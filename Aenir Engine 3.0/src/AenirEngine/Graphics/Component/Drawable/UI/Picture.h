#pragma once
#include "Graphics\Graphics.h"
#include "BuritoMath.h"
#include "Window\Input.h"
#include <wincodec.h>
#include <string>

namespace Aen {

	struct PictureData
	{
		D2D1_RECT_F rect;
		ID2D1Bitmap* bmp;
	};

	class Picture : public GCore
	{
	private:
		std::vector<PictureData> m_pictureData;
		Vec2f m_gameSize;
		int m_nr;

		IWICImagingFactory* mp_WFactory;
		IWICBitmapDecoder* mp_BCoder;
		IWICFormatConverter* mp_FormatConverter;
		IWICBitmapFrameDecode* mp_FrameDecode;
	public:
		Picture();
		~Picture();

		void Initialize();
		void AddPicture(LPCWSTR path);
		void SetPicPos(float x, float y);
		void SetPicSize(float width, float height);
		void UpdatePicture(float width, int indX);

		friend class GameLoop;

		Vec2f GetPicSize(D2D1_RECT_F& rect);
		Vec2f GetPicCenter(D2D1_RECT_F& rect);
		std::vector<PictureData> GetData()const;


		void Draw(PictureData& data);
		void Update(int index);
	};
}