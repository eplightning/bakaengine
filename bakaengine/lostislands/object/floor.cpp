#include "lostislands/object/floor.h"

#include "game/scene/scene.h"

using namespace Baka;

LiObjects::Floor::Floor(Scene* scene, Vec3 pos, int lives) : GameActor(scene), lives_(lives), max_lives_(lives)
{
	visible_ = new SimpleVisibleComponent(this, scene, "models/floor.dae", "shaders/standard.glsl");
	phys_ = new StaticMeshPhysicalComponent(this, scene, visible_);
	position_ = pos;

	UpdateModelMatrix();
}

LiObjects::Floor::~Floor()
{
	delete visible_;
	delete phys_;
}

VisibleComponent* LiObjects::Floor::GetVisibleComponent()
{
	return visible_;
}

PhysicalComponent* LiObjects::Floor::GetPhysicalComponent()
{
	return phys_;
}

void LiObjects::Floor::Update(double dt)
{
	if (lives_ <= 0) {
		Vec3 velocity(0.f, -20.f, 0.f);

		velocity *= static_cast<float>(dt);

		position_ += velocity;

		UpdateModelMatrix();

		if (position_.y < -100.f)
			scene_->DeleteObject(this);
	}
}

const char* LiObjects::Floor::GetName()
{
	return "Floor";
}

const char* LiObjects::Floor::GetID()
{
	return "floor";
}

void LiObjects::Floor::DecrementLife()
{
	--lives_;

	float color = lives_ / (float)max_lives_;

	GetVisibleComponent()->GetColor() = Vec4(1.f, color, color, 1.f);
}