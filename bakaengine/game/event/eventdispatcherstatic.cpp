/**
 * Event system for static objects
 */

#include "game/event/eventdispatcherstatic.h"

#include <vector>
#include <functional>
#include <unordered_map>

#include "bakaglobal.h"

using namespace Baka;

EventDispatcherStatic::EventDispatcherStatic() : subscriptions_()
{
}

void EventDispatcherStatic::Broadcast(const String &name, void *parameter) const
{
	auto it = subscriptions_.find(name);

	if (it == subscriptions_.end())
		return;

	for (auto &x : it->second) {
		x(parameter);
	}
}

void EventDispatcherStatic::Register(const String &name, std::function<void(void*)> fn)
{
	auto it = subscriptions_.find(name);

	if (it == subscriptions_.end()) {
		auto it2 = subscriptions_.emplace(std::piecewise_construct, std::forward_as_tuple(name), std::forward_as_tuple());

		if (!it2.second)
			return;

		it = it2.first;
	}

	it->second.push_back(fn);
}