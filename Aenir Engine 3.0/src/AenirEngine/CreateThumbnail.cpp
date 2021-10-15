#include "PCH.h"
#include "CreateThumbnail.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_RESIZE_IMPLEMENTATION

#include "ThirdParty/stb_image.h"
#include "ThirdParty/stb_image_write.h"
#include "ThirdParty/stb_image_resize.h"

namespace Aen {

	void ImageByteData::LoadImageThumb(const char* file)
	{
		imageData = stbi_load(file, &x, &y, &n, channels);
	}

	void ImageByteData::MakeThumbnail(const ImageByteData* source, ImageByteData* destination)
	{
		destination->imageData = new unsigned char[destination->x * destination->y * destination->channels];
		stbir_resize_uint8(source->imageData, source->x, source->y, 0, destination->imageData, destination->x, destination->y, 0, destination->channels);
	}

	void ImageByteData::MakeThumbnail(ImageByteData* destination)
	{
		destination->imageData = new unsigned char[destination->x * destination->y * destination->channels];
		stbir_resize_uint8(this->imageData, this->x, this->y, 0, destination->imageData, destination->x, destination->y, 0, destination->channels);
	}

	void ImageByteData::SaveImage(const char* filepath)
	{
		stbi_write_png(filepath, this->x, this->y, this->channels, this->imageData, this->x * channels);
	}

	void ImageByteData::SaveImage(const ImageByteData* source, const char* filepath)
	{
		stbi_write_png(filepath, source->x, source->y, source->channels, source->imageData, source->x * source->channels);
	}

	void ImageByteData::FreeData()
	{
		stbi_image_free(imageData);
	}
}

