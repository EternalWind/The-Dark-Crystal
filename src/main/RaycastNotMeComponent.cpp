#include "RaycastNotMeComponent.h"

#include <Scene/Node.hpp>
#include <Scene/Scene.hpp>
#include <Physics/PhysicsBodyComponent.hpp>

class CloestRaycastNotMeCallback : public btCollisionWorld::ClosestRayResultCallback {
public:
    CloestRaycastNotMeCallback(btCollisionObject* me) 
        : ClosestRayResultCallback(btVector3(0.0, 0.0, 0.0), btVector3(0.0, 0.0, 0.0)),
          mMe(me) {}

    btScalar addSingleResult(btCollisionWorld::LocalRayResult& rayResult, bool normalInWorldSpace) {
        if (rayResult.m_collisionObject == mMe) {
            return btScalar(1.0);
        }

        return ClosestRayResultCallback::addSingleResult(rayResult, normalInWorldSpace);
    }

private:
    btCollisionObject* mMe;
};

RaycastNotMeComponent::RaycastNotMeComponent(btCollisionObject* me, const QString name)
    : InteractionComponent(name),
      mMe(me) {}

void RaycastNotMeComponent::onCheck(const Ogre::Vector3& start, const Ogre::Vector3& end) {
    btVector3 bt_start, bt_end;
    bt_start = BtOgre::Convert::toBullet(start);
    bt_end = BtOgre::Convert::toBullet(end);

    CloestRaycastNotMeCallback raycast_callback(mMe);

    getNode()->getScene()->getPhysicsWorld()->getBulletWorld()->rayTest(bt_start, bt_end, raycast_callback);

    if(raycast_callback.hasHit()) {
        btCollisionObject* collision_object = raycast_callback.m_collisionObject;
        dt::PhysicsBodyComponent* hit_object = static_cast<dt::PhysicsBodyComponent*>(collision_object->getUserPointer());
        
        emit sHit(hit_object);
    }
}