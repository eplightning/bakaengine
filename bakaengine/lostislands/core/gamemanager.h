#ifndef LOSTISLANDS_CORE_GAMEMANAGER_H
#define LOSTISLANDS_CORE_GAMEMANAGER_H

#include "bakaglobal.h"

#include "game/scene/scene.h"
#include "graphics/renderer.h"
#include "physics/collisionengine.h"

namespace Baka
{
	class LiGameManager {
	public:
		LiGameManager();
		~LiGameManager();

		void FixedStep(double dt);
		void VariableStep(double dt, double accumulator);

		void OnReplaceScene(void *next_scene);

	private:
		Scene *scene_;
		Scene *next_scene_;
		Renderer *renderer_;
		CollisionEngine *collision_;
	};
}

#endif