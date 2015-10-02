#ifndef GAME_SCENE_GAMEOBJECT_H
#define GAME_SCENE_GAMEOBJECT_H

#include "bakaglobal.h"
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include "game/scene/component.h"

namespace Baka
{
	class Scene;

	class GameObject {
	public:
		explicit GameObject(Scene *scene);
		virtual ~GameObject();

		virtual const char* GetName();
		virtual const char* GetID();
		virtual VisibleComponent* GetVisibleComponent();
		virtual PhysicalComponent* GetPhysicalComponent();
		virtual LightComponent* GetLightComponent();

		virtual bool IsActor() const;

		virtual void Update(double dt);

		bool IsDying(bool die = false);

	protected:
		Scene *scene_;
		bool dying_;
	};

	class GameActor : public GameObject {
	public:
		explicit GameActor(Scene *scene);

		const char* GetName() override;
		const char* GetID() override;
		glm::vec3& GetPosition();
		glm::vec3& GetRotation();
		float& GetScale();
		const Mat4& GetModelMatrix() const;

		bool IsActor() const override;

		void UpdateModelMatrix();

	protected:
		virtual Mat4 CalculateModelMatrix() const;

		Vec3 position_;
		Vec3 rotation_;
		float scale_;
		Mat4 model_matrix_;
	};
}

#endif