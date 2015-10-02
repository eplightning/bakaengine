#include "lostislands/object/torch.h"

using namespace Baka;

LiObjects::Torch::Torch(Scene* scene, Vec3 pos) : GameActor(scene)
{
	light_ = new DirectionalLightComponent(this, scene, pos);
	position_ = pos;
}

LiObjects::Torch::~Torch()
{
	delete light_;
}

LightComponent* LiObjects::Torch::GetLightComponent()
{
	return light_;
}

const char* LiObjects::Torch::GetName()
{
	return "Torch";
}