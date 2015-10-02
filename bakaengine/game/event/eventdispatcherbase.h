#ifndef GAME_EVENT_EVENTDISPATCHERBASE_H_
#define GAME_EVENT_EVENTDISPATCHERBASE_H_

#include "bakaglobal.h"

namespace Baka
{
	class EventDispatcherBase {
	public:
		virtual ~EventDispatcherBase()
		{
		}

		virtual void Broadcast(const String &name, void *parameter) const = 0;
	};
} // namespace Baka

#endif