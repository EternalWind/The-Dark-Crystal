#include "Alien.h"
#include "Agent.h"
#include "ConfigurationManager.h"
#include "Ammo.h"
#include "FirstAidKit.h"
#include "Crystal.h"
#include "Agent.h"

#include <Logic/RaycastComponent.hpp>
#include <Scene/Scene.hpp>

#include <BtOgreExtras.h>

const QString Alien::WALK_SOUND_COMPONENT = "walk_sound";
const QString Alien::JUMP_SOUND_COMPONENT = "jump_sound";
const QString Alien::RUN_SOUND_COMPONENT = "run_sound";
const QString Alien::INTERACTOR_COMPONENT = "interactor";

Alien::Alien(const QString node_name, const QString mesh_handle, const dt::PhysicsBodyComponent::CollisionShapeType collision_shape_type, const btScalar mass,
    const QString walk_sound_handle, const QString jump_sound_handle, const QString run_sound_handle)
    : Entity(node_name, mesh_handle, collision_shape_type, mass),
      mWalkSoundHandle(walk_sound_handle),
      mJumpSoundHandle(jump_sound_handle),
      mRunSoundHandle(run_sound_handle),
      mCurWeapon(nullptr),
      mVelocity(0.0f, 0.0f, 0.0f),
      mBtController(nullptr),
      mBtGhostObject(nullptr) {
        // 准备好3种武器的位置。
        mWeapons[Weapon::PRIMARY] = nullptr;
        mWeapons[Weapon::SECONDARY] = nullptr;
        mWeapons[Weapon::THROWABLE] = nullptr;
}

Weapon* Alien::getCurWeapon() const {
    return mCurWeapon;
}

void Alien::changeCurWeapon(const Weapon::WeaponType type) {
    Weapon* cur_weapon = getCurWeapon();
    Weapon* new_weapon = mWeapons[type];

    if ((cur_weapon == nullptr || cur_weapon->getWeaponType() != type) && new_weapon != nullptr) {
        if(cur_weapon != nullptr)
            cur_weapon->disable();

        new_weapon->enable();
        mCurWeapon = new_weapon;

        this->setCurSpeed(this->getOrigSpeed() - new_weapon->getWeight() * 0.1f);
        if (mHasSpeededUp) {
            __onSpeedUp(true);
        }

        emit sWeaponChanged(cur_weapon, new_weapon);
    }
}

void Alien::addWeapon(Weapon* weapon) {
	if (weapon != nullptr) {
		if (mWeapons[weapon->getWeaponType()] != nullptr) {
			bool is_enabled = mWeapons[weapon->getWeaponType()]->isEnabled();
			removeWeapon(weapon->getWeaponType());

			mWeapons[weapon->getWeaponType()] = weapon;
			weapon->removeComponent("weapon-body");
			weapon->setParent(this);
			weapon->setRotation(Ogre::Quaternion::IDENTITY);
			weapon->setPosition(0.5f, -0.5f, -1.0f);

			if (!is_enabled)
				weapon->disable();
		}
		else {
			mWeapons[weapon->getWeaponType()] = weapon;
			weapon->removeComponent("weapon-body");
			weapon->setParent(this);
			weapon->setRotation(Ogre::Quaternion::IDENTITY);
			weapon->setPosition(0.5f, -0.5f, -1.0f);
		}
		emit sWeaponAdded(weapon);
	}
}

void Alien::removeWeapon(const Weapon::WeaponType type) {
    Weapon* weapon = mWeapons[type];

    if (weapon != nullptr) {
        mWeapons[type] = nullptr;

        weapon->enable();
        weapon->setPosition(0.0f, 2.0f, -2.0f);
        weapon->setParent((dt::Node*)(this->getScene()));
        weapon->addComponent<dt::PhysicsBodyComponent>(new dt::PhysicsBodyComponent(MESH_COMPONENT, PHYSICS_BODY_COMPONENT, 
            dt::PhysicsBodyComponent::BOX, (float)weapon->getWeight()));

        this->setCurSpeed(this->getOrigSpeed());
        if (mHasSpeededUp) {
            __onSpeedUp(true);
        }

        emit sWeaponRemoved(weapon);
    }
}

void Alien::onInitialize() {
    Entity::onInitialize();

    auto conf_mgr = ConfigurationManager::getInstance() ;
    SoundSetting sound_setting = conf_mgr->getSoundSetting();

    auto walk_sound = this->addComponent<dt::SoundComponent>(new dt::SoundComponent(mWalkSoundHandle, WALK_SOUND_COMPONENT));
    auto jump_sound = this->addComponent<dt::SoundComponent>(new dt::SoundComponent(mJumpSoundHandle, JUMP_SOUND_COMPONENT));
    auto run_sound = this->addComponent<dt::SoundComponent>(new dt::SoundComponent(mRunSoundHandle, RUN_SOUND_COMPONENT));

    walk_sound->setVolume((float)sound_setting.getSoundEffect());
    jump_sound->setVolume((float)sound_setting.getSoundEffect());
    run_sound->setVolume((float)sound_setting.getSoundEffect());

    walk_sound->getSound().setLoop(true);
    run_sound->getSound().setLoop(true);
    jump_sound->getSound().setLoop(false);

    auto node = this->addChildNode(new Node("getWeapon"));

    auto iteractor = node->addComponent<dt::InteractionComponent>(new dt::RaycastComponent(INTERACTOR_COMPONENT));
    iteractor->setRange(10.0f);
    node->setPosition(this->getEyePosition());

    connect(iteractor.get(), SIGNAL(sHit(dt::PhysicsBodyComponent*)), this, SLOT(__onEquiped(dt::PhysicsBodyComponent*)));

    this->setOrigSpeed(10.0f);
    this->setCurSpeed(10.0f);

    this->removeComponent(PHYSICS_BODY_COMPONENT);

    btTransform  start_trans;
    start_trans.setIdentity();
    start_trans.setOrigin(btVector3(0, 30, 0));
    start_trans.setRotation(BtOgre::Convert::toBullet(getRotation(Node::SCENE)));

    Ogre::Vector3 size = findComponent<dt::MeshComponent>(MESH_COMPONENT)->getOgreEntity()->getBoundingBox().getHalfSize();
    btBoxShape* box = new btBoxShape(BtOgre::Convert::toBullet(size));

    mBtGhostObject = std::shared_ptr<btPairCachingGhostObject>(new btPairCachingGhostObject());
    mBtGhostObject->setWorldTransform(start_trans);
    mBtGhostObject->setCollisionShape(box);
    mBtGhostObject->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);
    /////////////////////////////////////////////////////////////////////////////////////////////////
    //TODO: Make a better way to distinguish between AdvancedPlayerComponent and PhysicsBodyComponent.
    //For now, it's just a null pointer check to do this.
    mBtGhostObject->setUserPointer(nullptr);
    /////////////////////////////////////////////////////////////////////////////////////////////////
    mBtController = std::shared_ptr<btKinematicCharacterController>
        (new btKinematicCharacterController(mBtGhostObject.get(), box, 1));
    getScene()->getPhysicsWorld()->getBulletWorld()->addCollisionObject(mBtGhostObject.get());
    getScene()->getPhysicsWorld()->getBulletWorld()->addAction(mBtController.get());

    // 外星人的行走不需要考虑摩擦力。
    //auto physics_body = this->findComponent<dt::PhysicsBodyComponent>(PHYSICS_BODY_COMPONENT);
    //physics_body->getRigidBody()->setFriction(0.0);

    // 化身Kinematic Body拯救世界！！！
    //physics_body->getRigidBody()->setCollisionFlags(physics_body->getRigidBody()->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
    //physics_body->getRigidBody()->setActivationState(DISABLE_DEACTIVATION);
}

void Alien::onDeInitialize() {
    Entity::onDeinitialize();
}

void Alien::onUpdate(double time_diff) {
    this->mIsUpdatingAfterChange = (time_diff == 0);

    if (mIsAddingEquipment) {
        mIsAddingEquipment = false;
        this->findChildNode("getWeapon")->findComponent<dt::InteractionComponent>(INTERACTOR_COMPONENT)->check();
    }

    //auto physics_body = this->findComponent<dt::PhysicsBodyComponent>(PHYSICS_BODY_COMPONENT);
    //auto gravity = physics_body->getRigidBody()->getGravity();  // 这Gravity是重力加速度…… >_<
    auto velocity = BtOgre::Convert::toBullet(getRotation(dt::Node::SCENE) * mMoveVector * mCurSpeed);

    btTransform trans = mBtGhostObject->getWorldTransform();

    setPosition(BtOgre::Convert::toOgre(trans.getOrigin()), dt::Node::SCENE);
    setRotation(BtOgre::Convert::toOgre(trans.getRotation()), dt::Node::SCENE);
    //mVelocity.setX(velocity.x()
    //mVelocity += gravity;

    //velocity.setY(physics_body->getRigidBody()->getLinearVelocity().y());

    /*if (velocity != physics_body->getRigidBody()->getLinearVelocity()) {
        physics_body->activate();
        physics_body->getRigidBody()->setLinearVelocity(velocity);
    }*/

    mBtController->setVelocityForTimeInterval(velocity, 0.5);

    Node::onUpdate(time_diff);
}

void Alien::__onMove(Entity::MoveType type, bool is_pressed) {
    bool is_stopped = false;

    switch (type) {
    case FORWARD:
        if (is_pressed && mMoveVector.z > -1.0f)
            mMoveVector.z -= 1.0f; // Ogre Z轴正方向为垂直屏幕向外。
        else if (!is_pressed && mMoveVector.z < 1.0f)
            mMoveVector.z += 1.0f;

        break;

    case BACKWARD:
        if (is_pressed && mMoveVector.z < 1.0f)
            mMoveVector.z += 1.0f;
        else if (!is_pressed && mMoveVector.z > -1.0f)
            mMoveVector.z -= 1.0f;

        break;

    case LEFTWARD:
        if (is_pressed && mMoveVector.x > -1.0f)
            mMoveVector.x -= 1.0f;
        else if (!is_pressed && mMoveVector.x < 1.0f)
            mMoveVector.x += 1.0f;

        break;

    case RIGHTWARD:
        if (is_pressed && mMoveVector.x < 1.0f)
            mMoveVector.x += 1.0f;
        else if (!is_pressed && mMoveVector.x > -1.0f)
            mMoveVector.x -= 1.0f;

        break;

    case STOP:
        mMoveVector.x = 0.0f;
        mMoveVector.z = 0.0f;
        is_stopped = true;

        break;

    default:
        dt::Logger::get().debug("Not processed MoveType!");
    }

    if (is_stopped) {
        this->findComponent<dt::SoundComponent>(WALK_SOUND_COMPONENT)->stopSound();
        this->findComponent<dt::SoundComponent>(RUN_SOUND_COMPONENT)->stopSound();
    } else {
        std::shared_ptr<dt::SoundComponent> move_sound;

        if (mHasSpeededUp) {
            move_sound = this->findComponent<dt::SoundComponent>(RUN_SOUND_COMPONENT);
        } else {
            move_sound = this->findComponent<dt::SoundComponent>(WALK_SOUND_COMPONENT);
        }

        move_sound->playSound();
    }

    mIsMoving = !is_stopped;
}

void Alien::__onJump(bool is_pressed) {
    //auto physics_body = this->findComponent<dt::PhysicsBodyComponent>(PHYSICS_BODY_COMPONENT);

    if (is_pressed && this->isOnGround()) {
        // 调整该处的脉冲值使跳跃更自然。

        /*physics_body->activate();
        physics_body->applyCentralImpulse(0.0f, 20.0f, 0.0f);*/
        mBtController->jump();


        this->findComponent<dt::SoundComponent>(JUMP_SOUND_COMPONENT)->playSound();
    }

    if(!is_pressed) {
        this->findComponent<dt::SoundComponent>(JUMP_SOUND_COMPONENT)->stopSound();
    }
}

void Alien::__onAttack(bool is_pressed) {
    Weapon* weapon = this->getCurWeapon();

    if (weapon != nullptr)
        weapon->attack(is_pressed);
}

void Alien::__onSpeedUp(bool is_pressed) {
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

void Alien::__onChangeWeapon(Weapon::WeaponType type) {
    changeCurWeapon(type);
}

void Alien::__onRemoveWeapon() {
    removeWeapon(getCurWeapon()->getWeaponType());

    for (auto iter = mWeapons.begin() ; iter != mWeapons.end() ; ++iter) {
        if (iter->second != nullptr) {
            changeCurWeapon(iter->second->getWeaponType());

            break;
        }
    }
}

void Alien::__onRemoveWeapon(Weapon::WeaponType type) {
    if (type == getCurWeapon()->getWeaponType()) {
        __onRemoveWeapon();
    } else {
        removeWeapon(type);
    }
}

void Alien::__onAddEquipment(bool is_pressed) {
    if (is_pressed != mIsAddingEquipment) {
        mIsAddingEquipment = is_pressed;
    }
}

void Alien::__onEquiped(dt::PhysicsBodyComponent* object) {
    if (object != nullptr) {
        Prop* prop = dynamic_cast<Prop*>(object->getNode());
        Weapon* weapon = nullptr;

        if (prop != nullptr) {
            switch (prop->getPropType()) {
            case Prop::WEAPON:
                weapon = dynamic_cast<Weapon*>(prop);
                addWeapon(weapon);

                break;

            case Prop::AMMO:
                Ammo* ammo;

                ammo = dynamic_cast<Ammo*>(prop);
                weapon = mWeapons[ammo->getWeaponType()];

                if (weapon != nullptr && weapon->getMaxClip() != weapon->getCurClip()) {
                    weapon->setCurClip(weapon->getCurClip() + ammo->getClipNum());
                    ammo->kill();
                }

                break;

            case Prop::FIRST_AID_KIT:
                FirstAidKit* first_aid_kit;

                first_aid_kit = dynamic_cast<FirstAidKit*>(prop);

                if (this->getCurHealth() < this->getMaxHealth()) {
                    this->setCurHealth(this->getCurHealth() + first_aid_kit->getRecoveryVal());
                    first_aid_kit->kill();
                }

                break;

            case Prop::CRYSTAL:
                Crystal* crystal;

                crystal = dynamic_cast<Crystal*>(prop);

                if (crystal->isUnlocked()) {
                    emit sGetCrystal(this);
                    crystal->kill();
                } else {
                    crystal->beginUnlock();
                }

                break;

            case Prop::VEHICLE:

                break;

            default:
                dt::Logger::get().debug("Unknown prop type.");
            }
        }
    }
}

void Alien::__onGetOffVehicle() { /* =_= 很明显，外星人不是一种载具。*/ }

void Alien::__onLookAround(Ogre::Quaternion body_rot, Ogre::Quaternion agent_rot) {
    Ogre::Quaternion rotation(body_rot.getYaw(), Ogre::Vector3(0.0f, 1.0f, 0.0f));

    //auto physics_body = this->findComponent<dt::PhysicsBodyComponent>(PHYSICS_BODY_COMPONENT);
    btTransform trans;
    trans = mBtGhostObject->getWorldTransform();

    this->findChildNode(Agent::AGENT)->setRotation(agent_rot);

    trans.setRotation(BtOgre::Convert::toBullet(rotation));
    mBtGhostObject->setWorldTransform(trans);
}

void Alien::__onReload() {
    getCurWeapon()->reload();
}