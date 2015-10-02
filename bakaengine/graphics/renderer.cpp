#include "graphics/renderer.h"

#include <map>

#include "bakaglobal.h"
#include <GL/glew.h>

#include <glm/gtx/transform.hpp>

#include "game/scene/common_components.h"
#include "graphics/shader.h"
#include "graphics/mesh.h"
#include "game/core/registry.h"
#include "graphics/imagesupport.h"

using namespace Baka;

Renderer::Renderer(float fovy, float ar, float n, float f) : ambient_(0.1f, 0.1f, 0.1f)
{
	projection_ = glm::perspective(fovy, ar, n, f);
	placeholder_tex_ = Registry::Instance()->GetImageSupport()->LoadTexture("textures/placeholder.jpg");

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
}

void Renderer::RenderScene(Scene& scene)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Camera &cam = scene.GetCamera();

	// view matrix
	Mat4 view_matrix = glm::lookAt(cam.position, cam.point, cam.up);

	// gather info
	std::vector<DirectionalLightComponent*> directional_lights;
	std::vector<PointLightComponent*> point_lights;
	std::map<Shader*, std::vector<VisibleComponent*>> visible_by_shader;

	for (auto x : scene.GetVisibleComponents()) {
		Shader *shader = x->GetShader();

		if (shader != nullptr) {
			auto it = visible_by_shader.find(x->GetShader());

			if (it == visible_by_shader.end())
				it = visible_by_shader.emplace(std::piecewise_construct, std::forward_as_tuple(shader), std::forward_as_tuple()).first;

			it->second.push_back(x);
		}
	}

	for (auto x : scene.GetLightComponents()) {
		switch (x->GetType()) {
			case LightComponentType::Directional:
				directional_lights.push_back(static_cast<DirectionalLightComponent*>(x));
				break;

			case LightComponentType::Point:
				point_lights.push_back(static_cast<PointLightComponent*>(x));
				break;

			default:
				break;
		}
	}

	// render
	for (auto it = visible_by_shader.begin(); it != visible_by_shader.end(); ++it) {
		Shader *shader = it->first;

		shader->Enable();
		shader->SetupUniforms(view_matrix, projection_, ambient_, directional_lights, point_lights);

		for (auto x : it->second) {
			Mesh *mesh = x->GetMesh();

			if (mesh == nullptr)
				continue;

			glBindVertexArray(mesh->GetVAO());

			const MeshEntries &entries = mesh->GetEntries();
			int i = 0;

			const Mat4 *generic_model_matrix = x->GetModelMatrix(-1);
			Mat4 generic_normal_matrix = glm::inverse(view_matrix * *generic_model_matrix);
			Vec4 &blend = x->GetColor();

			for (auto it2 = entries.begin(); it2 != entries.end(); ++it2, ++i) {
				const Mat4 *model_matrix = x->GetModelMatrix(i);

				if (model_matrix != nullptr) {
					shader->SetupUniformsForMesh(blend, *model_matrix, glm::inverse(view_matrix * *model_matrix), *mesh, *it2, placeholder_tex_);
				} else {
					shader->SetupUniformsForMesh(blend, *generic_model_matrix, generic_normal_matrix, *mesh, *it2, placeholder_tex_);
				}

				glDrawElementsBaseVertex(GL_TRIANGLES, it2->vertex_count, GL_UNSIGNED_INT,
					reinterpret_cast<void*>(sizeof(uint) * it2->index_base), it2->vertex_base);
			}
		}
	}

	glBindVertexArray(0);
}

Vec3& Renderer::GetAmbient()
{
	return ambient_;
}