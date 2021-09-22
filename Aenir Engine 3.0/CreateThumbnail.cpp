#include "PCH.h"
#include "CreateThumbnail.h"

void ImageByteData::LoadImage(const char* file)
{
	stbi_load(file, &x, &y, &channels, 4);
}
