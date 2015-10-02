#include "game/scene/common_components.h"

#include "bakaglobal.h"

#include <glm/gtx/norm.hpp>

#include "game/scene/gameobject.h"
#include "graphics/objcache.h"
#include "game/scene/scene.h"

using namespace Baka;

DirectionalLightComponent::DirectionalLightComponent(GameObject* obj, Scene* scene, Vec3 direction) :
LightComponent(obj, scene)
{
	direction_ = glm::normalize(direction);
}

Vec3& DirectionalLightComponent::GetDirection()
{
	return direction_;
}

LightComponentType DirectionalLightComponent::GetType() const
{
	return LightComponentType::Directional;
}

PointLightComponent::PointLightComponent(GameObject* obj, Scene* scene, float intensity) : LightComponent(obj, scene), intensity_(intensity)
{
	if (actor_ == nullptr)
		throw std::logic_error("PointLightComponent needs to be a part of an actor");
}

const Vec3& PointLightComponent::GetPosition()
{
	return actor_->GetPosition();
}

float PointLightComponent::GetIntensity() const
{
	return intensity_;
}

LightComponentType PointLightComponent::GetType() const
{
	return LightComponentType::Point;
}

SimpleVisibleComponent::SimpleVisibleComponent(GameObject* obj, Scene* scene, const String& mesh, const String& shader) :
VisibleComponent(obj, scene)
{
	ObjCache &cache = scene->GetCache();

	if (!mesh.empty())
		mesh_ = cache.GetMesh(mesh);

	if (!shader.empty())
		shader_ = cache.GetShader(shader);
}

Shader* SimpleVisibleComponent::GetShader() const
{
	return shader_.get();
}

Mesh* SimpleVisibleComponent::GetMesh() const
{
	return mesh_.get();
}

StaticMeshPhysicalComponent::StaticMeshPhysicalComponent(GameObject* obj, Scene* scene, VisibleComponent* visible) : PhysicalComponent(obj, scene)
{
	UpdateVisibleComponent(visible);
}

const Vec3* StaticMeshPhysicalComponent::GetBoundingBox() const
{
	return visible_->GetMesh()->GetBounds();
}

PhysicalComponentType StaticMeshPhysicalComponent::GetType() const
{
	return PhysicalComponentType::Static;
}

void StaticMeshPhysicalComponent::UpdateVisibleComponent(VisibleComponent* comp)
{
	visible_ = comp;
}