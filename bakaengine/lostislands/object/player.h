#ifndef LOSTISLANDS_OBJECT_PLAYER_H_
#define LOSTISLANDS_OBJECT_PLAYER_H_

#include "bakaglobal.h"
#include <GL/glew.h>

#include "game/scene/gameobject.h"
#include "game/scene/common_components.h"
#include "game/core/input.h"

namespace Baka
{
	namespace LiObjects
	{
		class Player : public GameActor {
		public:
			Player(Scene *scene, Vec3 position);

			~Player() override;
			LightComponent* GetLightComponent() override;
			PhysicalComponent* GetPhysicalComponent() override;
			void Update(double dt) override;
			const char* GetName() override;
			const char* GetID() override;
			void OnMouseMove(void *param);
			void OnUp(void *param);
			void OnDown(void *param);
			void OnLeft(void *param);
			void OnRight(void *param);
			void OnSpeed(void *param);

		protected:
			PointLightComponent *light_;
			Vec3 dir_;
			Vec3 forward_dir_;
			int updown_;
			int leftright_;
			InputMouseCoords last_coords_;
			float xang_;
			float yang_;
			bool resetting_;
			float speed_;

			class PlayerPhysicalComponent : public PhysicalComponent {
			public:
				PlayerPhysicalComponent(GameObject *obj, Scene *scene);

				void Update(double dt) override;
				const Vec3* GetBoundingBox() const override;
				PhysicalComponentType GetType() const override;
				void OnStaticCollide(PhysicalComponent* other, Vec3& mtv) override;
				void OnJump(void *param);

			protected:
				Vec3 bounds_[8];
				Vec3 velocity_;
				Vec3 acceleration_;
			};

			PlayerPhysicalComponent *phys_;
		};
	}
}

#endif