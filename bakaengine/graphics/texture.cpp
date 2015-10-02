#include "graphics/texture.h"

#include "bakaglobal.h"
#include <GL/glew.h>

#include "graphics/imagesupport.h"
#include "game/core/registry.h"

using namespace Baka;

Texture::Texture(const char *resource)
{
	texid_ = Registry::Instance()->GetImageSupport()->LoadTexture(resource);
}

Texture::~Texture()
{
	if (texid_ != 0)
		glDeleteTextures(1, &texid_);
}

GLuint Texture::GetTextureID() const
{
	return texid_;
}

bool Texture::IsValid() const
{
	return texid_ != 0;
}