#include "lostislands/core/gamemanager.h"

#include "bakaglobal.h"
#include <GL/glew.h>

#include "logging/logger.h"
#include "game/scene/scene.h"
#include "game/event/eventmanager.h"
#include "game/core/registry.h"
#include "game/scene/gameobject.h"
#include "lostislands/scene/gameplay.h"

using namespace Baka;

LiGameManager::LiGameManager()
{
	scene_ = nullptr;
	next_scene_ = nullptr;

	Registry::Instance()->GetEventManager()->GetDispatcherStatic()
		->Register("game.manager.ReplaceScene", BIND_METHOD_CALL(LiGameManager::OnReplaceScene, this));

	Registry::Instance()->GetInput()->SetMouseMode(InputMouseMode::NormalAbsolute);

	Config *conf = Registry::Instance()->GetConfig();
	FramebufferSize size = static_cast<Window*>(Registry::Instance()->Get("window"))->GetFbSize();

	renderer_ = new Renderer(static_cast<float>(conf->GetDouble("Camera", "FOV", 30.f)),
		size.width / static_cast<float>(size.height), 0.1f, 100.f);

	renderer_->GetAmbient() = Vec3(0.1f, 0.1f, 0.1f);

	collision_ = new CollisionEngine;

	scene_ = new GameplayScene;
}

LiGameManager::~LiGameManager()
{
	if (scene_ != nullptr)
		delete scene_;

	if (next_scene_ != nullptr)
		delete next_scene_;

	delete renderer_;
}

void LiGameManager::FixedStep(double dt)
{
	// tick
	for (auto &x : scene_->GetObjects()) {
		x->Update(dt);
	}

	collision_->Tick(*scene_);

	scene_->PerformDeletion();
}

void LiGameManager::VariableStep(double dt, double accumulator)
{
	renderer_->RenderScene(*scene_);

	// next scene replacement
	if (next_scene_ != nullptr) {
		if (scene_ != nullptr)
			delete scene_;

		scene_ = next_scene_;
		next_scene_ = nullptr;
	}
}

void LiGameManager::OnReplaceScene(void *next_scene)
{
	if (next_scene != nullptr) {
		Scene *scn = static_cast<Scene*>(next_scene);

		if (next_scene_ != nullptr)
			delete next_scene_;

		next_scene_ = scn;
	}
}