#include "Car.h"
#include "Agent.h"

#include <Logic/RaycastComponent.hpp>
#include "ConfigurationManager.h"

const QString Car::MOVE_SOUND_COMPONENT = "move_sound";
const QString Car::RUSH_SOUND_COMPONENT = "rush_sound";

Car::Car(const QString node_name, 
	const QString mesh_handle, 
	const dt::PhysicsBodyComponent::CollisionShapeType collision_shape_type, 
	const btScalar mass,
	const uint16_t attack_value,
	const float attack_range,
	const float attack_interval,
	const QString attack_sound_handle,
	const QString move_sound_handle,
	const QString rush_sound_handle) 
	: Vehicle(node_name, mesh_handle, collision_shape_type, mass, 
	attack_value, attack_range, attack_interval, attack_sound_handle),
	mMoveSoundHandle(move_sound_handle),
	mRushSoundHandle(rush_sound_handle) {
}

void Car::onInitialize() {
	Vehicle::initialize();

	auto conf_mgr = ConfigurationManager::getInstance();
	SoundSetting& sound_setting = conf_mgr->getSoundSetting();

	auto move_sound = this->addComponent<dt::SoundComponent>(new dt::SoundComponent(mMoveSoundHandle, MOVE_SOUND_COMPONENT));
	auto rush_sound = this->addComponent<dt::SoundComponent>(new dt::SoundComponent(mRushSoundHandle, RUSH_SOUND_COMPONENT));

	move_sound->setVolume((float)sound_setting.getSoundEffect());
	rush_sound->setVolume((float)sound_setting.getSoundEffect());

	// 战车摩擦力！擦！擦
	this->findComponent<dt::PhysicsBodyComponent>(PHYSICS_BODY_COMPONENT)->getRigidBody()->setFriction(0.2f);
}

void Car::onDeinitialize() {
}

void Car::__onMove(MoveType type, bool is_pressed) {
	switch (type) {
    case FORWARD:
        if (is_pressed) {
            mMoveVector.z += 1.0f;
		} else {
            mMoveVector.z -= 1.0f;
		}
        break;

    case BACKWARD:
        if (is_pressed) {
            mMoveVector.z -= 1.0f;
		} else {
            mMoveVector.z += 1.0f;
		}
        break;

	case LEFTWARD:
		
		break;

	case RIGHTWARD:

		break;

	default:
		dt::Logger::get().debug("Not processed MoveType!");
	}
}

void Car::__onSpeedUp(bool is_pressed) {
	float increase_rate = 1.5f;

	if (is_pressed) {
		this->setCurSpeed(this->getCurSpeed() * increase_rate);

		if (mIsMoving) {
			this->findComponent<dt::SoundComponent>(MOVE_SOUND_COMPONENT)->stopSound();
			this->findComponent<dt::SoundComponent>(RUSH_SOUND_COMPONENT)->playSound();
		}
	} else {
		this->setCurSpeed(this->getCurSpeed() * increase_rate);

		if (mIsMoving) {
			this->findComponent<dt::SoundComponent>(RUSH_SOUND_COMPONENT)->stopSound();
			this->findComponent<dt::SoundComponent>(MOVE_SOUND_COMPONENT)->playSound();
		}
	}

	mHasSpeededUp = is_pressed;
}

void __onLookAround(Ogre::Quaternion quaternion) {
	/* 这是车啊，不是坦克啊！！！头不会转啊有木有啊！！！ */
}
