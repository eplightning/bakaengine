#include "lostislands/scene/gameplay.h"

#include "bakaglobal.h"

#include "lostislands/object/floor.h"
#include "lostislands/object/torch.h"
#include "lostislands/object/treasure.h"
#include "lostislands/object/player.h"
#include "lostislands/object/floor_manager.h"
#include "game/core/registry.h"

using namespace Baka;

GameplayScene::GameplayScene() : Scene("game")
{
	Init();
}

void GameplayScene::Init()
{
	Registry::Instance()->GetInput()->SetMouseMode(InputMouseMode::Locked);

	// pierwsza podłoga
	Vec3 first_floor_position(0.f, 0.f, 10.f);
	LiObjects::Floor *first_floor = new LiObjects::Floor(this, first_floor_position);
	AddObject(first_floor);

	// określimy wielkości podłogi korzystając z wygenerowanego Bounding Boxa
	const Vec3 *floor_bb = first_floor->GetVisibleComponent()->GetMesh()->GetBounds();
	Vec3 floor_size(abs(floor_bb[7].x - floor_bb[0].x), abs(floor_bb[7].y - floor_bb[0].y), abs(floor_bb[7].z - floor_bb[0].z));

	// kolejne podłogi
	Vec2 floors[7];
	floors[0] = Vec2(0, 1);
	floors[1] = Vec2(0, 2);
	floors[2] = Vec2(0, 3);
	floors[3] = Vec2(1, 1);
	floors[4] = Vec2(-1, 2);
	floors[5] = Vec2(-2, 2);
	floors[6] = Vec2(-3, 2);

	for (int i = 0; i < sizeof(floors) / sizeof(Vec2); i++) {
		LiObjects::Floor *flr = new LiObjects::Floor(this, Vec3(
			first_floor_position.x - floors[i].x * floor_size.x,
			first_floor_position.y,
			first_floor_position.z + floors[i].y * floor_size.z
			));

		AddObject(flr);
	}

	// skarby
	Vec3 tr_initial_position(0.f, floor_size.y / 2, 10.f);

	Vec2 treasures[3];
	treasures[0] = Vec2(0, 3);
	treasures[1] = Vec2(1, 1);
	treasures[2] = Vec2(-3, 2);

	for (int i = 0; i < sizeof(treasures) / sizeof(Vec2); i++) {
		LiObjects::Treasure *treasure = new LiObjects::Treasure(this, Vec3(
			tr_initial_position.x - floor_size.x * treasures[i].x,
			tr_initial_position.y,
			tr_initial_position.z + floor_size.z * treasures[i].y
			));

		AddObject(treasure);
	}

	// gracz
	LiObjects::Player *player = new LiObjects::Player(this, Vec3(0.f, 10.f, 7.f));
	AddObject(player);

	// światło
	LiObjects::Torch *torch = new LiObjects::Torch(this, Vec3(1.f, 2.f, 3.f));
	torch->GetLightComponent()->GetDiffuseColor() = Vec3(0.2f, 0.3f, 0.4f);
	AddObject(torch);

	// usuwa podłoge
	AddObject(new LiObjects::FloorManager(this));
}