#include "graphics/shader.h"

#include <stdexcept>

#include "bakaglobal.h"
#include <GL/glew.h>

#include <glm/gtc/type_ptr.hpp>

#include "game/core/registry.h"
#include "game/scene/common_components.h"

using namespace Baka;

Shader::Shader(const char *vertex, const char *fragment)
{
	ResourceData vsinfo = Registry::Instance()->GetResources()->GetData(vertex);
	ResourceData fsinfo = Registry::Instance()->GetResources()->GetData(fragment);

	if (vsinfo.size == 0 || fsinfo.size == 0)
		throw std::runtime_error("Vertex shader and/or fragment shader files not found");

	program_ = glCreateProgram();
	vshader_ = glCreateShader(GL_VERTEX_SHADER);
	fshader_ = glCreateShader(GL_FRAGMENT_SHADER);

	GLint tmp = vsinfo.size;
	GLint tmp2 = fsinfo.size;
	glShaderSource(vshader_, 1, &vsinfo.chars, &tmp);
	glShaderSource(fshader_, 1, &fsinfo.chars, &tmp2);

	glCompileShader(vshader_);
	glCompileShader(fshader_);

	glGetShaderiv(vshader_, GL_COMPILE_STATUS, &tmp);
	glGetShaderiv(fshader_, GL_COMPILE_STATUS, &tmp2);

	if (tmp == GL_FALSE || tmp2 == GL_FALSE) {
		if (tmp == GL_FALSE) {
			glGetShaderiv(vshader_, GL_INFO_LOG_LENGTH, &tmp);
			GLchar *error = new GLchar[tmp];
			glGetShaderInfoLog(vshader_, tmp, &tmp, error);

			LOG_INFO("Vertex shader compile error: %s", error);

			delete[] error;
		}

		if (tmp2 == GL_FALSE) {
			glGetShaderiv(fshader_, GL_INFO_LOG_LENGTH, &tmp);
			GLchar *error = new GLchar[tmp];
			glGetShaderInfoLog(fshader_, tmp, &tmp, error);

			LOG_INFO("Fragment shader compile error: %s", error);

			delete[] error;
		}

		throw std::runtime_error("Vertex shader and/or fragment shader did not compile");
	}

	glAttachShader(program_, vshader_);
	glAttachShader(program_, fshader_);
	glLinkProgram(program_);

	glGetProgramiv(program_, GL_LINK_STATUS, &tmp);

	if (tmp == GL_FALSE) {
		glGetProgramiv(program_, GL_INFO_LOG_LENGTH, &tmp);
		GLchar *error = new GLchar[tmp];
		glGetProgramInfoLog(program_, tmp, &tmp, error);

		LOG_INFO("Shader program link error: %s", error);

		delete[] error;

		throw std::runtime_error("Vertex shader and fragment shader couldn't get linked into program");
	}
}

Shader::~Shader()
{
	glDetachShader(program_, vshader_);
	glDetachShader(program_, fshader_);
	glDeleteShader(vshader_);
	glDeleteShader(fshader_);
	glDeleteProgram(program_);
}

void Shader::Enable()
{
	glUseProgram(program_);
}

GLint Shader::GetUniformLocation(const char *name)
{
	return glGetUniformLocation(program_, name);
}

void Shader::SetupUniforms(const Mat4 &view, const Mat4 &proj, const Vec3 &ambient, const std::vector<DirectionalLightComponent*> &directional, const std::vector<PointLightComponent*> &point)
{
	glUniformMatrix4fv(GetUniformLocation("view_matrix"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(GetUniformLocation("projection_matrix"), 1, GL_FALSE, glm::value_ptr(proj));
	glUniform3fv(GetUniformLocation("ambient_color"), 1, glm::value_ptr(ambient));

	std::vector<Vec3> dirlight;
	std::vector<Vec3> pointlight;
	std::vector<Vec3> dirlight_diffuse;
	std::vector<Vec3> dirlight_specular;
	std::vector<Vec3> pointlight_diffuse;
	std::vector<Vec3> pointlight_specular;
	std::vector<float> pointlight_intensity;

	for (auto x : directional) {
		dirlight.push_back(x->GetDirection());
		dirlight_diffuse.push_back(x->GetDiffuseColor());
		dirlight_specular.push_back(x->GetSpecularColor());
	}

	for (auto x : point) {
		pointlight.push_back(x->GetPosition());
		pointlight_diffuse.push_back(x->GetDiffuseColor());
		pointlight_specular.push_back(x->GetSpecularColor());
		pointlight_intensity.push_back(x->GetIntensity());
	}

	dirlight.resize(3);
	pointlight.resize(3);
	dirlight_diffuse.resize(3);
	dirlight_specular.resize(3);
	pointlight_diffuse.resize(3);
	pointlight_specular.resize(3);
	pointlight_intensity.resize(3);

	glUniform3fv(GetUniformLocation("dirlight"), 3, reinterpret_cast<float*>(&dirlight[0]));
	glUniform3fv(GetUniformLocation("pointlight"), 3, reinterpret_cast<float*>(&pointlight[0]));
	glUniform3fv(GetUniformLocation("dirlight_diffuse"), 3, reinterpret_cast<float*>(&dirlight_diffuse[0]));
	glUniform3fv(GetUniformLocation("pointlight_diffuse"), 3, reinterpret_cast<float*>(&pointlight_diffuse[0]));
	glUniform3fv(GetUniformLocation("dirlight_specular"), 3, reinterpret_cast<float*>(&dirlight_specular[0]));
	glUniform3fv(GetUniformLocation("pointlight_specular"), 3, reinterpret_cast<float*>(&pointlight_specular[0]));
	glUniform1fv(GetUniformLocation("pointlight_intensity"), 3, reinterpret_cast<float*>(&pointlight_intensity[0]));

	glUniform1i(GetUniformLocation("dirlight_count"), static_cast<GLint>(directional.size()));
	glUniform1i(GetUniformLocation("pointlight_count"), static_cast<GLint>(point.size()));
}

void Shader::SetupUniformsForMesh(const Vec4 &blend, const Mat4 &model, const Mat4 &normal, const Mesh& mesh, const MeshEntry& entry, GLuint placeholder_tex)
{
	glUniformMatrix4fv(GetUniformLocation("model_matrix"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(GetUniformLocation("normal_matrix"), 1, GL_TRUE, glm::value_ptr(normal));

	const MeshMaterial &mat = mesh.GetMaterial(entry.material_index);

	glUniform1f(GetUniformLocation("shininess"), mat.shininess);
	glUniform4fv(GetUniformLocation("blend_color"), 1, glm::value_ptr(blend));

	Texture *ambient = mat.texture_ambient.get();
	Texture *diffuse = mat.texture_diffuse.get();
	Texture *specular = mat.texture_specular.get();
	GLuint diffuse_id = 0;
	GLuint ambient_id = 0;
	GLuint specular_id = 0;

	if (diffuse != nullptr)
		diffuse_id = diffuse->GetTextureID();

	ambient_id = (ambient == nullptr) ? diffuse_id : ambient->GetTextureID();
	specular_id = (specular == nullptr) ? diffuse_id : specular->GetTextureID();

	diffuse_id = (diffuse_id != 0) ? diffuse_id : placeholder_tex;
	ambient_id = (ambient_id != 0) ? ambient_id : placeholder_tex;
	specular_id = (specular_id != 0) ? specular_id : placeholder_tex;

	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, diffuse_id);
	glActiveTexture(GL_TEXTURE0 + 1);
	glBindTexture(GL_TEXTURE_2D, specular_id);
	glActiveTexture(GL_TEXTURE0 + 2);
	glBindTexture(GL_TEXTURE_2D, ambient_id);

	glUniform1i(GetUniformLocation("diffuse_texture"), 0);
	glUniform1i(GetUniformLocation("specular_texture"), 1);
	glUniform1i(GetUniformLocation("ambient_texture"), 2);
}