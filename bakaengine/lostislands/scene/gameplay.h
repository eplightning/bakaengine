#ifndef LOSTISLANDS_SCENE_GAMEPLAY_H_
#define LOSTISLANDS_SCENE_GAMEPLAY_H_

#include "bakaglobal.h"
#include <GL/glew.h>

#include "game/scene/scene.h"

namespace Baka
{
	class GameplayScene : public Scene {
	public:
		GameplayScene();

	private:
		void Init();
	};
}

#endif