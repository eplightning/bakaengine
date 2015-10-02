#ifndef GAME_SCENE_COMPONENT_H
#define GAME_SCENE_COMPONENT_H

#include "bakaglobal.h"

#include "graphics/mesh.h"
#include "graphics/shader.h"

namespace Baka
{
	class GameObject;
	class GameActor;
	class Scene;

	class Component {
	public:
		Component(GameObject *obj, Scene *scene);
		virtual ~Component();

		virtual void Update(double dt);
		GameObject *GetGameObject();

	protected:
		GameObject *obj_;
		GameActor *actor_;
		Scene *scene_;
	};

	class VisibleComponent : public Component {
	public:
		VisibleComponent(GameObject *obj, Scene *scene);
		~VisibleComponent();

		virtual Shader* GetShader() const = 0;
		virtual Mesh* GetMesh() const = 0;
		virtual Vec4& GetColor();
		virtual const Mat4* GetModelMatrix(int elem) const;

	protected:
		Vec4 color_;
	};

	enum class PhysicalComponentType {
		Unknown = 0,
		Static = 1,
		Dynamic = 2,
		StaticIgnored = 3
	};

	class PhysicalComponent : public Component {
	public:
		PhysicalComponent(GameObject *obj, Scene *scene);
		~PhysicalComponent();

		virtual const Vec3* GetBoundingBox() const = 0;
		virtual const Mat4* GetModelMatrix() const;
		virtual PhysicalComponentType GetType() const;

		virtual void OnTouch(PhysicalComponent *other);
		virtual void OnStaticCollide(PhysicalComponent *other, Vec3 &mtv);
	};

	enum class LightComponentType {
		Unknown = 0,
		Directional = 1,
		Point = 2
	};

	class LightComponent : public Component {
	public:
		LightComponent(GameObject *obj, Scene *scene);
		~LightComponent();
		virtual LightComponentType GetType() const;

		virtual const Vec3 &GetDiffuseColor() const;
		virtual const Vec3 &GetSpecularColor() const;
		virtual Vec3 &GetDiffuseColor();
		virtual Vec3 &GetSpecularColor();

	protected:
		Vec3 diffuse_color_;
		Vec3 specular_color_;
	};
}

#endif