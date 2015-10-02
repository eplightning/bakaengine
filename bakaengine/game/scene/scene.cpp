#include "game/scene/scene.h"

#include <algorithm>
#include <set>
#include <vector>

#include "bakaglobal.h"

#include "game/core/registry.h"
#include "game/event/eventdispatcherscene.h"

using namespace Baka;

Camera::Camera() : position(0.f, 0.f, 0.f), point(0.f, 0.f, -10.f), up(0.f, 1.f, 0.f)
{
}

Scene::Scene(const String &name) : objects_(), physical_(), visible_(), deletion_(), name_(name), cache_()
{
	dispatcher_ = new EventDispatcherScene();

	Registry::Instance()->GetEventManager()->RegisterDispatcher(name, dispatcher_);
}

Scene::~Scene()
{
	Registry::Instance()->GetEventManager()->UnregisterDispatcher(name_, dispatcher_);

	for (auto &x : objects_) {
		delete x;
	}
}

void Scene::AddObject(GameObject *obj)
{
	objects_.emplace_back(obj);

	VisibleComponent *visible = obj->GetVisibleComponent();
	PhysicalComponent *physical = obj->GetPhysicalComponent();
	LightComponent *light = obj->GetLightComponent();

	if (visible != nullptr)
		visible_.push_back(visible);

	if (physical != nullptr)
		physical_.push_back(physical);

	if (light != nullptr)
		light_.push_back(light);
}

void Scene::DeleteObject(GameObject *obj)
{
	deletion_.emplace(obj);
	obj->IsDying(true);
}

EventDispatcherScene& Scene::GetDispatcher()
{
	return *dispatcher_;
}

SceneObjectsList& Scene::GetObjects()
{
	return objects_;
}

PhysicalComponentsList& Scene::GetPhysicalComponents()
{
	return physical_;
}

VisibleComponentsList& Scene::GetVisibleComponents()
{
	return visible_;
}

LightComponentsList& Scene::GetLightComponents()
{
	return light_;
}

Camera& Scene::GetCamera()
{
	return camera_;
}

ObjCache& Scene::GetCache()
{
	return cache_;
}

void Scene::PerformDeletion()
{
	for (auto &x : deletion_) {
		dispatcher_->OnObjectDeleted(x);

		PhysicalComponent *physical = x->GetPhysicalComponent();

		if (physical != nullptr) {
			auto it = std::find(physical_.begin(), physical_.end(), physical);

			if (it != physical_.end())
				physical_.erase(it);
		}

		VisibleComponent *visible = x->GetVisibleComponent();

		if (visible != nullptr) {
			auto it = std::find(visible_.begin(), visible_.end(), visible);

			if (it != visible_.end())
				visible_.erase(it);
		}

		LightComponent *light = x->GetLightComponent();

		if (light != nullptr) {
			auto it = std::find(light_.begin(), light_.end(), light);

			if (it != light_.end())
				light_.erase(it);
		}

		delete x;

		auto it = std::find(objects_.begin(), objects_.end(), x);

		if (it != objects_.end())
			objects_.erase(it);
	}

	deletion_.clear();
}