#include "Monster.h"
#include "Agent.h"
#include "Alien.h"
#include "BattleState.h"
#include "EntityManager.h"

#include "ConfigurationManager.h"
#include "AttackDetectComponent.h"

#include <Scene/Scene.hpp>
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
        this->findComponent<AttackDetectComponent>(INTERACTOR_COMPONENT)->setRange(this->getAttackRange());
	}
}

float Monster::getAttackInterval() {
    return mAttackInterval;
}

void Monster::setAttackInterval(float attack_interval) {
    if (attack_interval > 0.0f) {
        mAttackInterval = attack_interval;
        this->findComponent<AttackDetectComponent>(INTERACTOR_COMPONENT)->setIntervalTime(mAttackInterval);
    }
}

void Monster::onKilled() {
    if (!mHasKilled) {
        mHasKilled = true;
        auto mesh = this->findComponent<dt::MeshComponent>(MESH_COMPONENT);
        Agent* agent = dynamic_cast<Agent*>(this->findChildNode(Agent::AGENT).get());
         
        emit sIsDead(this);
        disconnect(this, SIGNAL(sIsDead(Character*)), EntityManager::get(), SLOT(__isMonsterDead(Character*)));
        

        if (agent != nullptr) {
            agent->disable();
        }

        mesh->setAnimation("die");
        mesh->setLoopAnimation(false);
        mesh->playAnimation();

        BattleState* battle_state = dynamic_cast<BattleState*>(this->getScene()->getState());
        battle_state->setRemainEnemyNum(battle_state->getRemainEnemyNum() - 1);
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
	const uint16_t attack_value,
	const float attack_range, 
	const float attack_interval)
	: Character(node_name, mesh_handle, collision_shape_type, mass, walk_sound_handle, jump_sound_handle, run_sound_handle, 20.0f),
	mAttackSoundHandle(attack_sound_handle),
	mAttackValue(attack_value),
	mAttackRange(attack_range),
	mAttackInterval(attack_interval),
	mIsAttacking(false) {}

void Monster::onInitialize() {
	Character::onInitialize();

	auto conf_mgr = ConfigurationManager::getInstance() ;
	SoundSetting sound_setting = conf_mgr->getSoundSetting();

	auto attack_sound = this->addComponent<dt::SoundComponent>(new SoundComponent(mAttackSoundHandle, ATTACK_SOUND_COMPONENT));

	attack_sound->setVolume((float)sound_setting.getSoundEffect());
	attack_sound->getSound().setLoop(true);

	auto interator = this->addComponent<dt::InteractionComponent>(new AttackDetectComponent(INTERACTOR_COMPONENT));
	interator->setRange(this->getAttackRange());
	interator->setIntervalTime(mAttackInterval);

	connect(interator.get(), SIGNAL(sHit(dt::PhysicsBodyComponent*)), 
		this, SLOT(__onHit(dt::PhysicsBodyComponent*)));

    connect(this, SIGNAL(sIsDead(Character*)), EntityManager::get(), SLOT(__isMonsterDead(Character*)));

	this->findComponent<dt::PhysicsBodyComponent>(PHYSICS_BODY_COMPONENT)->getRigidBody()->setFriction(0.0);

	this->setCurSpeed(15.0f);
}

void Monster::onDeinitialize() {
    Character::onDeinitialize();
}

void Monster::onUpdate(double time_diff) {
    if (mHasKilled) {
		//std::cout << "has_kill" << std::endl;
        auto mesh = this->findComponent<dt::MeshComponent>(MESH_COMPONENT);
        if (mesh->isAnimationStopped()) {
            this->disable(); 
            this->kill(); 
            return; 
        }
    }
    this->mIsUpdatingAfterChange = (time_diff == 0);

	if (mIsAttacking) {
		auto interator = this->findComponent<AttackDetectComponent>(INTERACTOR_COMPONENT);
		
		if (interator->isReady()) {
			auto attack_sound = this->findComponent<dt::SoundComponent>(ATTACK_SOUND_COMPONENT);
			attack_sound->stopSound();
			attack_sound->playSound();
			interator->check();
			
			// ���Ź�������
			auto mesh = this->findComponent<dt::MeshComponent>(MESH_COMPONENT);
            mesh->stopAnimation();
			mesh->setAnimation("attack");
			mesh->setLoopAnimation(true);
			mesh->playAnimation();
		}
	}
	Character::onUpdate(time_diff);

}
// --------------- slots -------------------//

void Monster::__onAttack(bool is_pressed) {
	mIsAttacking = is_pressed;
}

void Monster::__onLookAround(Ogre::Quaternion body_rot, Ogre::Quaternion agent_rot) {
	Character::__onLookAround(body_rot, agent_rot);
}

void Monster::__onHit(dt::PhysicsBodyComponent* hit) {

	// а������Ŀ�ָ�밡������
	if (hit == nullptr) return;

	// ֻ�ܹ���Alien
	Alien* obj = dynamic_cast<Alien*>(hit->getNode());

	if (obj != nullptr) {
		uint16_t cur_health = obj->getCurHealth();
		obj->setCurHealth(getAttackValue() > cur_health ? 0 : cur_health - getAttackValue());

		// ���������>_<
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