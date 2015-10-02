#ifndef GRAPHICS_RENDERER_H_
#define GRAPHICS_RENDERER_H_

#include "bakaglobal.h"
#include <GL/glew.h>

#include "game/scene/scene.h"

namespace Baka
{
	class Renderer {
	public:
		Renderer(float fovy, float ar, float n, float f);
		void RenderScene(Scene &scene);

		Vec3& GetAmbient();

	protected:
		Vec3 ambient_;
		Mat4 projection_;
		GLuint placeholder_tex_;
	};
} // namespace Baka

#endif