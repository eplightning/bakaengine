#include "game/event/eventmanager.h"

#include <unordered_map>

#include "bakaglobal.h"

#include "game/event/eventdispatcherbase.h"
#include "game/core/registry.h"
#include "logging/logger.h"

using namespace Baka;

EventManager::EventManager() : dispatchers_()
{
}

EventManager::~EventManager()
{
	for (auto &x : dispatchers_) {
		delete x.second;
	}
}

void EventManager::Broadcast(const String &ev, void *parameter) const
{
	for (auto &x : dispatchers_) {
		x.second->Broadcast(ev, parameter);
	}
}

EventDispatcherBase* EventManager::GetDispatcher(const String &name) const
{
	auto it = dispatchers_.find(name);

	if (it == dispatchers_.end())
		return nullptr;

	return it->second;
}

void EventManager::RegisterDispatcher(const String &name, EventDispatcherBase *obj)
{
	dispatchers_[name] = obj;

	LOG_INFO("EventManager: Registered new event dispatcher: %s", name.c_str());
}

void EventManager::UnregisterDispatcher(const String &name, EventDispatcherBase *obj)
{
	auto it = dispatchers_.find(name);

	if (it != dispatchers_.end()) {
		if (obj != nullptr) {
			if ((*it).second != obj)
				return;
		}

		dispatchers_.erase(it);
	}
}