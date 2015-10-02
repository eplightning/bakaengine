#ifndef LOSTISLANDS_OBJECT_FLOOR_H_
#define LOSTISLANDS_OBJECT_FLOOR_H_

#include "bakaglobal.h"
#include <GL/glew.h>

#include "game/scene/gameobject.h"
#include "game/scene/common_components.h"

namespace Baka
{
	namespace LiObjects
	{
		class Floor : public GameActor {
		public:
			explicit Floor(Scene* scene, Vec3 pos, int lives = 3);
			~Floor();

			VisibleComponent* GetVisibleComponent() override;
			PhysicalComponent* GetPhysicalComponent() override;

			void Update(double dt) override;
			const char* GetName() override;
			const char* GetID() override;
			void DecrementLife();

		private:
			SimpleVisibleComponent *visible_;
			StaticMeshPhysicalComponent *phys_;
			int lives_;
			int max_lives_;
		};
	}
}

#endif