#include "lostislands/object/floor_manager.h"

#include "physics/collisionengine.h"
#include "lostislands/object/floor.h"
#include "game/core/registry.h"
#include "game/scene/scene.h"

using namespace Baka;

LiObjects::FloorManager::FloorManager(Scene* scene) : GameObject(scene), prev_(nullptr), floors_()
{
	scene->GetDispatcher().Register("physics.StaticCollision", this, BIND_METHOD_CALL(LiObjects::FloorManager::OnStaticCollision, this));
}

LiObjects::FloorManager::~FloorManager()
{
}

void LiObjects::FloorManager::Update(double dt)
{
	if (floors_.size() == 1) {
		LiObjects::Floor *current = *(floors_.begin());

		if (prev_ != current) {
			current->DecrementLife();
			prev_ = current;
		}
	}

	floors_.clear();
}

const char* LiObjects::FloorManager::GetName()
{
	return "Floor Manager";
}

void LiObjects::FloorManager::OnStaticCollision(void* arg)
{
	StaticCollisionInfo *info = reinterpret_cast<StaticCollisionInfo*>(arg);

	if (strcmp(info->stat->GetGameObject()->GetID(), "floor") != 0 || strcmp(info->dynamic->GetGameObject()->GetID(), "player") != 0)
		return;

	LiObjects::Floor *floor = static_cast<LiObjects::Floor*>(info->stat->GetGameObject());

	if (floor->IsDying())
		return;

	floors_.insert(floor);
}