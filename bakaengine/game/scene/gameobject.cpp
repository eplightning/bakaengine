#include "game/scene/gameobject.h"

#include "bakaglobal.h"
#include <glm/gtx/transform.hpp>
#include <glm/vec3.hpp>

using namespace Baka;

GameObject::GameObject(Scene* scene) : scene_(scene), dying_(false)
{
}

GameObject::~GameObject()
{
}

const char* GameObject::GetName()
{
	return "GameObject";
}

const char* GameObject::GetID()
{
	return "object";
}

VisibleComponent* GameObject::GetVisibleComponent()
{
	return nullptr;
}

PhysicalComponent* GameObject::GetPhysicalComponent()
{
	return nullptr;
}

LightComponent* GameObject::GetLightComponent()
{
	return nullptr;
}

bool GameObject::IsActor() const
{
	return false;
}

void GameObject::Update(double dt)
{
}

bool GameObject::IsDying(bool die)
{
	if (die)
		dying_ = true;

	return dying_;
}

GameActor::GameActor(Scene* scene) : GameObject(scene), position_(0.f, 0.f, 0.f), rotation_(0.f, 0.f, 0.f), scale_(1.f), model_matrix_(1.f)
{
}

const char* GameActor::GetName()
{
	return "GameActor";
}

const char* GameActor::GetID()
{
	return "actor";
}

glm::vec3& GameActor::GetPosition()
{
	return position_;
}

glm::vec3& GameActor::GetRotation()
{
	return rotation_;
}

float& GameActor::GetScale()
{
	return scale_;
}

bool GameActor::IsActor() const
{
	return true;
}

const Mat4& GameActor::GetModelMatrix() const
{
	return model_matrix_;
}

Mat4 GameActor::CalculateModelMatrix() const
{
	if (scale_ == 1.f && rotation_.x == 0.f && rotation_.y == 0.f && rotation_.z == 0.f)
		return glm::translate(position_);

	glm::mat4 translation = glm::translate(position_);
	glm::mat4 scale = glm::scale(glm::vec3(scale_, scale_, scale_));
	glm::mat4 rotation(1.f);

	if (rotation_.x != 0.f)
		rotation = glm::rotate(rotation, glm::radians(rotation_.x), glm::vec3(1.f, 0.f, 0.f));

	return translation * rotation * scale;
}

void GameActor::UpdateModelMatrix()
{
	model_matrix_ = CalculateModelMatrix();
}