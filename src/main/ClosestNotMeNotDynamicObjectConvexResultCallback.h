#ifndef THE_DARK_CRYSTAL_CLOSEST_NOT_ME_NOT_DYNAMIC_OBJECT_CONVEX_RESULT_CALLBACK
#define THE_DARK_CRYSTAL_CLOSEST_NOT_ME_NOT_DYNAMIC_OBJECT_CONVEX_RESULT_CALLBACK

#include <BulletCollision/CollisionDispatch/btGhostObject.h>

// 我也来玩玩长得恶心的命名方式。 XD
/**
  * Bullet用最近非自己非动态物体检测回调类。
  */
class ClosestNotMeNotDynamicObjectConvexResultCallback : public btCollisionWorld::ClosestConvexResultCallback {
public:
    ClosestNotMeNotDynamicObjectConvexResultCallback(btCollisionObject* me) ;

    btScalar addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace);

private:
    btCollisionObject* mMe;
};

#endif