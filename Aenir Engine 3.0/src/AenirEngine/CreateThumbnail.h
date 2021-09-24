#pragma once

#include <memory>

namespace Aen {

	struct ImageByteData {
		int x; // width
		int y; // height
		int n; // 8 bit
		int channels = 4;
		
		unsigned char* imageData;

		void LoadImageThumb(const char* file);

		static void MakeThumbnail(const ImageByteData* source, ImageByteData* destination);

		void MakeThumbnail(ImageByteData* destination);

		void SaveImage(const char* filepath);

		static void SaveImage(const ImageByteData* source, const char* filepath);

		void freeData();
	};
}


