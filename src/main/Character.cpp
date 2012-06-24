#include "Character.h"
#include "Agent.h"
#include "ConfigurationManager.h"

#include <Scene/Scene.hpp>

const QString Character::WALK_SOUND_COMPONENT = "walk_sound";
const QString Character::JUMP_SOUND_COMPONENT = "jump_sound";
const QString Character::RUN_SOUND_COMPONENT = "run_sound";

// 我也来玩玩长得恶心的命名方式。 XD
class ClosestNotMeNotDynamicObjectConvexResultCallback : public btCollisionWorld::ClosestConvexResultCallback {
public:
    ClosestNotMeNotDynamicObjectConvexResultCallback(btCollisionObject* me) 
        : ClosestConvexResultCallback(btVector3(0.0, 0.0, 0.0), btVector3(0.0, 0.0, 0.0)),
          mMe(me) {}

    btScalar addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace) {
        if (convexResult.m_hitCollisionObject == mMe) {
            // 卧槽！这不是我自己么！
            return btScalar(1.0);
        }

        if (!convexResult.m_hitCollisionObject->isStaticOrKinematicObject()) {
            // 神马？动态物体？！不用管，T飞它！
            return btScalar(1.0);
        }

        return ClosestConvexResultCallback::addSingleResult(convexResult, normalInWorldSpace);
    }

private:
    btCollisionObject* mMe;
};

Character::Character(const QString node_name, const QString mesh_handle, const dt::PhysicsBodyComponent::CollisionShapeType collision_shape_type, 
    const btScalar mass, const QString walk_sound_handle, const QString jump_sound_handle, const QString run_sound_handle, const float jump_speed)
    : Entity(node_name, mesh_handle, collision_shape_type, mass),
      mWalkSoundHandle(walk_sound_handle),
      mJumpSoundHandle(jump_sound_handle),
      mRunSoundHandle(run_sound_handle),
      mVelocity(0.0, 0.0, 0.0),
      mJumpSpeed(jump_speed) {}

void Character::onInitialize() {
    Entity::onInitialize();

    auto conf_mgr = ConfigurationManager::getInstance() ;
    SoundSetting sound_setting = conf_mgr->getSoundSetting();

    auto walk_sound = this->addComponent<dt::SoundComponent>(new dt::SoundComponent(mWalkSoundHandle, WALK_SOUND_COMPONENT));
    auto jump_sound = this->addComponent<dt::SoundComponent>(new dt::SoundComponent(mJumpSoundHandle, JUMP_SOUND_COMPONENT));
    auto run_sound = this->addComponent<dt::SoundComponent>(new dt::SoundComponent(mRunSoundHandle, RUN_SOUND_COMPONENT));

    walk_sound->setVolume((float)sound_setting.getSoundEffect());
    jump_sound->setVolume((float)sound_setting.getSoundEffect());
    run_sound->setVolume((float)sound_setting.getSoundEffect());

    walk_sound->getSound().setLoop(true);
    run_sound->getSound().setLoop(true);
    jump_sound->getSound().setLoop(false);

    auto physics_body = this->findComponent<dt::PhysicsBodyComponent>(PHYSICS_BODY_COMPONENT);

    // 化身Kinematic Body拯救世界！！！
    physics_body->getRigidBody()->setCollisionFlags(physics_body->getRigidBody()->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
    physics_body->getRigidBody()->setActivationState(DISABLE_DEACTIVATION);
}

void Character::onDeinitialize() {
    Entity::onDeinitialize();
}

void Character::onUpdate(double time_diff) {
    this->mIsUpdatingAfterChange = (time_diff == 0);

    auto physics_body = this->findComponent<dt::PhysicsBodyComponent>(PHYSICS_BODY_COMPONENT);
    btMotionState* motion = physics_body->getRigidBody()->getMotionState();
    auto gravity = physics_body->getRigidBody()->getGravity();  // 这Gravity是重力加速度…… >_<
    
    Ogre::Vector3 normalized_move_vector = mMoveVector;
    normalized_move_vector.normalise();

    auto new_velocity = BtOgre::Convert::toBullet(getRotation(dt::Node::SCENE) * normalized_move_vector * mCurSpeed);

    btTransform target_position;
    motion->getWorldTransform(target_position);
    
    btTransform possible_position;
    possible_position.setOrigin(BtOgre::Convert::toBullet(getPosition(dt::Node::SCENE)));
    possible_position.setRotation(target_position.getRotation());

    mVelocity.setX(new_velocity.x());
    mVelocity.setZ(new_velocity.z());

    if (this->isOnGround()) {
        if (mVelocity.getY() < 0.0f) {
            // 人物已在地面上，因此将掉落速度清零。
            mVelocity.setY(0.0f);
        }
    } else {
        mVelocity += gravity * time_diff;
    }

    target_position.setOrigin(target_position.getOrigin() + mVelocity * time_diff);

    if (__canMoveTo(target_position, possible_position)) {
        // 竟然能够移动到这里！！！
        motion->setWorldTransform(target_position);
    } else {
        // 移动不到……
        mVelocity.setY(1.0f);
        //std::cout << mVelocity.getX() << " " << mVelocity.getY() << " " << mVelocity.getZ() << std::endl;
    }

    this->mIsUpdatingAfterChange = false;

    Node::onUpdate(time_diff);
}

void Character::setJumpSpeed(const float jump_speed) {
    mJumpSpeed = jump_speed;
}

float Character::getJumpSpeed() const {
    return mJumpSpeed;
}

void Character::__onMove(Entity::MoveType type, bool is_pressed) {
    bool is_stopped = false;

    switch (type) {
    case FORWARD:
        if (is_pressed && mMoveVector.z > -1.0f)
            mMoveVector.z -= 1.0f; // Ogre Z轴正方向为垂直屏幕向外。
        else if (!is_pressed && mMoveVector.z < 1.0f)
            mMoveVector.z += 1.0f;

        break;

    case BACKWARD:
        if (is_pressed && mMoveVector.z < 1.0f)
            mMoveVector.z += 1.0f;
        else if (!is_pressed && mMoveVector.z > -1.0f)
            mMoveVector.z -= 1.0f;

        break;

    case LEFTWARD:
        if (is_pressed && mMoveVector.x > -1.0f)
            mMoveVector.x -= 1.0f;
        else if (!is_pressed && mMoveVector.x < 1.0f)
            mMoveVector.x += 1.0f;

        break;

    case RIGHTWARD:
        if (is_pressed && mMoveVector.x < 1.0f)
            mMoveVector.x += 1.0f;
        else if (!is_pressed && mMoveVector.x > -1.0f)
            mMoveVector.x -= 1.0f;

        break;

    case STOP:
        mMoveVector.x = 0.0f;
        mMoveVector.z = 0.0f;
        is_stopped = true;

        break;

    default:
        dt::Logger::get().debug("Not processed MoveType!");
    }

    if (is_stopped) {
        this->findComponent<dt::SoundComponent>(WALK_SOUND_COMPONENT)->stopSound();
        this->findComponent<dt::SoundComponent>(RUN_SOUND_COMPONENT)->stopSound();
    } else {
        std::shared_ptr<dt::SoundComponent> move_sound;

        if (mHasSpeededUp) {
            move_sound = this->findComponent<dt::SoundComponent>(RUN_SOUND_COMPONENT);
        } else {
            move_sound = this->findComponent<dt::SoundComponent>(WALK_SOUND_COMPONENT);
        }

        move_sound->playSound();
    }

    mIsMoving = !is_stopped;
}

void Character::__onJump(bool is_pressed) {
    auto physics_body = this->findComponent<dt::PhysicsBodyComponent>(PHYSICS_BODY_COMPONENT);

    if (is_pressed && this->isOnGround()) {
        mVelocity.setY(mJumpSpeed);

        this->findComponent<dt::SoundComponent>(JUMP_SOUND_COMPONENT)->playSound();
    }
}

void Character::__onSpeedUp(bool is_pressed) {
    float increasing_rate = 1.5f;

    if (is_pressed) {
        this->setCurSpeed(this->getCurSpeed() * increasing_rate);

        if (mIsMoving) {
            this->findComponent<dt::SoundComponent>(WALK_SOUND_COMPONENT)->stopSound();
            this->findComponent<dt::SoundComponent>(RUN_SOUND_COMPONENT)->playSound();
        }
    } else {
        this->setCurSpeed(this->getCurSpeed() / increasing_rate);

        if (mIsMoving) {
            this->findComponent<dt::SoundComponent>(RUN_SOUND_COMPONENT)->stopSound();
            this->findComponent<dt::SoundComponent>(WALK_SOUND_COMPONENT)->playSound();
        }
    }

    mHasSpeededUp = is_pressed;
}

void Character::__onLookAround(Ogre::Quaternion body_rot, Ogre::Quaternion agent_rot) {
    Ogre::Quaternion rotation(body_rot.getYaw(), Ogre::Vector3(0.0f, 1.0f, 0.0f));

    auto physics_body = this->findComponent<dt::PhysicsBodyComponent>(PHYSICS_BODY_COMPONENT);
    auto motion = physics_body->getRigidBody()->getMotionState();
    btTransform trans;

    this->findChildNode(Agent::AGENT)->setRotation(agent_rot);

    motion->getWorldTransform(trans);
    trans.setRotation(BtOgre::Convert::toBullet(rotation));
    motion->setWorldTransform(trans);
}

bool Character::__canMoveTo(const btTransform& position, btTransform& closest_position) {
    auto physics_body = this->findComponent<dt::PhysicsBodyComponent>(PHYSICS_BODY_COMPONENT);
    ClosestNotMeNotDynamicObjectConvexResultCallback callback(physics_body->getRigidBody());
    
    btTransform target = position;
    btVector3 origin = target.getOrigin();
    origin.setY(origin.y() + 0.01f);
    target.setOrigin(origin);

    this->getScene()->getPhysicsWorld()->getBulletWorld()->convexSweepTest(dynamic_cast<btConvexShape*>(physics_body->getRigidBody()->getCollisionShape()), 
        physics_body->getRigidBody()->getWorldTransform(), target, callback);

    btRigidBody* rigid_body = dynamic_cast<btRigidBody*>(callback.m_hitCollisionObject);

    if (callback.hasHit() && rigid_body != nullptr) {
        /*btTransform character_trans;
        btTransform collided_obj_trans;

        physics_body->getRigidBody()->getMotionState()->getWorldTransform(character_trans);
        rigid_body->getMotionState()->getWorldTransform(collided_obj_trans);

        btVector3 direction = character_trans.getOrigin() - collided_obj_trans.getOrigin();
        float value = direction.length();
        direction.normalize();

        btVector3 center;
        btScalar radius;

        physics_body->getRigidBody()->getCollisionShape()->getBoundingSphere(center, radius);
        value -= radius;

        rigid_body->getCollisionShape()->getBoundingSphere(center, radius);
        value -= radius;

        closest_position.setOrigin(character_trans.getOrigin() + direction * value);
*/
        return false;
    }
    
    return true;
}