#ifndef GAME_EVENT_EVENTMANAGER_H_
#define GAME_EVENT_EVENTMANAGER_H_

#include <unordered_map>

#include "bakaglobal.h"

#include "game/event/eventdispatcherbase.h"
#include "game/event/eventdispatcherstatic.h"

namespace Baka
{
	class EventManager {
	public:
		EventManager();
		~EventManager();

		void Broadcast(const String &ev, void *parameter) const;

		EventDispatcherBase* GetDispatcher(const String &name) const;

		void RegisterDispatcher(const String &name, EventDispatcherBase *obj);
		void UnregisterDispatcher(const String &name, EventDispatcherBase *obj = nullptr);

		// laziness
		inline EventDispatcherStatic* GetDispatcherStatic() const
		{
			return static_cast<EventDispatcherStatic*>(GetDispatcher("static"));
		}

	private:
		std::unordered_map<String, EventDispatcherBase*> dispatchers_;
	};
} // namespace Baka

#endif
