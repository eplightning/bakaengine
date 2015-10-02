#ifndef GAME_EVENT_EVENTDISPATCHERSTATIC_H_
#define GAME_EVENT_EVENTDISPATCHERSTATIC_H_

#include <vector>
#include <functional>
#include <unordered_map>

#include "bakaglobal.h"

#include "game/event/eventdispatcherbase.h"

namespace Baka
{
	typedef std::vector<std::function<void(void*)>> EventDispatcherStaticFunctions;

	class EventDispatcherStatic : public EventDispatcherBase {
	public:
		EventDispatcherStatic();

		void Broadcast(const String &name, void *parameter) const;

		void Register(const String &name, std::function<void(void*)> fn);
	private:
		std::unordered_map<String, EventDispatcherStaticFunctions> subscriptions_;
	};
} // namespace Baka

#endif