#ifndef LOSTISLANDS_OBJECT_TREASURE_H_
#define LOSTISLANDS_OBJECT_TREASURE_H_

#include "bakaglobal.h"
#include <GL/glew.h>

#include "game/scene/gameobject.h"
#include "game/scene/common_components.h"

namespace Baka
{
	namespace LiObjects
	{
		class Treasure : public GameActor {
		public:
			explicit Treasure(Scene* scene, Vec3 pos);
			~Treasure();

			VisibleComponent* GetVisibleComponent() override;
			PhysicalComponent* GetPhysicalComponent() override;

			void Update(double dt) override;
			const char* GetName() override;

		private:
			class TreasureVisibleComponent : public SimpleVisibleComponent {
			public:
				TreasureVisibleComponent(GameObject* obj, Scene* scene, const String& mesh, const String& shader);
				~TreasureVisibleComponent();

				Mesh* GetMesh() const override;

			protected:
				Mesh *custom_mesh_;
			};

			class TreasurePhysicalComponent : public StaticMeshPhysicalComponent {
			public:
				TreasurePhysicalComponent(GameObject* obj, Scene* scene, VisibleComponent* visible);

				void OnUse(void *param);
				void OnTouch(PhysicalComponent* other) override;
				PhysicalComponentType GetType() const override;

			protected:
				bool using_;
			};

			TreasureVisibleComponent *visible_;
			TreasurePhysicalComponent *phys_;
		};
	}
}

#endif