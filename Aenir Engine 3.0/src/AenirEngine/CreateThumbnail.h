#pragma once

#include <memory>

namespace Aen {

	struct ImageByteData {
		int x = 0; // width
		int y = 0; // height
		int n = 0; // 8 bit
		int channels = 4;
		
		unsigned char* imageData = nullptr;

		void LoadImageThumb(const char* file);

		static void MakeThumbnail(const ImageByteData* source, ImageByteData* destination);

		void MakeThumbnail(ImageByteData* destination);

		void SaveImage(const char* filepath);

		static void SaveImage(const ImageByteData* source, const char* filepath);

		void FreeData();
	};
}


