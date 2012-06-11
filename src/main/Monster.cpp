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
	float attack_range,
	float attack_interval)
	: Entity(node_name, mesh_handle, collision_shape_type, mass),
	mWalkSoundHandle(walk_sound_handle),
	mJumpSoundHandle(jump_sound_handle),
	mRunSoundHandle(run_sound_handle),
	mAttackSoundHandle(attack_sound_handle),
	mAttackValue(attack_value),
	mAttackRange(attack_range),
	mAttackInterval(attack_interval),
	mTimeAfterAttack(0.0f),
	mDuringAttack(false) {
}

void Monster::onUpdate(double time_diff) {
	Node::onUpdate(time_diff);
	
	if (mDuringAttack) {
		mTimeAfterAttack += time_diff;
	}

	if (mTimeAfterAttack > mAttackInterval) {
		mTimeAfterAttack = 0.0f;
		mDuringAttack = false;
	}
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

	this->findComponent<dt::PhysicsBodyComponent>(PHYSICS_BODY_COMPONENT)->getRigidBody()->setFriction(0.0);
}

void Monster::onDeinitialize() {
}

// --------------- slots -------------------//

void Monster::__onMove(MoveType type, bool is_pressed) {
    bool is_stopped = false;

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
        if (is_pressed) {
            mMoveVector.x -= 1.0f;
		} else {
            mMoveVector.x += 1.0f;
		}
        break;

    case RIGHTWARD:
        if (is_pressed) {
            mMoveVector.x += 1.0f;
		} else {
            mMoveVector.x -= 1.0f;
		}
        break;

    case STOP:
        is_stopped = true;
        break;
	    
	default:
        dt::Logger::get().debug("Not processed MoveType!");
    }
	    
	if (is_stopped) {
        this->findComponent<dt::SoundComponent>(WALK_SOUND_COMPONENT)->stopSound();
        this->findComponent<dt::SoundComponent>(RUN_SOUND_COMPONENT)->stopSound();
    } else {
        if (!mMoveVector.isZeroLength())
            mMoveVector.normalise();

        this->findComponent<dt::PhysicsBodyComponent>(PHYSICS_BODY_COMPONENT)->getRigidBody()
            ->setLinearVelocity(BtOgre::Convert::toBullet(this->getRotation() * mMoveVector * mCurSpeed));
        
        std::shared_ptr<dt::SoundComponent> move_sound;

        if (mHasSpeededUp) {
            move_sound = this->findComponent<dt::SoundComponent>(RUN_SOUND_COMPONENT);
        } else {
            move_sound = this->findComponent<dt::SoundComponent>(WALK_SOUND_COMPONENT);
        }

        move_sound->playSound();
    }
}

void Monster::__onJump(bool is_pressed) {
    auto physics_body = this->findComponent<dt::PhysicsBodyComponent>(PHYSICS_BODY_COMPONENT);

    if (is_pressed && this->isOnGround()) {
        // 调整该处的脉冲值使跳跃更自然。
        physics_body->applyCentralImpulse(0.0f, 10.0f, 0.0f);

        this->findComponent<dt::SoundComponent>(JUMP_SOUND_COMPONENT)->playSound();
    }

    if (!is_pressed) {
        this->findComponent<dt::SoundComponent>(JUMP_SOUND_COMPONENT)->stopSound();
    }
}

void Monster::__onAttack(bool is_pressed) {
	if (is_pressed && !mDuringAttack) {
		mDuringAttack = true;
		auto interator = this->findComponent<dt::RaycastComponent>(INTERACTOR_COMPONENT);
		auto attack_sound = this->findComponent<dt::SoundComponent>(ATTACK_SOUND_COMPONENT);
		if (interator->isReady()) {
			//attack_sound->stopSound();
			attack_sound->playSound();
			interator->check();
		}
	}
}

void Monster::__onSpeedUp(bool is_pressed) {
    float increasing_rate = 1.5f;

    if (is_pressed) {
        this->setCurSpeed(this->getCurSpeed() * increasing_rate);

        if (mIsMoving) {
            this->findComponent<dt::SoundComponent>(WALK_SOUND_COMPONENT)->stopSound();
            this->findComponent<dt::SoundComponent>(RUN_SOUND_COMPONENT)->playSound();
        }
    } else {
        this->setCurSpeed(this->getCurSpeed() / increasing_rate);

        if (mIsMoving) {
            this->findComponent<dt::SoundComponent>(RUN_SOUND_COMPONENT)->stopSound();
            this->findComponent<dt::SoundComponent>(WALK_SOUND_COMPONENT)->playSound();
        }
    }

    mHasSpeededUp = is_pressed;
}

void Monster::__onLookAround(Ogre::Quaternion quaternion) {
    Ogre::Quaternion rotation(quaternion.getYaw(), Ogre::Vector3(0.0f, 1.0f, 0.0f));
    auto physics_body = this->findComponent<dt::PhysicsBodyComponent>(PHYSICS_BODY_COMPONENT);
    
    // 暂时禁用PhysicsBody以便手动设置旋转。
    physics_body->disable();
    this->setRotation(rotation, dt::Node::SCENE);
    physics_body->enable();
}

void Monster::__onHit(dt::PhysicsBodyComponent* hit) {
	Entity* obj = dynamic_cast<Entity*>(hit->getNode());
	
	if (obj != nullptr) {
		uint16_t cur_health = obj->getCurHealth();
		obj->setCurHealth(getAttackValue() > cur_health ? 0 : cur_health - getAttackValue());
	}
}
