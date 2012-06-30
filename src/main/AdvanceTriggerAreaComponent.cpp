#include "AdvanceTriggerAreaComponent.h"

#include <Scene/Node.hpp>
#include <Scene/Scene.hpp>
#include <Scene/Component.hpp>

AdvanceTriggerAreaComponent::AdvanceTriggerAreaComponent(btCollisionShape* areaShape,
                                           const QString name) : 
    Component(name),
    mArea(areaShape),
    mObject(nullptr) { }

void AdvanceTriggerAreaComponent::onUpdate(double time_diff) {
    if(!isEnabled()) {
        return;
    }

    //// TODO: Update only on node position change
    btTransform transform;
    transform.setIdentity();
    transform.setOrigin(BtOgre::Convert::toBullet(getNode()->getPosition(dt::Node::SCENE)));
    transform.setRotation(BtOgre::Convert::toBullet(getNode()->getRotation(dt::Node::SCENE)));
    mObject->setWorldTransform(transform);

    for(int32_t i = 0; i < mObject->getNumOverlappingObjects(); ++i)
    {
        void* user_pointer = mObject->getOverlappingObject(i)->getUserPointer();

        if (user_pointer != nullptr) {
            Component* collidingComponent = static_cast<Component*>(user_pointer);

            if (collidingComponent == nullptr)
                continue;

            if (collidingComponent->getNode() == nullptr)
                continue;

            emit triggered(this, collidingComponent);
        }
    }

    this->onDisable();
    getNode()->kill();
}

void AdvanceTriggerAreaComponent::onInitialize() {
    btTransform transform;
    transform.setIdentity();
    transform.setOrigin(BtOgre::Convert::toBullet(getNode()->getPosition(dt::Node::SCENE)));
    transform.setRotation(BtOgre::Convert::toBullet(getNode()->getRotation(dt::Node::SCENE)));
    
    mObject = new btGhostObject();
    mObject->setCollisionShape(mArea);
    mObject->setWorldTransform(transform);
    mObject->setCollisionFlags(mObject->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
    mObject->setUserPointer(nullptr);
    getNode()->getScene()->getPhysicsWorld()->getBulletWorld()->addCollisionObject(mObject);
}

void AdvanceTriggerAreaComponent::setAreaShape(btCollisionShape* areaShape)
{
    mArea = areaShape;
    mObject->setCollisionShape(mArea);
}

void AdvanceTriggerAreaComponent::onDeinitialize() {
    //elete mObject;
    //delete mArea;
}
void AdvanceTriggerAreaComponent::onEnable(){
    getNode()->getScene()->getPhysicsWorld()->getBulletWorld()->addCollisionObject(mObject);
}
void AdvanceTriggerAreaComponent::onDisable(){
    getNode()->getScene()->getPhysicsWorld()->getBulletWorld()->getCollisionWorld()->removeCollisionObject(mObject);
    getNode()->getScene()->getPhysicsWorld()->getBulletWorld()->removeCollisionObject(mObject);
    this->setAreaShape(new btBoxShape(btVector3(0, 0, 0)));
    getNode()->getScene()->getPhysicsWorld()->getBulletWorld()->addCollisionObject(mObject);
}

