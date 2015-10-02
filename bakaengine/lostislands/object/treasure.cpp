#include "lostislands/object/treasure.h"

#include "game/scene/scene.h"

using namespace Baka;

LiObjects::Treasure::Treasure(Scene* scene, Vec3 pos) : GameActor(scene)
{
	visible_ = new TreasureVisibleComponent(this, scene, "models/tank.dae", "shaders/standard.glsl");
	phys_ = new TreasurePhysicalComponent(this, scene, visible_);
	position_ = pos;

	rotation_.x = -90;

	scale_ = 0.5f;

	UpdateModelMatrix();
}

LiObjects::Treasure::~Treasure()
{
	delete visible_;
	delete phys_;
}

VisibleComponent* LiObjects::Treasure::GetVisibleComponent()
{
	return visible_;
}

PhysicalComponent* LiObjects::Treasure::GetPhysicalComponent()
{
	return phys_;
}

void LiObjects::Treasure::Update(double dt)
{
}

const char* LiObjects::Treasure::GetName()
{
	return "Treasure";
}

LiObjects::Treasure::TreasureVisibleComponent::TreasureVisibleComponent(GameObject* obj, Scene* scene, const String& mesh, const String& shader)
	: SimpleVisibleComponent(obj, scene, mesh, shader)
{
	custom_mesh_ = new Mesh(*mesh_.get());
	mesh_.reset();
	MeshMaterial &mat = custom_mesh_->GetMaterial(0);
	MeshMaterial &mat2 = custom_mesh_->GetMaterial(2);
	MeshMaterial &mat3 = custom_mesh_->GetMaterial(1);
	mat.texture_diffuse = scene->GetCache().GetTexture("textures/tank.jpg");
	mat2.texture_diffuse = scene->GetCache().GetTexture("textures/tank3.dds");
	mat3.texture_diffuse = scene->GetCache().GetTexture("textures/tank2.dds");
}

LiObjects::Treasure::TreasureVisibleComponent::~TreasureVisibleComponent()
{
}

Mesh* LiObjects::Treasure::TreasureVisibleComponent::GetMesh() const
{
	return custom_mesh_;
}

LiObjects::Treasure::TreasurePhysicalComponent::TreasurePhysicalComponent(GameObject* obj, Scene* scene, VisibleComponent* visible)
	: StaticMeshPhysicalComponent(obj, scene, visible), using_(false)
{
	scene->GetDispatcher().Register("action.UseDown", obj, BIND_METHOD_CALL(LiObjects::Treasure::TreasurePhysicalComponent::OnUse, this));
	scene->GetDispatcher().Register("action.UseUp", obj, BIND_METHOD_CALL(LiObjects::Treasure::TreasurePhysicalComponent::OnUse, this));
}

void LiObjects::Treasure::TreasurePhysicalComponent::OnUse(void* param)
{
	bool down = reinterpret_cast<bool>(param);

	using_ = down;
}

void LiObjects::Treasure::TreasurePhysicalComponent::OnTouch(PhysicalComponent* other)
{
	if (using_)
		scene_->DeleteObject(obj_);
}

PhysicalComponentType LiObjects::Treasure::TreasurePhysicalComponent::GetType() const
{
	return PhysicalComponentType::StaticIgnored;
}