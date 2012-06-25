#include "Monster.h"
#include "Agent.h"

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
	Entity::onInitialize();

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

    Node::onUpdate(time_diff);
}

// --------------- slots -------------------//

//void Monster::__onMove(MoveType type, bool is_pressed) {
//    bool is_stopped = false;
//
//    switch (type) {
//    case FORWARD:
//        if (is_pressed && mMoveVector.z > -1.0f)
//            mMoveVector.z -= 1.0f; // Ogre Z轴正方向为垂直屏幕向外。
//        else if (!is_pressed && mMoveVector.z < 1.0f)
//            mMoveVector.z += 1.0f;
//
//        break;
//
//    case BACKWARD:
//        if (is_pressed && mMoveVector.z < 1.0f)
//            mMoveVector.z += 1.0f;
//        else if (!is_pressed && mMoveVector.z > -1.0f)
//            mMoveVector.z -= 1.0f;
//
//        break;
//
//    case LEFTWARD:
//        if (is_pressed && mMoveVector.x > -1.0f)
//            mMoveVector.x -= 1.0f;
//        else if (!is_pressed && mMoveVector.x < 1.0f)
//            mMoveVector.x += 1.0f;
//
//        break;
//
//    case RIGHTWARD:
//        if (is_pressed && mMoveVector.x < 1.0f)
//            mMoveVector.x += 1.0f;
//        else if (!is_pressed && mMoveVector.x > -1.0f)
//            mMoveVector.x -= 1.0f;
//
//        break;
//
//    case STOP:
//        mMoveVector.x = 0.0f;
//        mMoveVector.z = 0.0f;
//        is_stopped = true;
//
//        break;
//
//    default:
//        dt::Logger::get().debug("Not processed MoveType!");
//    }
//
//    if (is_stopped) {
//        this->findComponent<dt::SoundComponent>(WALK_SOUND_COMPONENT)->stopSound();
//        this->findComponent<dt::SoundComponent>(RUN_SOUND_COMPONENT)->stopSound();
//    } else {
//        std::shared_ptr<dt::SoundComponent> move_sound;
//
//        if (mHasSpeededUp) {
//            move_sound = this->findComponent<dt::SoundComponent>(RUN_SOUND_COMPONENT);
//        } else {
//            move_sound = this->findComponent<dt::SoundComponent>(WALK_SOUND_COMPONENT);
//        }
//
//        move_sound->playSound();
//    }
//
//    mIsMoving = !is_stopped;
//}

//void Monster::__onJump(bool is_pressed) {
//	auto physics_body = this->findComponent<dt::PhysicsBodyComponent>(PHYSICS_BODY_COMPONENT);
//
//    if (is_pressed && this->isOnGround()) {
//        // 调整该处的脉冲值使跳跃更自然。
//        physics_body->activate();
//        physics_body->applyCentralImpulse(0.0f, 20.0f, 0.0f);
//
//        this->findComponent<dt::SoundComponent>(JUMP_SOUND_COMPONENT)->playSound();
//    }
//
//    if(!is_pressed) {
//        this->findComponent<dt::SoundComponent>(JUMP_SOUND_COMPONENT)->stopSound();
//    }
//}

void Monster::__onAttack(bool is_pressed) {
	if (is_pressed) {
		auto interator = this->findComponent<dt::RaycastComponent>(INTERACTOR_COMPONENT);
		auto attack_sound = this->findComponent<dt::SoundComponent>(ATTACK_SOUND_COMPONENT);
		if (interator->isReady()) {
			attack_sound->playSound();
			interator->check();
		}
	}
}

//void Monster::__onSpeedUp(bool is_pressed) {
//	float increasing_rate = 1.5f;
//
//	if (is_pressed) {
//		this->setCurSpeed(this->getCurSpeed() * increasing_rate);
//
//		if (mIsMoving) {
//			this->findComponent<dt::SoundComponent>(WALK_SOUND_COMPONENT)->stopSound();
//			this->findComponent<dt::SoundComponent>(RUN_SOUND_COMPONENT)->playSound();
//		}
//	} else {
//		this->setCurSpeed(this->getCurSpeed() / increasing_rate);
//
//		if (mIsMoving) {
//			this->findComponent<dt::SoundComponent>(RUN_SOUND_COMPONENT)->stopSound();
//			this->findComponent<dt::SoundComponent>(WALK_SOUND_COMPONENT)->playSound();
//		}
//	}
//
//	mHasSpeededUp = is_pressed;
//}

void Monster::__onLookAround(Ogre::Quaternion body_rot, Ogre::Quaternion agent_rot) {
	Character::__onLookAround(body_rot, agent_rot);
}

void Monster::__onHit(dt::PhysicsBodyComponent* hit) {
	Entity* obj = dynamic_cast<Entity*>(hit->getNode());

	if (obj != nullptr) {
		uint16_t cur_health = obj->getCurHealth();
		obj->setCurHealth(getAttackValue() > cur_health ? 0 : cur_health - getAttackValue());
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