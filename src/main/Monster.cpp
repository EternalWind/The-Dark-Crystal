#include "Monster.h"
#include "Agent.h"
#include "Alien.h"
#include "BattleState.h"
#include "EntityManager.h"
#include "ClosestNotMeNotDynamicObjectConvexResultCallback.h"
#include "ConfigurationManager.h"
#include "AttackDetectComponent.h"

#include <Scene/Scene.hpp>
#include <Audio/SoundComponent.hpp>
#include <Logic/RaycastComponent.hpp>
#include <Utils/Random.hpp>

#include <BulletDynamics/Dynamics/btRigidBody.h>

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
		std::cout << "Die!!!!!!!!!!!!!Name = " << dt::Utils::toStdString(this->getName()) << std::endl;
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
	mIsAttacking(false) {
}

void Monster::onInitialize() {
	Character::onInitialize();

	auto conf_mgr = ConfigurationManager::getInstance() ;
	SoundSetting sound_setting = conf_mgr->getSoundSetting();

	// 邪恶的硬编码=_=
	mAttackSoundHandle = "monster_attack.wav";

	auto attack_sound = this->addComponent<dt::SoundComponent>(new SoundComponent(mAttackSoundHandle, ATTACK_SOUND_COMPONENT));

	attack_sound->setVolume((float)sound_setting.getSoundEffect());
	attack_sound->getSound().setLoop(true);

	auto interator = this->addComponent<dt::InteractionComponent>(new AttackDetectComponent(INTERACTOR_COMPONENT));
	interator->setRange(mAttackRange);
	interator->setIntervalTime(mAttackInterval);

	connect(interator.get(), SIGNAL(sHit(dt::PhysicsBodyComponent*)), 
		this, SLOT(__onHit(dt::PhysicsBodyComponent*)));

    connect(this, SIGNAL(sIsDead(Character*)), EntityManager::get(), SLOT(__isMonsterDead(Character*)));

	// 行走的时候忽略摩擦力
	this->findComponent<dt::PhysicsBodyComponent>(PHYSICS_BODY_COMPONENT)->getRigidBody()->setFriction(0.0);

	this->setCurSpeed(15.0f);

	// 攻击效果
	mFlashNode = this->addChildNode(new dt::Node(this->getName() + "_flash_node"));
	mFlashNode->addComponent(new dt::MeshComponent("lightning.mesh", "", "FlashMesh"));
	mFlashNode->setScale(5.0);
	mFlashNode->setPosition(mFlashNode->getPosition() + Ogre::Vector3(0, 0, -7));	
	mFlashNode->disable();

	// 初始化随机值
	dt::Random::initialize();
}

void Monster::onDeinitialize() {
    Character::onDeinitialize();
}

void Monster::onUpdate(double time_diff) {
	this->mIsUpdatingAfterChange = (time_diff == 0);

	if (mHasKilled) {
        if (this->findComponent<dt::MeshComponent>(MESH_COMPONENT)->isAnimationStopped()) {
            this->kill(); 
            return; 
        }

		//既然已经死了，那么往下的更新都没用了，全部cut掉！！！
		mFlashNode->disable();
		this->findComponent<dt::SoundComponent>(ATTACK_SOUND_COMPONENT)->stopSound();
		this->findComponent<dt::PhysicsBodyComponent>(PHYSICS_BODY_COMPONENT)->getRigidBody()->setLinearVelocity(BtOgre::Convert::toBullet(Ogre::Vector3(0, 0, 0)));

		dt::Node::onUpdate(time_diff);

		return;
    }

	if (mIsAttacking) {
		auto interator = this->findComponent<AttackDetectComponent>(INTERACTOR_COMPONENT);

		if (interator->isReady()) {
			interator->check();
			
			// 播放攻击动画
			auto mesh = this->findComponent<dt::MeshComponent>(MESH_COMPONENT);
            //把Stop去掉只是为了让它播放的更顺畅而已>_<
			mesh->setAnimation("attack");
			mesh->setLoopAnimation(true);
			mesh->playAnimation();

			float random_degree = dt::Random::get((float)0.0, (float)360.0);
			mFlashNode->setRotation(mFlashNode->getRotation() * Ogre::Quaternion(Ogre::Degree(random_degree), Ogre::Vector3(0, 0, 1)));
		}
	}

	Character::onUpdate(time_diff);
}
// --------------- slots -------------------//

void Monster::__onAttack(bool is_pressed) {	
	if (is_pressed) {
		mIsAttacking = true;
		mFlashNode->enable();
		this->findComponent<dt::SoundComponent>(ATTACK_SOUND_COMPONENT)->playSound();
	} else {
		mIsAttacking = false;
		mFlashNode->disable();
		this->findComponent<dt::SoundComponent>(ATTACK_SOUND_COMPONENT)->stopSound();
	}
}

void Monster::__onLookAround(Ogre::Quaternion body_rot, Ogre::Quaternion agent_rot) {
	Character::__onLookAround(body_rot, agent_rot);
}

void Monster::__onHit(dt::PhysicsBodyComponent* hit) {

	// 邪恶的万恶的空指针啊！！！
	if (hit == nullptr) return;

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

bool Monster::__canMoveTo(const btTransform& position, btTransform& closest_position) {
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
        dt::PhysicsBodyComponent* other_physics_body = static_cast<dt::PhysicsBodyComponent*>(rigid_body->getUserPointer());

        if (other_physics_body != nullptr) {
            Alien* alien = dynamic_cast<Alien*>(other_physics_body->getNode());

            if (alien != nullptr) {
                return false;
            }
        }
    }

    return true;
}