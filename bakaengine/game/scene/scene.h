#ifndef GAME_SCENE_SCENE_H
#define GAME_SCENE_SCENE_H

#include <set>
#include <vector>

#include "bakaglobal.h"
#include <glm/vec3.hpp>

#include "game/scene/gameobject.h"
#include "game/scene/component.h"
#include "game/event/eventdispatcherscene.h"
#include "graphics/objcache.h"

namespace Baka
{
	typedef std::vector<GameObject*> SceneObjectsList;
	typedef std::vector<VisibleComponent*> VisibleComponentsList;
	typedef std::vector<PhysicalComponent*> PhysicalComponentsList;
	typedef std::vector<LightComponent*> LightComponentsList;
	typedef std::set<GameObject*> ObjectDeletionSchedule;

	class Camera {
	public:
		Camera();

		glm::vec3 position;
		glm::vec3 point;
		glm::vec3 up;
	};

	class Scene {
	public:
		explicit Scene(const String &name);
		virtual ~Scene();

		virtual void AddObject(GameObject *obj);
		virtual void DeleteObject(GameObject *obj);

		virtual SceneObjectsList& GetObjects();
		virtual PhysicalComponentsList& GetPhysicalComponents();
		virtual VisibleComponentsList& GetVisibleComponents();
		virtual LightComponentsList& GetLightComponents();
		virtual EventDispatcherScene& GetDispatcher();
		virtual Camera& GetCamera();
		virtual ObjCache& GetCache();

		virtual void PerformDeletion();

	protected:
		SceneObjectsList objects_;
		PhysicalComponentsList physical_;
		VisibleComponentsList visible_;
		LightComponentsList light_;
		ObjectDeletionSchedule deletion_;
		String name_;
		EventDispatcherScene *dispatcher_;
		Camera camera_;
		ObjCache cache_;
	};
}

#endif