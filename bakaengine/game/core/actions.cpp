#include "game/core/actions.h"

#include "bakaglobal.h"

#include "game/event/eventmanager.h"
#include "game/core/registry.h"
#include "logging/logger.h"

using namespace Baka;

void Actions::Execute(const String &action, bool down)
{
	LOG_DEBUG("Actions: Executing %s", action.c_str());

	char front = action.front();

	String event_name("action.");

	if (front == '+' || front == '-') {
		event_name.append(action.cbegin() + 1, action.cend());
		event_name.append(down ? "Down" : "Up");
	} else {
		event_name.append(action.cbegin(), action.cend());
	}

	Registry::Instance()->GetEventManager()->Broadcast(event_name, reinterpret_cast<void*>(down));
}