#include "Vehicle.h"
#include "Alien.h"
#include "Agent.h"

#include "ConfigurationManager.h"

#include <Logic/RaycastComponent.hpp>

const QString Vehicle::ATTACK_SOUND_COMPONENT = "attack_sound"; 
//const QString Vehicle::INTERATOR_COMPONENT = "interactor";


uint16_t Vehicle::getAttackValue() const {
	return mAttackValue;
}

void Vehicle::setAttackValue(uint16_t attack_value) {
	mAttackValue = attack_value;
}

float Vehicle::getAttackRange() const {
	return mAttackRange;
}

void Vehicle::setAttackRange(float attack_range) {
	mAttackRange = attack_range;
}


Vehicle::Vehicle(const QString node_name, 
	const QString mesh_handle, 
	const dt::PhysicsBodyComponent::CollisionShapeType collision_shape_type, 
	const btScalar mass,
	const uint16_t attack_value,
	const float attack_range,
	const float attack_interval,
	const QString attack_sound_handle)
	: Entity(node_name, mesh_handle, collision_shape_type, mass),
	mAttackValue(attack_value),
	mAttackRange(attack_range),
	mAttackInterval(attack_interval),
	mAttackSoundHandle(attack_sound_handle),
	mIsAttacking(false) {
}


void Vehicle::onInitialize() {
	Entity::onInitialize();

	auto conf_mgr = ConfigurationManager::getInstance();
	SoundSetting& sound_setting = conf_mgr->getSoundSetting();

	auto attack_sound = this->addComponent<dt::SoundComponent>(new dt::SoundComponent(mAttackSoundHandle, ATTACK_SOUND_COMPONENT));
	attack_sound->setVolume((float)sound_setting.getSoundEffect());
	attack_sound->getSound().setLoop(true);

	//设置载具长宽
	btBoxShape* box = dynamic_cast<btBoxShape*>(this->findComponent<dt::PhysicsBodyComponent>(PHYSICS_BODY_COMPONENT)
													->getRigidBody()->getCollisionShape());
	if (box != nullptr) {
		btVector3 size = box->getHalfExtentsWithoutMargin();
		mWidth = size.x();
		mLength = size.z();
	}
}

void Vehicle::onDeinitialize() {
	Entity::onDeinitialize();
}

// -------- slots --------- //

void Vehicle::__onAttack(bool is_pressed) {
	mIsAttacking = is_pressed;

	auto sound = this->findComponent<dt::SoundComponent>(ATTACK_SOUND_COMPONENT);
	if (sound != nullptr) {
		is_pressed ? sound->playSound() : sound->stopSound();
	}
}

void Vehicle::__onGetOffVehicle() {
}

void Vehicle::__onHit(dt::PhysicsBodyComponent* hit) {
	Entity* obj = dynamic_cast<Entity*>(hit->getNode());
	
	if (obj != nullptr) {
		uint16_t cur_health = obj->getCurHealth();
		obj->setCurHealth(getAttackValue() > cur_health ? 0 : cur_health - getAttackValue());

		// 如果他挂了>_<
		if (obj->getCurHealth() == 0) {
			obj->onKilled();
		}
	}
}

void Vehicle::__onJump(bool is_pressed) {
}

void Vehicle::__onChangeWeapon(Weapon::WeaponType type) {
}

void Vehicle::__onRemoveWeapon() {
}

void Vehicle::__onRemoveWeapon(Weapon::WeaponType type) {
}

void Vehicle::__onAddEquipment(bool is_pressed) {
}

void Vehicle::__onEquiped(dt::PhysicsBodyComponent* object) {
}

void Vehicle::__onReload() {
}