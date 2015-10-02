#include "game/scene/component.h"

#include "bakaglobal.h"

#include "game/scene/gameobject.h"

using namespace Baka;

Component::Component(GameObject* obj, Scene* scene)
{
	obj_ = obj;
	scene_ = scene;
	actor_ = obj->IsActor() ? static_cast<GameActor*>(obj) : nullptr;
}

Component::~Component()
{
}

void Component::Update(double dt)
{
}

GameObject* Component::GetGameObject()
{
	return obj_;
}

VisibleComponent::VisibleComponent(GameObject* obj, Scene* scene) : Component(obj, scene), color_(1.f, 1.f, 1.f, 1.f)
{
	if (actor_ == nullptr)
		throw std::logic_error("VisibleComponent needs to be a part of an actor");
}

VisibleComponent::~VisibleComponent()
{
}

const Mat4* VisibleComponent::GetModelMatrix(int elem) const
{
	if (elem == -1)
		return &actor_->GetModelMatrix();

	return nullptr;
}

Vec4& VisibleComponent::GetColor()
{
	return color_;
}

PhysicalComponent::PhysicalComponent(GameObject* obj, Scene* scene) : Component(obj, scene)
{
	if (actor_ == nullptr)
		throw std::logic_error("PhysicalComponent needs to be a part of an actor");
}

PhysicalComponent::~PhysicalComponent()
{
}

const Mat4* PhysicalComponent::GetModelMatrix() const
{
	return &actor_->GetModelMatrix();
}

PhysicalComponentType PhysicalComponent::GetType() const
{
	return PhysicalComponentType::Unknown;
}

void PhysicalComponent::OnTouch(PhysicalComponent* other)
{
}

void PhysicalComponent::OnStaticCollide(PhysicalComponent* other, Vec3& mtv)
{
}

LightComponent::LightComponent(GameObject* obj, Scene* scene) : Component(obj, scene), diffuse_color_(1.f, 1.f, 1.f), specular_color_(1.f, 1.f, 1.f)
{
}

LightComponent::~LightComponent()
{
}

LightComponentType LightComponent::GetType() const
{
	return LightComponentType::Unknown;
}

const Vec3& LightComponent::GetDiffuseColor() const
{
	return diffuse_color_;
}

const Vec3& LightComponent::GetSpecularColor() const
{
	return specular_color_;
}

Vec3& LightComponent::GetDiffuseColor()
{
	return diffuse_color_;
}

Vec3& LightComponent::GetSpecularColor()
{
	return specular_color_;
}