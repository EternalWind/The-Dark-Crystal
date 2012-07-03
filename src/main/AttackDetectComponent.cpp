#include "AttackDetectComponent.h"
#include "Entity.h"
#include "ClosestNotMeNotDynamicObjectConvexResultCallback.h"

#include <Scene/Scene.hpp>
#include <Physics/PhysicsBodyComponent.hpp>

#include "BulletCollision/CollisionDispatch/btGhostObject.h"

#include "Agent.h"


// 没想到YD这个类这么给力啊！！！ >_<
//class ClosestNotMeNotDynamicObjectConvexResultCallback : public btCollisionWorld::ClosestConvexResultCallback {
//public:
//    ClosestNotMeNotDynamicObjectConvexResultCallback(btCollisionObject* me) 
//        : ClosestConvexResultCallback(btVector3(0.0, 0.0, 0.0), btVector3(0.0, 0.0, 0.0)),
//          mMe(me) {}
//
//    btScalar addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace) {
//        if (convexResult.m_hitCollisionObject == mMe) {
//            // 卧槽！这不是我自己么！
//            return btScalar(1.0);
//        }
//
//        if (!convexResult.m_hitCollisionObject->isStaticOrKinematicObject()) {
//            // 神马？动态物体？！不用管，T飞它！
//            return btScalar(1.0);
//        }
//
//		// 如果是Ghost的话，比如说那个神马TriggerAreaComponent的话就直接把他无视掉啦！！！！
//		// 话说为什么过滤会返回1.0啊！！！尼玛！！！
//		btGhostObject* ghost = dynamic_cast<btGhostObject*>(convexResult.m_hitCollisionObject);
//		if (ghost != nullptr) {
//			return btScalar(1.0);
//		}
//
//        return ClosestConvexResultCallback::addSingleResult(convexResult, normalInWorldSpace);
//    }
//
//private:
//    btCollisionObject* mMe;
//};

AttackDetectComponent::AttackDetectComponent(const QString& name)
	: dt::InteractionComponent(name) {}

void AttackDetectComponent::onCheck(const Ogre::Vector3& start, const Ogre::Vector3& end) {
	btVector3 bt_start = BtOgre::Convert::toBullet(start);
	btVector3 bt_end = BtOgre::Convert::toBullet(end);

	// 擦！！！It's useless！！！
	//btCollisionWorld::ClosestConvexResultCallback callback(bt_start, bt_end);
    Agent * agent = dynamic_cast<Agent *>(getNode());
    btRigidBody* rigid_body;
    if (agent != nullptr) {
        rigid_body = agent->getParent()->findComponent<dt::PhysicsBodyComponent>(Entity::PHYSICS_BODY_COMPONENT)->getRigidBody();
    } else if (getNode()->getName() == "getProp")
        rigid_body = getNode()->getParent()->findComponent<dt::PhysicsBodyComponent>(Entity::PHYSICS_BODY_COMPONENT)->getRigidBody();
    else 
	rigid_body = getNode()->findComponent<dt::PhysicsBodyComponent>(Entity::PHYSICS_BODY_COMPONENT)->getRigidBody();

	ClosestNotMeNotDynamicObjectConvexResultCallback callback(rigid_body);

	btTransform start_trans = rigid_body->getWorldTransform();
	btTransform end_trans = start_trans;

	start_trans.setOrigin(bt_start);
	end_trans.setOrigin(bt_end);

	getNode()->getScene()->getPhysicsWorld()->getBulletWorld()->convexSweepTest(dynamic_cast<btConvexShape*>(rigid_body->getCollisionShape()), start_trans, end_trans, callback);

	btCollisionObject* collision_object = callback.m_hitCollisionObject;

	if (callback.hasHit() && collision_object != nullptr) {
		dt::PhysicsBodyComponent* hit_object = static_cast<dt::PhysicsBodyComponent*>(collision_object->getUserPointer());

		if (hit_object != nullptr) {
			emit sHit(hit_object);
		}
	}

}