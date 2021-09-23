#pragma once
#include "ThirdParty/stb_image.h"
#include "ThirdParty/stb_image_resize.h"
#include "ThirdParty/stb_image_write.h"

#include <memory>

struct ImageByteData {
	int x;
	int y;
	int channels;

	std::unique_ptr<unsigned char> imageData;
	void LoadImage(const char* file);

	static void MakeThumbnail(const ImageByteData* source, const ImageByteData* destination) {
		stbir_resize_uint8(source->imageData.get(), source->x, source->y, 1, destination->imageData.get(), destination->x, destination->y, 1, destination->channels);
	}
	void MakeThumbnail(const ImageByteData* destination) {
		stbir_resize_uint8(this->imageData.get(), this->x, this->y, 1, destination->imageData.get(), destination->x, destination->y, 1, destination->channels);
	}
	void SaveImage(const char* filepath) {
		stbi_write_png(filepath, this->x, this->y, 8, this->imageData.get(), 1);
	}
	static void SaveImage(const ImageByteData* source, const char* filepath) {
		stbi_write_png(filepath, source->x, source->y, 8, source->imageData.get(), 1);
	}
};

