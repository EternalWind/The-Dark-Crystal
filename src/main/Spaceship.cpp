#include "Spaceship.h"

#include "ConfigurationManager.h"

#include <Logic/RaycastComponent.hpp>

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
    this->findComponent<dt::PhysicsBodyComponent>(PHYSICS_BODY_COMPONENT)->getRigidBody()->setFriction(0.0);
}

void Spaceship::onDeinitialize() {
}

void Spaceship::onUpdate(double time_diff) {
    static const float EPS = 1e-9;
    dt::Node::onUpdate(time_diff);

    // v = v0 + at
	float z_speed = getCurSpeed() + mMoveVector.z * mAcceleration * time_diff;
	if (z_speed > mMaxSpeed + EPS) {
		z_speed = mMaxSpeed;
	}
	if (z_speed + EPS < mMinSpeed) {
		z_speed = mMinSpeed;
	}
	setCurSpeed(z_speed);

//    this->findComponent<dt::PhysicsBodyComponent>(PHYSICS_BODY_COMPONENT)->getRigidBody()->getLinearVelocity().length();
    this->setRotation(this->mLookAroundQuaternion);
    this->findComponent<dt::PhysicsBodyComponent>(PHYSICS_BODY_COMPONENT)->getRigidBody()->
        setLinearVelocity(BtOgre::Convert::toBullet(this->getRotation() * mMoveVector * getCurSpeed()));
}

// slots

void Spaceship::__onMove(MoveType type, bool is_pressed) {
    switch (type) {

    case FORWARD:
		mMoveVector.z += (is_pressed ? 1.0f : -1.0f);
		break;

    case BACKWARD:
		mMoveVector.z += (is_pressed ? -1.0f : 1.0f);
		break;

	case STOP:
		break;

	default:
		dt::Logger::get().debug("Not processed MoveType!");
	}

}

void Spaceship::__onSpeedUp(bool is_pressed) {

}

void Spaceship::__onLookAround(Ogre::Quaternion quaternion) {
    this->mLookAroundQuaternion = quaternion;
}

void Spaceship::__moveAround() {

}