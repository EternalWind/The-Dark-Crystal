#include "Agent.h"

#include <Scene/Scene.hpp>

const QString Agent::AGENT = "agent";

Agent::Agent(const QString name) : dt::Node(AGENT), mName(name) {}

void Agent::attachTo(Entity* entity) {
    if (entity != nullptr) {
        Agent* agent = dynamic_cast<Agent*>(entity->findChildNode(AGENT, false).get());

        if (agent != nullptr) {
            agent->detach();
        }

        this->setParent(entity);

        QObject::connect(this, SIGNAL(sMove(Entity::MoveType, bool)), entity, SLOT(__onMove(Entity::MoveType, bool)));
        QObject::connect(this, SIGNAL(sAttack(bool)), entity, SLOT(__onAttack(bool)));
        QObject::connect(this, SIGNAL(sJump(bool)), entity, SLOT(__onJump(bool)));
        QObject::connect(this, SIGNAL(sSpeedUp(bool)), entity, SLOT(__onSpeedUp(bool)));
        QObject::connect(this, SIGNAL(sGetOffVehicle()), entity, SLOT(__onGetOffVehicle()));
        QObject::connect(this, SIGNAL(sAddEquipment(bool)), entity, SLOT(__onAddEquipment(bool)));
        QObject::connect(this, SIGNAL(sChangeWeapon(Weapon::WeaponType)), entity, SLOT(__onChangeWeapon(Weapon::WeaponType)));
        QObject::connect(this, SIGNAL(sRemoveWeapon()), entity, SLOT(__onRemoveWeapon()));
        QObject::connect(this, SIGNAL(sRemoveWeapon(Weapon::WeaponType)), entity, SLOT(__onRemoveWeapon(Weapon::WeaponType)));
        QObject::connect(this, SIGNAL(sLookAround(Ogre::Quaternion)), entity, SLOT(__onLookAround(Ogre::Quaternion)));
        QObject::connect(this, SIGNAL(sReload()), entity, SLOT(__onReload()));
    }
}

void Agent::detach() {
    Entity* entity = dynamic_cast<Entity*>(this->getParent());

    if (entity != nullptr) {
        QObject::disconnect(this, SIGNAL(sMove(Entity::MoveType, bool)), entity, SLOT(__onMove(MoveType, bool)));
        QObject::disconnect(this, SIGNAL(sAttack(bool)), entity, SLOT(__onAttack(bool)));
        QObject::disconnect(this, SIGNAL(sJump(bool)), entity, SLOT(__onJump(bool)));
        QObject::disconnect(this, SIGNAL(sSpeedUp(bool)), entity, SLOT(__onSpeedUp(bool)));
        QObject::disconnect(this, SIGNAL(sGetOffVehicle()), entity, SLOT(__onGetOffVehicle()));
        QObject::disconnect(this, SIGNAL(sAddEquipment(bool)), entity, SLOT(__onAddEquipment(bool)));
        QObject::disconnect(this, SIGNAL(sChangeWeapon(Weapon::WeaponType)), entity, SLOT(__onChangeWeapon(Weapon::WeaponType)));
        QObject::disconnect(this, SIGNAL(sRemoveWeapon()), entity, SLOT(__onRemoveWeapon()));
        QObject::disconnect(this, SIGNAL(sRemoveWeapon(Weapon::WeaponType)), entity, SLOT(__onRemoveWeapon(Weapon::WeaponType)));
        QObject::disconnect(this, SIGNAL(sLookAround(Ogre::Quaternion)), entity, SLOT(__onLookAround(Ogre::Quaternion)));
        QObject::disconnect(this, SIGNAL(sReload()), entity, SLOT(__onReload()));

        this->setParent(this->getScene());
    }
}