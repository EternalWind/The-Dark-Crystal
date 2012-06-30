#include "Entity.h"

#include <Scene/Scene.hpp>

const QString Entity::MESH_COMPONENT = "mesh";
const QString Entity::PHYSICS_BODY_COMPONENT = "physics_body";
const float Entity::LENGTH_BEYOND_ONGROUND_DETECTING_RADIUS = 0.5f;

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
      //mDirection(Ogre::Quaternion::IDENTITY),
      mIsAddingEquipment(false),
      mIsMoving(false),
	  mIsJumping(false),
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

        emit sHealthChanged(mCurHealth);
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

float Entity::getCurSpeed() const {
    return mCurSpeed;
}

void Entity::setCurSpeed(const float cur_speed) {
    if (mCurSpeed != cur_speed) {
        uint16_t pre_speed = mCurSpeed;
        mCurSpeed = cur_speed;

        emit sSpeedChanged(pre_speed, mCurSpeed);
    }
}

float Entity::getOrigSpeed() const {
    return mOrigSpeed;
}

void Entity::setOrigSpeed(const float original_speed) {
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
    auto mesh = this->findComponent<dt::MeshComponent>(MESH_COMPONENT);
    auto physics_body = this->findComponent<dt::PhysicsBodyComponent>(PHYSICS_BODY_COMPONENT);
    //float radius;
    //btVector3 center;

    //physics_body->getRigidBody()->getCollisionShape()->getBoundingSphere(center, radius);

    //Ogre::Vector3 half_size = mesh->getOgreEntity()->getBoundingBox().getHalfSize();

	btVector3 size = dynamic_cast<btBoxShape*>(physics_body->getRigidBody()->getCollisionShape())->getHalfExtentsWithoutMargin();

    Ogre::Vector3 start(0.0f, 0.0f, 0.0f);
    Ogre::Vector3 end(0.0f, 0.0f, 0.0f);

    start = getRotation(Node::SCENE) * Ogre::Vector3(0.0, size.y(), size.z())
                + getPosition(Node::SCENE);
    end = getRotation(Node::SCENE) * Ogre::Vector3(0.0, -size.y() - LENGTH_BEYOND_ONGROUND_DETECTING_RADIUS, size.z())
                + getPosition(Node::SCENE);

    btVector3 bt_start, bt_end;
    bt_start = BtOgre::Convert::toBullet(start);
    bt_end = BtOgre::Convert::toBullet(end);

    btCollisionWorld::ClosestRayResultCallback raycast_callback(bt_start, bt_end);

    getScene()->getPhysicsWorld()->getBulletWorld()->rayTest(bt_start, bt_end, raycast_callback);

    bool result = raycast_callback.hasHit();

    return result;
}

btScalar Entity::getMass() const {
	return mMass;
}

void Entity::resetPhysicsBody() {
	auto physics_body = this->findComponent<dt::PhysicsBodyComponent>(PHYSICS_BODY_COMPONENT);

	if (physics_body != nullptr) {
		physics_body->disable();
		physics_body->enable();
	}
}

void Entity::onKilled() {
}

void Entity::onInitialize() {
    this->addComponent<dt::MeshComponent>(new dt::MeshComponent(mMeshHandle, "", MESH_COMPONENT));
    auto physics = this->addComponent<dt::PhysicsBodyComponent>(new dt::PhysicsBodyComponent(MESH_COMPONENT, PHYSICS_BODY_COMPONENT, mCollisionShapeType, mMass));
}

void Entity::onDeinitialize() {
}