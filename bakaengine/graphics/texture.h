#ifndef GRAPHICS_TEXTURE_H_
#define GRAPHICS_TEXTURE_H_

#include "bakaglobal.h"
#include <GL/glew.h>

namespace Baka
{
	class Texture {
	public:
		explicit Texture(const char *resource);
		~Texture();

		GLuint GetTextureID() const;
		bool IsValid() const;

	protected:
		GLuint texid_;
	};
} // namespace Baka

#endif