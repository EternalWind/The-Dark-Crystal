#include "Vehicle.h"
#include "Alien.h"
#include "Agent.h"

#include "ConfigurationManager.h"

#include <Logic/RaycastComponent.hpp>

const QString Vehicle::ATTACK_SOUND_COMPONENT = "attack_sound"; 
const QString Vehicle::INTERATOR_COMPONENT = "interator";


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
	mAttackSoundHandle(attack_sound_handle) {
}


void Vehicle::onInitialize() {
	Entity::onInitialize();

	auto conf_mgr = ConfigurationManager::getInstance();
	SoundSetting& sound_setting = conf_mgr->getSoundSetting();

	auto attack_sound = this->addComponent<dt::SoundComponent>(new dt::SoundComponent(mAttackSoundHandle, ATTACK_SOUND_COMPONENT));
	attack_sound->setVolume((float)sound_setting.getSoundEffect());

	auto interator = this->addComponent<dt::InteractionComponent>(new dt::RaycastComponent(INTERATOR_COMPONENT));
	interator->setIntervalTime(mAttackInterval);

	connect(interator.get(), SIGNAL(sHit(dt::PhysicsBodyComponent*)), 
		this, SLOT(__onEquiped(dt::PhysicsBodyComponent*)));
}

void Vehicle::onDeinitialize() {
}

// -------- slots --------- //


void Vehicle::__onAttack(bool is_pressed) {
	if (is_pressed) {
		auto interator = this->findComponent<dt::RaycastComponent>(INTERATOR_COMPONENT);
		auto attack_sound = this->findComponent<dt::SoundComponent>(ATTACK_SOUND_COMPONENT);

		if (interator->isReady()) {
			attack_sound->playSound();
			interator->check();
		}
	}
}

void Vehicle::__onGetOffVehicle() {
	// ÔØ¾ß¼ÝÊ»Õß
	Alien* alien = dynamic_cast<Alien*>(this->getParent());
	alien->findComponent<dt::PhysicsBodyComponent>(PHYSICS_BODY_COMPONENT)->enable();
	alien->findComponent<dt::MeshComponent>(MESH_COMPONENT)->enable();

	this->setParent((Node*)this->getScene());
	Agent* agent = dynamic_cast<Agent*>(this->findChildNode(Agent::AGENT).get());
	agent->detach();
	agent->attachTo(alien);
}

void Vehicle::__onHit(dt::PhysicsBodyComponent* hit) {
	Entity* obj = dynamic_cast<Entity*>(hit->getNode());
	
	if (obj != nullptr) {
		uint16_t cur_health = obj->getCurHealth();
		obj->setCurHealth(getAttackValue() > cur_health ? 0 : cur_health - getAttackValue());
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