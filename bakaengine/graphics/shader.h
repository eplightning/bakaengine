#ifndef GRAPHICS_SHADER_H_
#define GRAPHICS_SHADER_H_

#include <vector>

#include "bakaglobal.h"
#include <GL/glew.h>

#include "graphics/mesh.h"

namespace Baka
{
	class DirectionalLightComponent;
	class PointLightComponent;

	class Shader {
	public:
		Shader(const char *vertex, const char *fragment);
		~Shader();

		void Enable();

		GLint GetUniformLocation(const char *name);

		void SetupUniforms(const Mat4 &view, const Mat4 &proj, const Vec3 &ambient, const std::vector<DirectionalLightComponent*> &directional, const std::vector<PointLightComponent*> &point);
		void SetupUniformsForMesh(const Vec4 &blend, const Mat4 &model, const Mat4 &normal, const Mesh &mesh, const MeshEntry &entry, GLuint placeholder_tex);

	private:
		GLuint program_;
		GLuint vshader_;
		GLuint fshader_;
	};
} // namespace Baka

#endif