#ifndef LOSTISLANDS_OBJECT_FLOORMANAGER_H_
#define LOSTISLANDS_OBJECT_FLOORMANAGER_H_

#include <set>

#include "bakaglobal.h"
#include <GL/glew.h>

#include "game/scene/gameobject.h"
#include "lostislands/object/floor.h"

namespace Baka
{
	namespace LiObjects
	{
		class FloorManager : public GameObject {
		public:
			explicit FloorManager(Scene* scene);
			~FloorManager();

			void Update(double dt) override;
			const char* GetName() override;

			void OnStaticCollision(void *arg);

		protected:
			std::set<LiObjects::Floor*> floors_;
			LiObjects::Floor* prev_;
		};
	}
}

#endif