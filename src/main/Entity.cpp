#include "Entity.h"

const QString Entity::MESH_COMPONENT = "mesh";

const QString Entity::PHYSICS_BODY_COMPONENT = "physics_body";

Entity::Entity(const QString name, const QString mesh_handle, const dt::PhysicsBodyComponent::CollisionShapeType collision_shape_type, const btScalar mass) 
    : dt::Node(name),
      mCurHealth(0),
      mMaxHealth(0),
      mCurSpeed(0),
      mOrigSpeed(0),
      mEyePosition(0.0f, 0.0f, 0.0f),
      mID(""),
      mMeshHandle(mesh_handle),
      mCollisionShapeType(collision_shape_type),
      mMass(mass),
      mMoveVector(0.0f, 0.0f, 0.0f),
      mIsAddingEquipment(false),
      mIsMoving(false),
      mHasSpeededUp(false) {}

uint16_t Entity::getCurHealth() const {
    return mCurHealth;
}

void Entity::setCurHealth(const uint16_t current_health) {
    if (mCurHealth != current_health) {
        uint16_t pre_health = mCurHealth;

        if (current_health <= mMaxHealth) {
            mCurHealth = current_health;
        } else {
            mCurHealth = mMaxHealth;
        }

        emit sHealthChanged(pre_health, mCurHealth);
    }
}

uint16_t Entity::getMaxHealth() const {
    return mMaxHealth;
}

void Entity::setMaxHealth(const uint16_t max_health) {
    if (mMaxHealth != max_health)
        mMaxHealth = max_health;
}

QString Entity::getID() const {
    return mID;
}

void Entity::setID(const QString id) {
    if (mID != id)
        mID = id;
}

uint16_t Entity::getCurSpeed() const {
    return mCurSpeed;
}

void Entity::setCurSpeed(const uint16_t cur_speed) {
    if (mCurSpeed != cur_speed) {
        uint16_t pre_speed = mCurSpeed;
        mCurSpeed = cur_speed;

        emit sSpeedChanged(pre_speed, mCurSpeed);
    }
}

uint16_t Entity::getOrigSpeed() const {
    return mOrigSpeed;
}

void Entity::setOrigSpeed(const uint16_t original_speed) {
    if (mOrigSpeed != original_speed)
        mOrigSpeed = original_speed;
}

Ogre::Vector3 Entity::getEyePosition() const {
    return mEyePosition;
}

void Entity::setEyePosition(const Ogre::Vector3 eye_position) {
    if (mEyePosition != eye_position)
        mEyePosition = eye_position;
}

bool Entity::isOnGround() {
    return this->findComponent<dt::PhysicsBodyComponent>(PHYSICS_BODY_COMPONENT)->getRigidBody()->
        getLinearVelocity().getY() == 0.0f;
}

void Entity::onInitialize() {
    this->addComponent<dt::MeshComponent>(new dt::MeshComponent(mMeshHandle, "", MESH_COMPONENT));
    this->addComponent<dt::PhysicsBodyComponent>(new dt::PhysicsBodyComponent(MESH_COMPONENT, PHYSICS_BODY_COMPONENT, mCollisionShapeType, mMass));
}

void Entity::onDeinitialize() {
}