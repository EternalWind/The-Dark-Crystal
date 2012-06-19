#include "Spaceship.h"

#include "ConfigurationManager.h"

#include <Logic/RaycastComponent.hpp>

const QString Spaceship::ATTACK_SOUND_COMPONENT = "attack_sound";
const QString Spaceship::FLYING_SOUND_COMPONENT = "flying_sound";
const QString Spaceship::RISE_SOUND_COMPONENT = "rise_sound";
const QString Spaceship::FALL_SOUND_COMPONENT = "fall_sound";

Spaceship::Spaceship(const QString node_name, 
	const QString mesh_handle, 
	const dt::PhysicsBodyComponent::CollisionShapeType collision_shape_type, 
	const btScalar mass,
	const uint16_t attack_value,
	const float attack_range,
	const float attack_interval,
	const QString attack_sound_handle,
	const QString flying_sound_handle,
	const QString rise_sound_handle,
	const QString fall_sound_handle,
    const float max_speed,
    const float min_speed,
    const float acceleration)
	: Vehicle(node_name, mesh_handle, collision_shape_type, mass, 
	attack_value, attack_range, attack_interval, attack_sound_handle),
	mFlyingSoundHandle(flying_sound_handle),
	mRiseSoundHandle(rise_sound_handle),
	mFallSoundHandle(fall_sound_handle),
    mMaxSpeed(max_speed), 
    mMinSpeed(min_speed),
    mAcceleration(acceleration) {
}

void Spaceship::onInitialize() {
	Vehicle::onInitialize();
	 
	auto conf_mgr = ConfigurationManager::getInstance();
	SoundSetting& sound_setting = conf_mgr->getSoundSetting();

    auto attack_sound = this->addComponent<dt::SoundComponent>(new dt::SoundComponent(mAttackSoundHandle, ATTACK_SOUND_COMPONENT));
	auto flying_sound = this->addComponent<dt::SoundComponent>(new dt::SoundComponent(mFlyingSoundHandle, FLYING_SOUND_COMPONENT));
	auto rise_sound = this->addComponent<dt::SoundComponent>(new dt::SoundComponent(mRiseSoundHandle, RISE_SOUND_COMPONENT));
	auto fall_sound = this->addComponent<dt::SoundComponent>(new dt::SoundComponent(mFallSoundHandle, FALL_SOUND_COMPONENT));

	flying_sound->setVolume((float)sound_setting.getSoundEffect());
	rise_sound->setVolume((float)sound_setting.getSoundEffect());
	fall_sound->setVolume((float)sound_setting.getSoundEffect());

    flying_sound->getSound().setLoop(true);
    rise_sound->getSound().setLoop(true);
    fall_sound->getSound().setLoop(false);

    // 太空没有空气，不需要考虑摩擦力！
    auto p = this->findComponent<dt::PhysicsBodyComponent>(PHYSICS_BODY_COMPONENT);
	p->getRigidBody()->setFriction(0.0);
	p->setCentralForce(0.0f, mMass, 0.0f);

	this->setCurSpeed(6.0f);
}

void Spaceship::onDeinitialize() {
}

void Spaceship::onUpdate(double time_diff) {	
	mIsUpdatingAfterChange = (time_diff == 0);

	this->findComponent<dt::PhysicsBodyComponent>(PHYSICS_BODY_COMPONENT)->setCentralForce(0, 5, 0);
	dt::Node::onUpdate(time_diff);

	mIsUpdatingAfterChange = false;
}

// slots

void Spaceship::__onMove(MoveType type, bool is_pressed) {
    bool is_stopped = false;

	auto physics_body = this->findComponent<dt::PhysicsBodyComponent>(PHYSICS_BODY_COMPONENT);

	switch (type) {
	case FORWARD:
		if (is_pressed)
			mMoveVector.z -= 1.0f; // Bullet的Z轴和Ogre方向相反
		else
			mMoveVector.z += 1.0f;

		break;

	case BACKWARD:
		if (is_pressed)
			mMoveVector.z += 1.0f;
		else
			mMoveVector.z -= 1.0f;

		break;

	case LEFTWARD:
		if (is_pressed) {
			mMoveVector.x -= 1.0f;

			physics_body->disable();
			this->setRotation(this->getRotation() * Ogre::Quaternion(Ogre::Radian(1.0f / 6), Ogre::Vector3(0.0f, 0.0f, 1.0f)));
			physics_body->enable();

		} else {
			mMoveVector.x += 1.0f;

			physics_body->disable();
			this->setRotation(this->getRotation() * Ogre::Quaternion(Ogre::Radian(-1.0f / 6), Ogre::Vector3(0.0f, 0.0f, 1.0f)));
			physics_body->enable();
		}

		break;

	case RIGHTWARD:
		if (is_pressed) {
			mMoveVector.x += 1.0f;

			physics_body->disable();
			this->setRotation(this->getRotation() * Ogre::Quaternion(Ogre::Radian(-1.0f / 6), Ogre::Vector3(0.0f, 0.0f, 1.0f)));
			physics_body->enable();

		} else {
			mMoveVector.x -= 1.0f;
			physics_body->disable();
			this->setRotation(this->getRotation() * Ogre::Quaternion(Ogre::Radian(1.0f / 6), Ogre::Vector3(0.0f, 0.0f, 1.0f)));
			physics_body->enable();

		}

		break;

	case STOP:
		is_stopped = true;

		break;

	default:
		dt::Logger::get().debug("Not processed MoveType!");
	}

    if (is_stopped) {

        this->findComponent<dt::PhysicsBodyComponent>(PHYSICS_BODY_COMPONENT)->getRigidBody()
            ->setLinearVelocity(btVector3(0.0f, 0.0f, 0.0f));
    } else {

        this->findComponent<dt::PhysicsBodyComponent>(PHYSICS_BODY_COMPONENT)->getRigidBody()
			->setLinearVelocity(BtOgre::Convert::toBullet(this->getRotation(dt::Node::SCENE) * mMoveVector * mCurSpeed));

    }

    mIsMoving = !is_stopped;
}

/* 飞机下降 -_- 话说这加速肿么变成下降了，这什么神设定啊 =.= */
void Spaceship::__onSpeedUp(bool is_pressed) {
    float increasing_rate = 1.5f;

    if (is_pressed) {
        this->setCurSpeed(this->getCurSpeed() * increasing_rate);

        //if (mIsMoving) {
        //    this->findComponent<dt::SoundComponent>(WALK_SOUND_COMPONENT)->stopSound();
        //    this->findComponent<dt::SoundComponent>(RUN_SOUND_COMPONENT)->playSound();
        //}
    } else {
        this->setCurSpeed(this->getCurSpeed() / increasing_rate);

        //if (mIsMoving) {
        //    this->findComponent<dt::SoundComponent>(RUN_SOUND_COMPONENT)->stopSound();
        //    this->findComponent<dt::SoundComponent>(WALK_SOUND_COMPONENT)->playSound();
        //}
    }

    mHasSpeededUp = is_pressed;
}

void Spaceship::__onLookAround(Ogre::Quaternion quaternion) {
    this->mLookAroundQuaternion = quaternion;
}

void Spaceship::__moveAround() {
    auto physics_body = this->findComponent<dt::PhysicsBodyComponent>(PHYSICS_BODY_COMPONENT);

    physics_body->disable();
    this->setRotation(quaternion, dt::Node::SCENE);

	physics_body->getRigidBody()	->setLinearVelocity(BtOgre::Convert::toBullet(this->getRotation(dt::Node::SCENE) * mMoveVector * mCurSpeed));

    physics_body->enable();
}

void Spaceship::__onJump(bool is_pressed) {
	if (is_pressed) {
		mMoveVector.y += 1.0f;
		
		this->findComponent<dt::SoundComponent>(FLYING_SOUND_COMPONENT)->stopSound();
		this->findComponent<dt::SoundComponent>(RISE_SOUND_COMPONENT)->playSound();
	} else {
		mMoveVector.y -= 1.0f;

		this->findComponent<dt::SoundComponent>(RISE_SOUND_COMPONENT)->stopSound();
		this->findComponent<dt::SoundComponent>(FLYING_SOUND_COMPONENT)->playSound();
	}

	this->findComponent<dt::PhysicsBodyComponent>(PHYSICS_BODY_COMPONENT)->getRigidBody()
		->setLinearVelocity(BtOgre::Convert::toBullet(this->getRotation(dt::Node::SCENE) * mMoveVector * mCurSpeed));

	mIsJumping = is_pressed;
}