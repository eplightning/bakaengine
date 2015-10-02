/**
 * Event system for static objects
 */

#include "game/event/eventdispatcherscene.h"

#include <vector>
#include <functional>
#include <unordered_map>

#include "bakaglobal.h"

#include "game/scene/gameobject.h"

using namespace Baka;

EventDispatcherScene::EventDispatcherScene() : subscriptions_(), object_to_events_()
{
}

void EventDispatcherScene::Broadcast(const String &name, void *parameter) const
{
	auto it = subscriptions_.find(name);

	if (it == subscriptions_.end())
		return;

	for (auto &x : it->second) {
		x.function_(parameter);
	}
}

void EventDispatcherScene::Register(const String &name, GameObject *obj, std::function<void(void*)> fn)
{
	auto it = subscriptions_.find(name);

	if (it == subscriptions_.end()) {
		auto it2 = subscriptions_.emplace(std::piecewise_construct, std::forward_as_tuple(name), std::forward_as_tuple());

		if (!it2.second)
			return;

		it = it2.first;
	}

	it->second.emplace_back(fn, obj);

	auto it3 = object_to_events_.find(obj);

	if (it3 == object_to_events_.end())
		it3 = object_to_events_.emplace(std::piecewise_construct, std::forward_as_tuple(obj), std::forward_as_tuple()).first;

	it3->second.push_back(name);
}

void EventDispatcherScene::OnObjectDeleted(GameObject *obj)
{
	auto it = object_to_events_.find(obj);

	if (it == object_to_events_.end())
		return;

	for (auto &x : it->second) {
		auto it2 = subscriptions_.find(x);

		if (it2 == subscriptions_.end())
			continue;

		auto it3 = it2->second.begin();

		while (it3 != it2->second.end()) {
			if (it3->obj_ == obj) {
				it3 = it2->second.erase(it3);
			} else {
				++it3;
			}
		}
	}

	object_to_events_.erase(it);
}