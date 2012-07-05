#include "Alien.h"
#include "Agent.h"
#include "ConfigurationManager.h"
#include "Ammo.h"
#include "FirstAidKit.h"
#include "Crystal.h"
#include "Agent.h"
#include "Vehicle.h"
#include "stateManager.h"
#include "EntityManager.h"
#include "HumanAgent.h"
#include "RaycastNotMeComponent.h"
#include "BattleState.h"

#include <Logic/RaycastComponent.hpp>
#include <Scene/Scene.hpp>
#include <Physics/PhysicsBodyComponent.hpp>

const QString Alien::INTERACTOR_COMPONENT = "interactor";

Alien::Alien(const QString node_name, const QString mesh_handle, const dt::PhysicsBodyComponent::CollisionShapeType collision_shape_type, const btScalar mass,
    const QString walk_sound_handle, const QString jump_sound_handle, const QString run_sound_handle)
    : Character(node_name, mesh_handle, collision_shape_type, mass, walk_sound_handle, jump_sound_handle, run_sound_handle, 10.0f),
    mCurWeapon(nullptr) {
        // 准备好3种武器的位置。
        mWeapons[Weapon::PRIMARY] = nullptr;
        mWeapons[Weapon::SECONDARY] = nullptr;
        mWeapons[Weapon::THROWABLE] = nullptr;
        mIsAddingEquipment = false;
}

Weapon* Alien::getCurWeapon() const {
    return mCurWeapon;
}

void Alien::changeCurWeapon(const Weapon::WeaponType type) {
    Weapon* new_weapon = mWeapons[type];
    HumanAgent* agent;
    agent = dynamic_cast<HumanAgent*>(this->findChildNode(Agent::AGENT).get());

    if ((mCurWeapon == nullptr || mCurWeapon->getWeaponType() != type) && new_weapon != nullptr) {
        if(mCurWeapon != nullptr)
        {
            mCurWeapon->disable();
            if (agent != nullptr) {
                // 只有人类玩家有HUD连接。
                disconnect(mCurWeapon, SIGNAL(sAmmoChanged(uint16_t)), this->getState(), SLOT(__onAmmoChanged(uint16_t)));
                disconnect(mCurWeapon, SIGNAL(sClipNumChanged(uint16_t)), this->getState(), SLOT(__onClipNumChanged(uint16_t)));
            }
            this->findChildNode("getProp")->findChildNode("ammo_node")->setParent(mCurWeapon);
            this->findChildNode(mCurWeapon->getName() + "_muzzle_node")->setParent(mCurWeapon);
        }
        new_weapon->enable();
        mCurWeapon = new_weapon;

        if (agent != nullptr) {
            // 只有人类玩家有HUD连接。
            connect(mCurWeapon, SIGNAL(sAmmoChanged(uint16_t)), this->getState(), SLOT(__onAmmoChanged(uint16_t)));
            connect(mCurWeapon, SIGNAL(sClipNumChanged(uint16_t)), this->getState(), SLOT(__onClipNumChanged(uint16_t)));
        }

        mCurWeapon->findChildNode("ammo_node")->setParent(this->findChildNode("getProp").get());
        mCurWeapon->findChildNode(mCurWeapon->getName() +  "_muzzle_node")->setParent(this);

        emit sAmmoClipChange(mCurWeapon->getCurAmmo(), mCurWeapon->getCurClip());

        this->setCurSpeed(this->getOrigSpeed() - new_weapon->getWeight() * 0.1f);

        if (mHasSpeededUp) {
            __onSpeedUp(true);
        }

        emit sWeaponChanged(mCurWeapon, new_weapon);
    } else if (new_weapon == nullptr) {

        if(mCurWeapon != nullptr)
        {
            if (mCurWeapon->getParent() == this) {
                mCurWeapon->disable();
            }

            if (agent != nullptr) {
                // 只有人类玩家有HUD连接。
                disconnect(mCurWeapon, SIGNAL(sAmmoChanged(uint16_t)), this->getState(), SLOT(__onAmmoChanged(uint16_t)));
                disconnect(mCurWeapon, SIGNAL(sClipNumChanged(uint16_t)), this->getState(), SLOT(__onClipNumChanged(uint16_t)));
            }

            this->findChildNode("getProp")->findChildNode("ammo_node")->setParent(mCurWeapon);
            this->findChildNode(mCurWeapon->getName() + "_muzzle_node")->setParent(mCurWeapon);
        }

        mCurWeapon = nullptr;
        emit sAmmoClipChange(0, 0);
    }
}

void Alien::addWeapon(Weapon* weapon) {
    if (weapon != nullptr) {
        if (mWeapons[weapon->getWeaponType()] != nullptr) {
            removeWeapon(weapon->getWeaponType());
        }

        mWeapons[weapon->getWeaponType()] = weapon;

        weapon->removeComponent(PHYSICS_BODY_COMPONENT);
        weapon->setParent(this);
        weapon->setRotation(Ogre::Quaternion::IDENTITY);
        weapon->setPosition(1.0f, 0.0f, -4.0f);
        weapon->setScale(Ogre::Vector3(20.0f, 20.0f, 20.0f));
        changeCurWeapon(weapon->getWeaponType());
    }
}

void Alien::removeWeapon(const Weapon::WeaponType type) {
    Weapon* weapon = mWeapons[type];

    if (weapon != nullptr) {
        mWeapons[type] = nullptr;

        weapon->enable();
        weapon->setParent((dt::Node*)(this->getScene()));
        weapon->setPosition(this->getPosition().x + 0.0f, this->getPosition().y + 8.0f, this->getPosition().z -3.0f);
        weapon->addComponent<dt::PhysicsBodyComponent>(new dt::PhysicsBodyComponent("prop_mesh", PHYSICS_BODY_COMPONENT, 
            dt::PhysicsBodyComponent::BOX, 0)).get();
        weapon->setScale(Ogre::Vector3(1, 1, 1));
        weapon->findComponent<dt::PhysicsBodyComponent>(PHYSICS_BODY_COMPONENT)->disable();
        weapon->findComponent<dt::PhysicsBodyComponent>(PHYSICS_BODY_COMPONENT)->enable();

        this->setCurSpeed(this->getOrigSpeed());
        if (mHasSpeededUp) {
            __onSpeedUp(true);
        }

        changeCurWeapon(type);

        emit sWeaponRemoved(weapon);
    }
}

void Alien::onInitialize() {
    Character::onInitialize();


    auto node = this->addChildNode(new Node("getProp"));

    auto interactor = node->addComponent<dt::InteractionComponent>(new RaycastNotMeComponent(this->
        findComponent<dt::PhysicsBodyComponent>(PHYSICS_BODY_COMPONENT)->getRigidBody(), INTERACTOR_COMPONENT));
    interactor->setRange(15.0f);
    interactor->setIntervalTime(0.019);
    interactor->setOffset(3.0f);
	
    node->setPosition(this->getEyePosition());
	
    connect(interactor.get(), SIGNAL(sHit(dt::PhysicsBodyComponent*)), this, SLOT(__onEquiped(dt::PhysicsBodyComponent*)));

    this->setOrigSpeed(20.0f);
    this->setCurSpeed(20.0f);

    // death signal
    connect(this, SIGNAL(sIsDead(Character*)), EntityManager::get(), SLOT(__isAlienDead(Character*)));

}

void Alien::onDeinitialize() {
    State* state = this->getState();
    HumanAgent* agent;
    agent = dynamic_cast<HumanAgent*>(this->findChildNode(Agent::AGENT).get());

    if (agent != nullptr) {
        // 只有人类玩家有HUD连接。
        disconnect(this, SIGNAL(sAmmoClipChange(uint16_t, uint16_t)), state, SLOT(__onAmmoClipChange(uint16_t, uint16_t)));
        disconnect(this, SIGNAL(sHealthChanged(uint16_t)), state, SLOT(__onHealthChanged(uint16_t)));
    }

    emit sIsDead(this);

    Character::onDeinitialize();
}

void Alien::onUpdate(double time_diff) {
    this->mIsUpdatingAfterChange = (time_diff == 0);

    if (mIsAddingEquipment) {
        this->findChildNode("getProp")->findComponent<dt::InteractionComponent>(INTERACTOR_COMPONENT)->check();
    }

    Character::onUpdate(time_diff);
}

void Alien::__onAttack(bool is_pressed) {
    Weapon* weapon = this->getCurWeapon();

    if (weapon != nullptr)
        weapon->attack(is_pressed);
}

void Alien::__onChangeWeapon(Weapon::WeaponType type) {
    __onAttack(false);
    changeCurWeapon(type);
}

void Alien::__onRemoveWeapon() {
    removeWeapon(getCurWeapon()->getWeaponType());
    auto iter = mWeapons.begin();
    for ( ; iter != mWeapons.end() ; ++iter) {
        if (iter->second != nullptr) {
            changeCurWeapon(iter->second->getWeaponType());

            break;
        }
    }
    if (iter == mWeapons.end())
    {
        emit sAmmoClipChange(0, 0);
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

        mIsAddingEquipment = false;

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

                if (crystal->hasUnlocked()) {
                    emit sGetCrystal(this);
                    std::cout << "Get Crystal Successfully!!!!!!" << std::endl;
                    crystal->kill();
                } else {
                    crystal->beginUnlock();
                }

                mIsAddingEquipment = true;

                break;

            default:
                dt::Logger::get().debug("Unknown prop type.");
            }
        } else {

            // 如果是载具
            Vehicle* vehicle;
            Entity* entity;

            entity = dynamic_cast<Entity*>(object->getNode());

            if (entity != nullptr) {
                vehicle = dynamic_cast<Vehicle*>(entity);

                if (vehicle != nullptr) {
                    this->findComponent<dt::PhysicsBodyComponent>(PHYSICS_BODY_COMPONENT)->disable();
                    this->findComponent<dt::MeshComponent>(MESH_COMPONENT)->disable();

                    Agent* agent;
                    agent = dynamic_cast<Agent*>(this->findChildNode(Agent::AGENT, false).get());
                    agent->detach();
                    agent->attachTo(vehicle);		

                    this->setParent(vehicle);
                    vehicle->findComponent<dt::PhysicsBodyComponent>(Vehicle::PHYSICS_BODY_COMPONENT)->setMass(vehicle->getMass());
                    vehicle->resetPhysicsBody();
                }
            }
        }
    }
}

void Alien::__onGetOffVehicle() { /* =_= 很明显，外星人不是一种载具。*/ }

void Alien::__onLookAround(Ogre::Quaternion body_rot, Ogre::Quaternion agent_rot) {
    Character::__onLookAround(body_rot, agent_rot);
    this->findChildNode("getProp")->setRotation(this->findChildNode(Agent::AGENT)->getRotation());
}

void Alien::__onReload() {
    getCurWeapon()->reload();
}


void Alien::onKilled() {
    dt::Node* agent_node = this->findChildNode(Agent::AGENT).get();

    if (agent_node != nullptr) {
        HumanAgent* human_agent;

        human_agent = dynamic_cast<HumanAgent*>(agent_node);

        if (human_agent != nullptr) {
            BattleState* battle_state;

            battle_state = dynamic_cast<BattleState*>(this->getState());

            if (battle_state != nullptr) {
                battle_state->fail();
            }
        }

        this->kill();
    }
}
