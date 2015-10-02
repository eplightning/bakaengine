#ifndef GAME_EVENT_EVENTDISPATCHERSCENE_H_
#define GAME_EVENT_EVENTDISPATCHERSCENE_H_

#include <vector>
#include <functional>
#include <unordered_map>

#include "bakaglobal.h"

#include "game/event/eventdispatcherbase.h"
#include "game/scene/gameobject.h"

namespace Baka
{
	class EventDispatcherSceneFunction {
	public:
		EventDispatcherSceneFunction(std::function<void(void*)> func, GameObject *obj) : function_(func), obj_(obj)
		{
		}

		std::function<void(void*)> function_;
		GameObject *obj_;
	};

	typedef std::vector<EventDispatcherSceneFunction> EventDispatcherSceneFunctions;

	class EventDispatcherScene : public EventDispatcherBase {
	public:
		EventDispatcherScene();

		void Broadcast(const String &name, void *parameter) const override;

		void Register(const String &name, GameObject *obj, std::function<void(void*)> fn);

		void OnObjectDeleted(GameObject *obj);
	private:
		std::unordered_map<String, EventDispatcherSceneFunctions> subscriptions_;
		std::unordered_map<GameObject*, std::vector<String>> object_to_events_;
	};
} // namespace Baka

#endif