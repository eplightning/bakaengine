#ifndef LOSTISLANDS_OBJECT_TORCH_H_
#define LOSTISLANDS_OBJECT_TORCH_H_

#include "bakaglobal.h"
#include <GL/glew.h>

#include "game/scene/gameobject.h"
#include "game/scene/common_components.h"

namespace Baka
{
	namespace LiObjects
	{
		class Torch : public GameActor {
		public:
			explicit Torch(Scene* scene, Vec3 pos);
			~Torch();

			LightComponent* GetLightComponent() override;
			const char* GetName() override;

		private:
			DirectionalLightComponent *light_;
		};
	}
}

#endif