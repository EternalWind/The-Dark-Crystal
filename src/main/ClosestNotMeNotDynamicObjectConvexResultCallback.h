#ifndef THE_DARK_CRYSTAL_CLOSEST_NOT_ME_NOT_DYNAMIC_OBJECT_CONVEX_RESULT_CALLBACK
#define THE_DARK_CRYSTAL_CLOSEST_NOT_ME_NOT_DYNAMIC_OBJECT_CONVEX_RESULT_CALLBACK

#include <BulletCollision/CollisionDispatch/btGhostObject.h>

// ��Ҳ�����泤�ö��ĵ�������ʽ�� XD
/**
  * Bullet��������Լ��Ƕ�̬������ص��ࡣ
  */
class ClosestNotMeNotDynamicObjectConvexResultCallback : public btCollisionWorld::ClosestConvexResultCallback {
public:
    ClosestNotMeNotDynamicObjectConvexResultCallback(btCollisionObject* me) ;

    btScalar addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace);

private:
    btCollisionObject* mMe;
};

#endif