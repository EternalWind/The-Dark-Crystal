#include "AttackDetectComponent.h"
#include "Entity.h"

#include <Scene/Scene.hpp>
#include <Physics/PhysicsBodyComponent.hpp>

// û�뵽YD�������ô������������ >_<
class ClosestNotMeNotDynamicObjectConvexResultCallback : public btCollisionWorld::ClosestConvexResultCallback {
public:
    ClosestNotMeNotDynamicObjectConvexResultCallback(btCollisionObject* me) 
        : ClosestConvexResultCallback(btVector3(0.0, 0.0, 0.0), btVector3(0.0, 0.0, 0.0)),
          mMe(me) {}

    btScalar addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace) {
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

private:
    btCollisionObject* mMe;
};

AttackDetectComponent::AttackDetectComponent(const QString& name)
	: dt::InteractionComponent(name) {}

void AttackDetectComponent::onCheck(const Ogre::Vector3& start, const Ogre::Vector3& end) {
	btVector3 bt_start = BtOgre::Convert::toBullet(start);
	btVector3 bt_end = BtOgre::Convert::toBullet(end);

	// ��������It's useless������
	//btCollisionWorld::ClosestConvexResultCallback callback(bt_start, bt_end);

	auto rigid_body = getNode()->findComponent<dt::PhysicsBodyComponent>(Entity::PHYSICS_BODY_COMPONENT)->getRigidBody();

	ClosestNotMeNotDynamicObjectConvexResultCallback callback(rigid_body);

	btTransform start_trans = rigid_body->getWorldTransform();
	btTransform end_trans = start_trans;

	start_trans.setOrigin(bt_start);
	end_trans.setOrigin(bt_end);

	getNode()->getScene()->getPhysicsWorld()->getBulletWorld()->convexSweepTest(dynamic_cast<btConvexShape*>(rigid_body->getCollisionShape()), start_trans, end_trans, callback);

	if (callback.hasHit()) {
		btCollisionObject* collision_object = callback.m_hitCollisionObject;
		dt::PhysicsBodyComponent* hit_object = static_cast<dt::PhysicsBodyComponent*>(collision_object->getUserPointer());

		emit sHit(hit_object);
	}

}