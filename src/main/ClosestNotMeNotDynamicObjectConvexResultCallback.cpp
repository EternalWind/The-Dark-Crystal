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
        // �Բۣ��ⲻ�����Լ�ô��
        return btScalar(1.0);
    }

    if (!convexResult.m_hitCollisionObject->isStaticOrKinematicObject()) {
        // ������̬���壿�����ùܣ�T������
        return btScalar(1.0);
    }

    return ClosestConvexResultCallback::addSingleResult(convexResult, normalInWorldSpace);
}