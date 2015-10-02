#ifndef PHYSICS_COLLISIONENGINE_H_
#define PHYSICS_COLLISIONENGINE_H_

#include "bakaglobal.h"

#include "game/scene/scene.h"
#include "game/core/registry.h"

namespace Baka
{
	struct StaticCollisionInfo {
		PhysicalComponent *dynamic;
		PhysicalComponent *stat;
	};

	typedef std::pair<PhysicalComponent*, PhysicalComponent*> ColliderPair;
	typedef std::set<ColliderPair> StaticCollisions;
	typedef std::pair<Vec3, Vec3> AABBPair;
	typedef std::unordered_map<PhysicalComponent*, AABBPair> TransformedAABB;

	class CollisionEngine {
	public:
		void Tick(Scene &scene);

	protected:
		void FindCollisions(const PhysicalComponentsList &phys, TransformedAABB &aabb, StaticCollisions &statcol);
		void TransformBoundingBoxes(const PhysicalComponentsList &phys, TransformedAABB &aabb);

		bool BoxesCollide(const AABBPair &a, const AABBPair &b);
	};
}

#endif