#include "game/core/registry.h"

#include <unordered_map>

#include "bakaglobal.h"

using namespace Baka;

Registry *Registry::instance_ = nullptr;

Registry::Registry() : objects_()
{
	instance_ = this;
	obj_eventmanager_ = nullptr;
	obj_mainconf_ = nullptr;
	obj_logger_ = nullptr;
	obj_resources_ = nullptr;
	obj_imgsupport_ = nullptr;
	obj_actions_ = nullptr;
	obj_input_ = nullptr;
}

Registry::~Registry()
{
	// service locator has exclusive ownership of all objects in his map
	for (auto &obj : objects_) {
		delete obj.second;
	}
}

void* Registry::Get(const String &name) const
{
	auto obj = objects_.find(name);

	if (obj == objects_.end()) {
		return nullptr;
	} else {
		return obj->second;
	}
}

void Registry::Set(const String &name, void *object)
{
	void *old = Get(name);

	if (old != nullptr)
		delete old;

	objects_[name] = object;

	// TODO: Maybe check if we're changing non-generic service and update pointer
}

void Registry::SetEventManager(EventManager *object)
{
	if (obj_eventmanager_ != nullptr)
		delete obj_eventmanager_;

	obj_eventmanager_ = object;
	objects_["event_manager"] = object;
}

void Registry::SetConfig(Config *object)
{
	if (obj_mainconf_ != nullptr)
		delete obj_mainconf_;

	obj_mainconf_ = object;
	objects_["main_config"] = object;
}

void Registry::SetLogger(Logger *object)
{
	if (obj_logger_ != nullptr)
		delete obj_logger_;

	obj_logger_ = object;
	objects_["logger"] = object;
}

void Registry::SetResources(Resources *object)
{
	if (obj_resources_ != nullptr)
		delete obj_resources_;

	obj_resources_ = object;
	objects_["resources"] = object;
}

void Registry::SetImageSupport(ImageSupport *object)
{
	if (obj_imgsupport_ != nullptr)
		delete obj_imgsupport_;

	obj_imgsupport_ = object;
	objects_["imgsupport"] = object;
}

void Registry::SetActions(Actions *object)
{
	if (obj_actions_ != nullptr)
		delete obj_actions_;

	obj_actions_ = object;
	objects_["actions"] = object;
}

void Registry::SetInput(Input *object)
{
	if (obj_input_ != nullptr)
		delete obj_input_;

	obj_input_ = object;
	objects_["input"] = object;
}