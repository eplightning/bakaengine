#ifndef GRAPHICS_IMAGESUPPORT_H_
#define GRAPHICS_IMAGESUPPORT_H_

#include "bakaglobal.h"
#include <GL/glew.h>

namespace Baka
{
	enum class ImageChannels : int {
		Unknown = 0,
		L,
		LA,
		RGB,
		RGBA
	};

	struct ImageData {
		u8 *data;
		int width;
		int height;
		ImageChannels channels;
	};

	class ImageSupport {
	public:
		void Free(ImageData *img);
		ImageData Load(const char *resource);

		GLuint LoadTexture(const char *resource);
		GLuint LoadTexture(ImageData *img) const;
	};
} // namespace Baka

#endif