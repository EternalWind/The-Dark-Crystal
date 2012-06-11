#include "Monster.h"

#include "ConfigurationManager.h"

#include <Audio/SoundComponent.hpp>
#include <Logic/RaycastComponent.hpp>

const QString Monster::WALK_SOUND_COMPONENT = "walk_sound";
const QString Monster::JUMP_SOUND_COMPONENT = "jump_sound";
const QString Monster::RUN_SOUND_COMPONENT = "run_sound";
const QString Monster::ATTACK_SOUND_COMPONENT = "attack_sound";
const QString Monster::INTERACTOR_COMPONENT = "interator";

uint16_t Monster::getAttackValue() {
	return mAttackValue;
}

void Monster::setAttackValue(uint16_t attack_value) {
	mAttackValue = attack_value;
}

float Monster::getAttackRange() {
	return mAttackRange;
}

void Monster::setAttackRange(float attack_range) {
	if (attack_range > 0.0f) {
		mAttackRange = attack_range;
	}
}

Monster::Monster(const QString node_name,
	const QString mesh_handle,
	const dt::PhysicsBodyComponent::CollisionShapeType collision_shape_type, 
	const btScalar mass,
	const QString walk_sound_handle, 
	const QString jump_sound_handle, 
	const QString run_sound_handle,
	const QString attack_sound_handle,
	uint16_t attack_value,
	float attack_range) 
	: Entity(node_name, mesh_handle, collision_shape_type, mass),
	mWalkSoundHandle(walk_sound_handle),
	mJumpSoundHandle(jump_sound_handle),
	mRunSoundHandle(run_sound_handle),
	mAttackSoundHandle(attack_sound_handle),
	mAttackValue(attack_value),
	mAttackRange(attack_range) {
}
	
void Monster::onInitialize() {
    auto conf_mgr = ConfigurationManager::getInstance() ;
    SoundSetting sound_setting = conf_mgr->getSoundSetting();

	auto walk_sound = this->addComponent<dt::SoundComponent>(new SoundComponent(mWalkSoundHandle, WALK_SOUND_COMPONENT));
	auto jump_sound = this->addComponent<dt::SoundComponent>(new SoundComponent(mJumpSoundHandle, JUMP_SOUND_COMPONENT));
	auto run_sound = this->addComponent<dt::SoundComponent>(new SoundComponent(mRunSoundHandle, RUN_SOUND_COMPONENT));
	auto attack_sound = this->addComponent<dt::SoundComponent>(new SoundComponent(mAttackSoundHandle, ATTACK_SOUND_COMPONENT));

	walk_sound->setVolume((float)sound_setting.getSoundEffect());
	jump_sound->setVolume((float)sound_setting.getSoundEffect());
	run_sound->setVolume((float)sound_setting.getSoundEffect());
	attack_sound->setVolume((float)sound_setting.getSoundEffect());

	walk_sound->getSound().setLoop(true);
	jump_sound->getSound().setLoop(true);
	run_sound->getSound().setLoop(true);
	attack_sound->getSound().setLoop(true);

	auto interator = this->addComponent<dt::InteractionComponent>(new dt::RaycastComponent(INTERACTOR_COMPONENT));
	interator->setRange(this->getAttackRange());

	connect(interator.get(), SIGNAL(sHit(dt::PhysicsBodyComponent*)), 
		this, SLOT(__onHit(dt::PhysicsBodyComponent*)));
}

void Monster::onDeinitialize() {

}

// --------------- slots -------------------//

void Monster::__onMove(MoveType type, bool is_pressed) {

}

void Monster::__onJump(bool is_pressed) {

}

void Monster::__onAttack(bool is_pressed) {

}

void Monster::__onSpeedUp(bool is_pressed) {

}

void Monster::__onLookAround(Ogre::Quaternion quaternion) {

}
void Monster::__onHit(dt::PhysicsBodyComponent* component) {
}
