#include "Entity.h"

const QString Entity::MESH_COMPONENT = "mesh";

const QString Entity::PHYSICS_BODY_COMPONENT = "physics_body";

Entity::Entity(const QString mesh_handle, const dt::PhysicsBodyComponent::CollisionShapeType collision_shape_type, const btScalar mass) 
    : mMeshHandle(mesh_handle),
      mCollisionShapeType(collision_shape_type),
      mMass(mass) {}

uint16_t Entity::getCurHealth() const {
    return mCurHealth;
}

void Entity::setCurHealth(const uint16_t current_health) {
    if(mCurHealth != current_health)
        mCurHealth = current_health;
}

uint16_t Entity::getMaxHealth() const {
    return mMaxHealth;
}

void Entity::setMaxHealth(const uint16_t max_health) {
    if(mMaxHealth != max_health)
        mMaxHealth = max_health;
}

QString Entity::getID() const {
    return mID;
}

void Entity::setID(const QString id) {
    if(mID != id)
        mID = id;
}

uint16_t Entity::getCurSpeed() const {
    return mCurSpeed;
}

void Entity::setCurSpeed(const uint16_t cur_speed) {
    if(mCurSpeed != cur_speed)
        mCurSpeed = cur_speed;
}

uint16_t Entity::getMaxSpeed() const {
    return mMaxSpeed;
}

void Entity::setMaxSpeed(const uint16_t max_speed) {
    if(mMaxSpeed != max_speed)
        mMaxSpeed = max_speed;
}

Ogre::Vector3 Entity::getEyePosition() const {
    return mEyePosition;
}

void Entity::setEyePosition(const Ogre::Vector3 eye_position) {
    if(mEyePosition != eye_position)
        mEyePosition = eye_position;
}

void Entity::onInitialize() {
    this->addComponent<dt::MeshComponent>(new dt::MeshComponent(mMeshHandle, "", MESH_COMPONENT));
    this->addComponent<dt::PhysicsBodyComponent>(new dt::PhysicsBodyComponent(MESH_COMPONENT, PHYSICS_BODY_COMPONENT, mCollisionShapeType, mMass));
}

void Entity::onDeinitialize() {
}