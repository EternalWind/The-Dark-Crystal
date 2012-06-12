#include "Car.h"
#include "Alien.h"
#include "Agent.h"

#include <Logic/RaycastComponent.hpp>
#include "ConfigurationManager.h"

const QString Car::MOVE_SOUND_COMPONENT = "move_sound";
const QString Car::RUSH_SOUND_COMPONENT = "rush_sound";
const QString Car::ATTACK_SOUND_COMPONENT = "attack_sound"; 
const QString Car::INTERATOR_COMPONENT = "interator";

Car::Car(const QString node_name, 
	const QString mesh_handle, 
	const dt::PhysicsBodyComponent::CollisionShapeType collision_shape_type, 
	const btScalar mass,
	const uint16_t attack_value,
	const float attack_range,
	const float attack_interval,
	const QString move_sound_handle,
	const QString rush_sound_handle,
	const QString attack_sound_handle) 
	: Vehicle(node_name, mesh_handle, collision_shape_type, mass, attack_value, attack_range, attack_interval),
	mMoveSoundHandle(move_sound_handle),
	mRushSoundHandle(rush_sound_handle),
	mAttackSoundHandle(attack_sound_handle) {
}

void Car::onInitialize() {
	Vehicle::onDeinitialize();

	auto conf_mgr = ConfigurationManager::getInstance();
	SoundSetting sound_setting = conf_mgr->getSoundSetting();

	auto move_sound = this->addComponent<dt::SoundComponent>(new dt::SoundComponent(mMoveSoundHandle, MOVE_SOUND_COMPONENT));
	auto rush_sound = this->addComponent<dt::SoundComponent>(new dt::SoundComponent(mRushSoundHandle, RUSH_SOUND_COMPONENT));
	auto attack_sound = this->addComponent<dt::SoundComponent>(new dt::SoundComponent(mAttackSoundHandle, ATTACK_SOUND_COMPONENT));

	move_sound->setVolume((float)sound_setting.getSoundEffect());
	rush_sound->setVolume((float)sound_setting.getSoundEffect());
	attack_sound->setVolume((float)sound_setting.getSoundEffect());

	auto interator = this->addComponent<dt::InteractionComponent>(new dt::RaycastComponent(INTERATOR_COMPONENT));
	interator->setIntervalTime(mAttackInterval);

	connect(interator.get(), SIGNAL(sHit(dt::PhysicsBodyComponent*)), this, SLOT(__onEquiped(dt::PhysicsBodyComponent*)));

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

void Car::__onAttack(bool is_pressed) {
	if (is_pressed) {
		auto interator = this->findComponent<dt::RaycastComponent>(INTERATOR_COMPONENT);
		auto attack_sound = this->findComponent<dt::SoundComponent>(ATTACK_SOUND_COMPONENT);

		if (interator->isReady()) {
			attack_sound->playSound();
			interator->check();
		}
	}
}

void __onLookAround(Ogre::Quaternion quaternion) {
	/* 这是车啊，不是坦克啊！！！头不会转啊有木有啊！！！ */
}

void Car::__onGetOffVehicle() {
	// 载具驾驶者
	Alien* alien = dynamic_cast<Alien*>(this->getParent());
	alien->findComponent<dt::PhysicsBodyComponent>(PHYSICS_BODY_COMPONENT)->enable();
	alien->findComponent<dt::MeshComponent>(MESH_COMPONENT)->enable();

	this->setParent((Node*)this->getScene());
	Agent* agent = dynamic_cast<Agent*>(this->findChildNode(Agent::AGENT).get());
	agent->detach();
	agent->attachTo(alien);
}

void Car::__onHit(dt::PhysicsBodyComponent* hit) {
	Entity* obj = dynamic_cast<Entity*>(hit->getNode());
	
	if (obj != nullptr) {
		uint16_t cur_health = obj->getCurHealth();
		obj->setCurHealth(getAttackValue() > cur_health ? 0 : cur_health - getAttackValue());
	}
}