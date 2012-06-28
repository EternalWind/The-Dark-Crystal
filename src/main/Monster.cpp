#include "Monster.h"
#include "Agent.h"
#include "Alien.h"

#include "ConfigurationManager.h"

#include <Audio/SoundComponent.hpp>
#include <Logic/RaycastComponent.hpp>

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

void Monster::onKilled() {
	auto mesh = this->findComponent<dt::MeshComponent>(MESH_COMPONENT);
	mesh->setAnimation("die");
	mesh->playAnimation();
}

Monster::Monster(const QString node_name,
	const QString mesh_handle,
	const dt::PhysicsBodyComponent::CollisionShapeType collision_shape_type, 
	const btScalar mass,
	const QString walk_sound_handle, 
	const QString jump_sound_handle, 
	const QString run_sound_handle,
	const QString attack_sound_handle,
	const uint16_t attack_value,
	const float attack_range, 
	const float attack_interval)
	: Character(node_name, mesh_handle, collision_shape_type, mass, walk_sound_handle, jump_sound_handle, run_sound_handle, 20.0f),
	mAttackSoundHandle(attack_sound_handle),
	mAttackValue(attack_value),
	mAttackRange(attack_range),
	mAttackInterval(attack_interval) {
}

void Monster::onInitialize() {
	Character::onInitialize();

	auto conf_mgr = ConfigurationManager::getInstance() ;
	SoundSetting sound_setting = conf_mgr->getSoundSetting();

	auto attack_sound = this->addComponent<dt::SoundComponent>(new SoundComponent(mAttackSoundHandle, ATTACK_SOUND_COMPONENT));

	attack_sound->setVolume((float)sound_setting.getSoundEffect());
	attack_sound->getSound().setLoop(true);

	auto interator = this->addComponent<dt::InteractionComponent>(new dt::RaycastComponent(INTERACTOR_COMPONENT));
	interator->setRange(this->getAttackRange());
	interator->setIntervalTime(mAttackInterval);

	connect(interator.get(), SIGNAL(sHit(dt::PhysicsBodyComponent*)), 
		this, SLOT(__onHit(dt::PhysicsBodyComponent*)));

	this->findComponent<dt::PhysicsBodyComponent>(PHYSICS_BODY_COMPONENT)->getRigidBody()->setFriction(0.0);

	this->setCurSpeed(15.0f);
}

void Monster::onDeinitialize() {
}

void Monster::onUpdate(double time_diff) {
    this->mIsUpdatingAfterChange = (time_diff == 0);

    auto physics_body = this->findComponent<dt::PhysicsBodyComponent>(PHYSICS_BODY_COMPONENT);
    auto velocity = BtOgre::Convert::toBullet(getRotation(dt::Node::SCENE) * mMoveVector * mCurSpeed);
    velocity.setY(physics_body->getRigidBody()->getLinearVelocity().y());

    if (velocity != physics_body->getRigidBody()->getLinearVelocity()) {
        physics_body->activate();
        physics_body->getRigidBody()->setLinearVelocity(velocity);
    }

	//if (this->getCurHealth() == 0) {
	//	if (this->findComponent<dt::MeshComponent>(MESH_COMPONENT)->isAnimationStopped()) {
	//		this->kill();
	//	}
	//}

	Character::onUpdate(time_diff);
}
// --------------- slots -------------------//

void Monster::__onAttack(bool is_pressed) {
	if (is_pressed) {
		auto interator = this->findComponent<dt::RaycastComponent>(INTERACTOR_COMPONENT);
		auto attack_sound = this->findComponent<dt::SoundComponent>(ATTACK_SOUND_COMPONENT);
		if (interator->isReady()) {
			attack_sound->playSound();
			interator->check();
			
			// 播放攻击动画
			auto mesh = this->findComponent<dt::MeshComponent>(MESH_COMPONENT);
			mesh->setAnimation("attack");
			mesh->setLoopAnimation(false);
			mesh->playAnimation();
		}
	}
}

void Monster::__onLookAround(Ogre::Quaternion body_rot, Ogre::Quaternion agent_rot) {
	Character::__onLookAround(body_rot, agent_rot);
}

void Monster::__onHit(dt::PhysicsBodyComponent* hit) {
	// 只能攻击Alien
	Alien* obj = dynamic_cast<Alien*>(hit->getNode());

	if (obj != nullptr) {
		uint16_t cur_health = obj->getCurHealth();
		obj->setCurHealth(getAttackValue() > cur_health ? 0 : cur_health - getAttackValue());

		// 如果他挂了>_<
		if (obj->getCurHealth() == 0) {
			obj->onKilled();
		}
	}
}

void Monster::__onChangeWeapon(Weapon::WeaponType type) {
}

void Monster::__onRemoveWeapon() {
}

void Monster::__onRemoveWeapon(Weapon::WeaponType type) {
}

void Monster::__onAddEquipment(bool is_pressed) {
}

void Monster::__onEquiped(dt::PhysicsBodyComponent* object) {
}

void Monster::__onGetOffVehicle() {
}

void Monster::__onReload() {
}