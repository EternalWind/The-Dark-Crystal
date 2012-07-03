#include "ClosestNotMeNotDynamicObjectConvexResultCallback.h"

ClosestNotMeNotDynamicObjectConvexResultCallback::ClosestNotMeNotDynamicObjectConvexResultCallback(btCollisionObject* me)
    : ClosestConvexResultCallback(btVector3(0.0, 0.0, 0.0), btVector3(0.0, 0.0, 0.0)),
      mMe(me) {}

btScalar ClosestNotMeNotDynamicObjectConvexResultCallback::addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace) {
    btGhostObject* ghost = dynamic_cast<btGhostObject*>(convexResult.m_hitCollisionObject);

    if (ghost != nullptr) {
        return btScalar(1.0);
    }

    if (convexResult.m_hitCollisionObject == mMe) {
        // 卧槽！这不是我自己么！
        return btScalar(1.0);
    }

    if (!convexResult.m_hitCollisionObject->isStaticOrKinematicObject()) {
        // 神马？动态物体？！不用管，T飞它！
        return btScalar(1.0);
    }

    return ClosestConvexResultCallback::addSingleResult(convexResult, normalInWorldSpace);
}