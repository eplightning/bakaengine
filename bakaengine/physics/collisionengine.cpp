#include "physics/collisionengine.h"

#include <unordered_map>
#include <set>

using namespace Baka;

void CollisionEngine::Tick(Scene& scene)
{
	// aabb
	TransformedAABB aabb;
	TransformBoundingBoxes(scene.GetPhysicalComponents(), aabb);

	// find collisions
	StaticCollisions statcol;

	FindCollisions(scene.GetPhysicalComponents(), aabb, statcol);

	// nasz genialny collision engine w chwili obecnej potrafi tylko obsługiwać kolizje na osi y ! (i to dynamiczny na górze)
	EventManager *man = Registry::Instance()->GetEventManager();

	std::unordered_map<PhysicalComponent*, Vec3> mtv_total;

	for (auto &x : statcol) {
		if (x.second->GetType() == PhysicalComponentType::StaticIgnored) {
			x.second->OnTouch(x.first);
			continue;
		}

		AABBPair a = aabb[x.first];
		AABBPair b = aabb[x.second];

		Vec3 mtv(0.f, 0.f, 0.f);

		if (a.first.y < b.second.y)
			mtv.y = b.second.y - a.first.y;

		auto it = mtv_total.find(x.first);

		if (it == mtv_total.end()) {
			mtv_total[x.first] = mtv;
		} else {
			if (it->second.y >= mtv.y) {
				mtv.y = 0.f;
			}
		}

		x.first->OnStaticCollide(x.second, mtv);
		x.second->OnTouch(x.first);

		StaticCollisionInfo info;

		info.dynamic = x.first;
		info.stat = x.second;

		man->Broadcast("physics.StaticCollision", &info);
	}
}

void CollisionEngine::FindCollisions(const PhysicalComponentsList &phys, TransformedAABB &aabb, StaticCollisions& statcol)
{
	for (auto x : phys) {
		if (x->GetType() == PhysicalComponentType::Dynamic) {
			for (auto y : phys) {
				if (x == y)
					continue;

				if (y->GetType() == PhysicalComponentType::Static || y->GetType() == PhysicalComponentType::StaticIgnored) {
					AABBPair a = aabb[x];
					AABBPair b = aabb[y];

					if (!BoxesCollide(a, b))
						continue;

					statcol.emplace(x, y);
				}
			}
		}
	}
}

void CollisionEngine::TransformBoundingBoxes(const PhysicalComponentsList& phys, TransformedAABB& aabb)
{
	for (auto x : phys) {
		// Model Coords OBB -> World Coords OBB
		const Mat4 *model_matrix = x->GetModelMatrix();
		const Vec3 *bounding_box = x->GetBoundingBox();

		Vec4 world_obb[8];

		for (int i = 0; i < 8; i++) {
			world_obb[i] = *model_matrix * Vec4(bounding_box[i], 1.f);
		}

		// World Coords OBB -> World Coords AABB
		AABBPair pair;

		pair.first.x = pair.second.x = world_obb[0].x;
		pair.first.y = pair.second.y = world_obb[0].y;
		pair.first.z = pair.second.z = world_obb[0].z;

		for (int i = 1; i < 8; i++) {
			if (world_obb[i].x < pair.first.x) pair.first.x = world_obb[i].x;
			if (world_obb[i].y < pair.first.y) pair.first.y = world_obb[i].y;
			if (world_obb[i].z < pair.first.z) pair.first.z = world_obb[i].z;
			if (world_obb[i].x > pair.second.x) pair.second.x = world_obb[i].x;
			if (world_obb[i].y > pair.second.y) pair.second.y = world_obb[i].y;
			if (world_obb[i].z > pair.second.z) pair.second.z = world_obb[i].z;
		}

		aabb[x] = pair;
	}
}

bool CollisionEngine::BoxesCollide(const AABBPair& a, const AABBPair& b)
{
	if (a.second.x < b.first.x || a.first.x > b.second.x)
		return false;

	if (a.second.y < b.first.y || a.first.y > b.second.y)
		return false;

	if (a.second.z < b.first.z || a.first.z > b.second.z)
		return false;

	return true;
}