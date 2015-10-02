#include "graphics/imagesupport.h"

#include "bakaglobal.h"
#include <GL/glew.h>
#include <SOIL.h>

#include "resources/resources.h"
#include "game/core/registry.h"

using namespace Baka;

void ImageSupport::Free(ImageData *img)
{
	free(img->data);
}

ImageData ImageSupport::Load(const char *resource)
{
	ImageData out;
	out.data = nullptr;

	ResourceData data = Registry::Instance()->GetResources()->GetData(resource);

	if (!data.bytes)
		return out;

	out.data = SOIL_load_image_from_memory(data.bytes, data.size, &out.width, &out.height, reinterpret_cast<int*>(&out.channels), SOIL_LOAD_AUTO);

	return out;
}

GLuint ImageSupport::LoadTexture(const char *resource)
{
	ImageData img = Load(resource);

	GLuint result = LoadTexture(&img);

	Free(&img);

	return result;
}

GLuint ImageSupport::LoadTexture(ImageData *img) const
{
	if (img->data == nullptr) {
		LOG_DEBUG("Texture couldn't be loaded, resource is not loaded correctly");
		return 0;
	}

	GLuint id;
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	img->width = img->width / 2 * 2;
	img->height = img->height / 2 * 2;

	switch (img->channels) {
		case ImageChannels::RGB:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img->width, img->height, 0, GL_RGB, GL_UNSIGNED_BYTE, img->data);
			break;

		case ImageChannels::RGBA:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img->width, img->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img->data);
			break;

		default:
			LOG_DEBUG("Texture couldn't be loaded, unsupported data format");
			return 0;
	}

	glGenerateMipmap(GL_TEXTURE_2D);

	return id;
}