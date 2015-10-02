#ifndef GAME_SCENE_COMMON_COMPONENTS_H
#define GAME_SCENE_COMMON_COMPONENTS_H

#include "bakaglobal.h"

#include "game/scene/component.h"
#include "graphics/shader.h"
#include "graphics/mesh.h"

namespace Baka
{
	class DirectionalLightComponent : public LightComponent {
	public:
		DirectionalLightComponent(GameObject *obj, Scene *scene, Vec3 direction);
		Vec3& GetDirection();
		LightComponentType GetType() const override;

	protected:
		Vec3 direction_;
	};

	class PointLightComponent : public LightComponent {
	public:
		PointLightComponent(GameObject *obj, Scene *scene, float intensity = 3.f);
		const Vec3& GetPosition();
		float GetIntensity() const;
		LightComponentType GetType() const override;

	protected:
		float intensity_;
	};

	class SimpleVisibleComponent : public VisibleComponent {
	public:
		SimpleVisibleComponent(GameObject *obj, Scene *scene, const String &mesh, const String &shader);

		Shader* GetShader() const override;
		Mesh* GetMesh() const override;

	protected:
		SharedPtr<Shader> shader_;
		SharedPtr<Mesh> mesh_;
	};

	class StaticMeshPhysicalComponent : public PhysicalComponent {
	public:
		StaticMeshPhysicalComponent(GameObject *obj, Scene *scene, VisibleComponent *visible);
		const Vec3* GetBoundingBox() const override;
		PhysicalComponentType GetType() const override;

		void UpdateVisibleComponent(VisibleComponent *comp);

	protected:
		VisibleComponent *visible_;
	};
}

#endif