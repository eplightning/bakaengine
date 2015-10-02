#include "lostislands/object/player.h"

#include <glm/gtx/rotate_vector.hpp>

#include "game/scene/scene.h"
#include "game/core/registry.h"
#include "game/core/input.h"
#include <lostislands/scene/gameplay.h>

using namespace Baka;

LiObjects::Player::Player(Scene* scene, Vec3 position) : GameActor(scene), xang_(0.f), yang_(0.f), dir_(0.f, 0.f, 1.f), forward_dir_(0.f, 0.f, 1.f),
updown_(0), leftright_(0), resetting_(false), speed_(6)
{
	position_ = position;
	light_ = new PointLightComponent(this, scene);
	phys_ = new PlayerPhysicalComponent(this, scene);

	EventDispatcherScene &disp = scene->GetDispatcher();

	disp.Register("input.MouseMove", this, BIND_METHOD_CALL(LiObjects::Player::OnMouseMove, this));
	disp.Register("action.UpDown", this, BIND_METHOD_CALL(LiObjects::Player::OnUp, this));
	disp.Register("action.DownDown", this, BIND_METHOD_CALL(LiObjects::Player::OnDown, this));
	disp.Register("action.LeftDown", this, BIND_METHOD_CALL(LiObjects::Player::OnLeft, this));
	disp.Register("action.RightDown", this, BIND_METHOD_CALL(LiObjects::Player::OnRight, this));
	disp.Register("action.UpUp", this, BIND_METHOD_CALL(LiObjects::Player::OnUp, this));
	disp.Register("action.DownUp", this, BIND_METHOD_CALL(LiObjects::Player::OnDown, this));
	disp.Register("action.LeftUp", this, BIND_METHOD_CALL(LiObjects::Player::OnLeft, this));
	disp.Register("action.RightUp", this, BIND_METHOD_CALL(LiObjects::Player::OnRight, this));
	disp.Register("action.SpeedUp", this, BIND_METHOD_CALL(LiObjects::Player::OnSpeed, this));
	disp.Register("action.SpeedDown", this, BIND_METHOD_CALL(LiObjects::Player::OnSpeed, this));

	last_coords_ = Registry::Instance()->GetInput()->GetCurrentCoords();
}

LiObjects::Player::~Player()
{
	delete light_;
	delete phys_;
}

LightComponent* LiObjects::Player::GetLightComponent()
{
	return light_;
}

PhysicalComponent* LiObjects::Player::GetPhysicalComponent()
{
	return phys_;
}

void LiObjects::Player::Update(double dt)
{
	if (updown_ == 1)
		position_ += forward_dir_ * static_cast<float>(dt * speed_);

	if (updown_ == 2)
		position_ -= forward_dir_ * static_cast<float>(dt * speed_);

	if (leftright_ == 1)
		position_ -= glm::cross(forward_dir_, scene_->GetCamera().up) * static_cast<float>(dt * speed_);

	if (leftright_ == 2)
		position_ += glm::cross(forward_dir_, scene_->GetCamera().up) * static_cast<float>(dt * speed_);

	scene_->GetCamera().position = position_;
	scene_->GetCamera().point = position_ + dir_;

	phys_->Update(dt);

	if (position_.y < -25.f && !resetting_) {
		Registry::Instance()->GetEventManager()->Broadcast("game.manager.ReplaceScene", new GameplayScene);
		resetting_ = true;
	}
}

const char* LiObjects::Player::GetName()
{
	return "Player";
}

const char* LiObjects::Player::GetID()
{
	return "player";
}

void LiObjects::Player::OnMouseMove(void* param)
{
	InputMouseCoords *coords = static_cast<InputMouseCoords*>(param);

	float xang = static_cast<float>((coords->x - last_coords_.x) / -10);
	float yang = static_cast<float>((coords->y - last_coords_.y) / 10);

	xang_ += xang;
	yang_ += yang;

	// fixup
	if (xang_ >= 360.f) {
		xang_ -= 360.f;
	} else if (xang_ <= -360.f) {
		xang_ += 360.f;
	}

	if (yang_ >= 89.f) {
		yang_ = 89.9f;
	} else if (yang_ <= -89.f) {
		yang_ = -89.f;
	}

	dir_ = Vec3(0.f, 0.f, 1.f);
	forward_dir_ = Vec3(0.f, 0.f, 1.f);
	dir_ = glm::rotate(dir_, glm::radians(yang_), Vec3(1.f, 0.f, 0.f));
	dir_ = glm::rotate(dir_, glm::radians(xang_), Vec3(0.f, 1.f, 0.f));
	forward_dir_ = glm::rotate(forward_dir_, glm::radians(xang_), Vec3(0.f, 1.f, 0.f));

	last_coords_.x = coords->x;
	last_coords_.y = coords->y;
}

void LiObjects::Player::OnUp(void* param)
{
	int down = reinterpret_cast<bool>(param);

	updown_ = down ? 1 : (updown_ == 1 ? 0 : updown_);
}

void LiObjects::Player::OnDown(void* param)
{
	bool down = reinterpret_cast<bool>(param);

	updown_ = down ? 2 : (updown_ == 2 ? 0 : updown_);
}

void LiObjects::Player::OnLeft(void* param)
{
	bool down = reinterpret_cast<bool>(param);

	leftright_ = down ? 1 : (leftright_ == 1 ? 0 : leftright_);
}

void LiObjects::Player::OnRight(void* param)
{
	bool down = reinterpret_cast<bool>(param);

	leftright_ = down ? 2 : (leftright_ == 2 ? 0 : leftright_);
}

void LiObjects::Player::OnSpeed(void* param)
{
	bool down = reinterpret_cast<bool>(param);

	speed_ = down ? 10 : 6;
}

LiObjects::Player::PlayerPhysicalComponent::PlayerPhysicalComponent(GameObject* obj, Scene* scene) : PhysicalComponent(obj, scene)
{
	Vec3 maxpos(1.f, 2.f, 0.5f);
	Vec3 minpos(-1.f, -2.f, -0.5f);

	bounds_[0] = Vec3(minpos.x, minpos.y, minpos.z); // xyz
	bounds_[1] = Vec3(maxpos.x, minpos.y, minpos.z); // Xyz
	bounds_[2] = Vec3(minpos.x, maxpos.y, minpos.z); // xYz
	bounds_[3] = Vec3(maxpos.x, maxpos.y, minpos.z); // XYz
	bounds_[4] = Vec3(minpos.x, minpos.y, maxpos.z); // xyZ
	bounds_[5] = Vec3(maxpos.x, minpos.y, maxpos.z); // XyZ
	bounds_[6] = Vec3(minpos.x, maxpos.y, maxpos.z); // xYZ
	bounds_[7] = Vec3(maxpos.x, maxpos.y, maxpos.z); // XYZ

	scene->GetDispatcher().Register("actions.Jump", obj, BIND_METHOD_CALL(LiObjects::Player::PlayerPhysicalComponent::OnJump, this));
}

void LiObjects::Player::PlayerPhysicalComponent::Update(double dt)
{
	Vec3 velocity(0.f, -10.f, 0.f);

	Vec3 &pos = actor_->GetPosition();

	velocity *= static_cast<float>(dt);

	pos += velocity;

	actor_->UpdateModelMatrix();
}

const Vec3* LiObjects::Player::PlayerPhysicalComponent::GetBoundingBox() const
{
	return bounds_;
}

PhysicalComponentType LiObjects::Player::PlayerPhysicalComponent::GetType() const
{
	return PhysicalComponentType::Dynamic;
}

void LiObjects::Player::PlayerPhysicalComponent::OnStaticCollide(PhysicalComponent* other, Vec3& mtv)
{
	Vec3 &pos = actor_->GetPosition();

	pos += mtv;

	actor_->UpdateModelMatrix();
}

void LiObjects::Player::PlayerPhysicalComponent::OnJump(void* param)
{
}