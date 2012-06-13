#include "HumanAgent.h"
#include "ConfigurationManager.h"

HumanAgent::HumanAgent(const QString name) : Agent(name) {}

void HumanAgent::onInitialize() {
    auto input_mgr = dt::InputManager::get();

    mCameraComponent = this->addComponent<dt::CameraComponent>(new dt::CameraComponent("camera")).get();

    QObject::connect(input_mgr, SIGNAL(sPressed(dt::InputManager::InputCode, const OIS::EventArg&)),
                          this, SLOT(__onKeyDown(dt::InputManager::InputCode, const OIS::EventArg&)));

    QObject::connect(input_mgr, SIGNAL(sReleased(dt::InputManager::InputCode, const OIS::EventArg&)),
                          this, SLOT(__onKeyUp(dt::InputManager::InputCode, const OIS::EventArg&)));

    QObject::connect(input_mgr, SIGNAL(sMouseMoved(const OIS::MouseEvent&)),
                          this, SLOT(__onMouseMove(const OIS::MouseEvent&)));
}

void HumanAgent::onDeinitialize() {
    auto input_mgr = dt::InputManager::get();

    QObject::disconnect(input_mgr, SIGNAL(sPressed(dt::InputManager::InputCode, const OIS::EventArg&)),
                             this, SLOT(__onKeyDown(dt::InputManager::InputCode, const OIS::EventArg&)));

    QObject::disconnect(input_mgr, SIGNAL(sReleased(dt::InputManager::InputCode, const OIS::EventArg&)),
                             this, SLOT(__onKeyUp(dt::InputManager::InputCode, const OIS::EventArg&)));

    QObject::disconnect(input_mgr, SIGNAL(sMouseMoved(const OIS::MouseEvent&)),
                             this, SLOT(__onMouseMove(const OIS::MouseEvent&)));
}

void HumanAgent::__onKeyDown(dt::InputManager::InputCode code, const OIS::EventArg& event) {
    auto config_mgr = ConfigurationManager::getInstance();
    ControlSetting control_setting = config_mgr->getControlSetting();

    if (code == control_setting.getKey(ControlSetting::ACTIVATE)) {
        emit sAddEquipment(true);
    } else if (code == control_setting.getKey(ControlSetting::ARM1)) {
        emit sChangeWeapon(Weapon::WeaponType::PRIMARY);
    } else if (code == control_setting.getKey(ControlSetting::ARM2)) {
        emit sChangeWeapon(Weapon::WeaponType::SECONDARY);
    } else if (code == control_setting.getKey(ControlSetting::ARM3)) {
        emit sChangeWeapon(Weapon::WeaponType::THROWABLE);
    } else if (code == control_setting.getKey(ControlSetting::ATTACK)) {
        emit sAttack(true);
    } else if (code == control_setting.getKey(ControlSetting::BACKWARD)) {
        emit sMove(Entity::MoveType::BACKWARD, true);
    } else if (code == control_setting.getKey(ControlSetting::FORWARD)) {
        emit sMove(Entity::MoveType::FORWARD, true);
    } else if (code == control_setting.getKey(ControlSetting::JUMP)) {
        emit sJump(true);
    } else if (code == control_setting.getKey(ControlSetting::LEFTWARD)) {
        emit sMove(Entity::MoveType::LEFTWARD, true);
    } else if (code == control_setting.getKey(ControlSetting::RIGHTWARD)) {
        emit sMove(Entity::MoveType::RIGHTWARD, true);
    } else if (code == control_setting.getKey(ControlSetting::SPRINT)) {
        emit sSpeedUp(true);
    } else if (code == control_setting.getKey(ControlSetting::GET_OFF)) {
        emit sGetOffVehicle();
    } else if (code == control_setting.getKey(ControlSetting::RELOAD)) {
        emit sReload();
    }
}

void HumanAgent::__onKeyUp(dt::InputManager::InputCode code, const OIS::EventArg& event) {
    auto config_mgr = ConfigurationManager::getInstance();
    ControlSetting control_setting = config_mgr->getControlSetting();
    auto input_mgr = dt::InputManager::get();
    bool isMoveRelated = false;

    if (code == control_setting.getKey(ControlSetting::ACTIVATE)) {
        emit sAddEquipment(false);
    } else if (code == control_setting.getKey(ControlSetting::ATTACK)) {
        emit sAttack(false);
    } else if (code == control_setting.getKey(ControlSetting::BACKWARD)) {
        isMoveRelated = true;

        emit sMove(Entity::MoveType::BACKWARD, false);
    } else if (code == control_setting.getKey(ControlSetting::FORWARD)) {
        isMoveRelated = true;

        emit sMove(Entity::MoveType::FORWARD, false);
    } else if (code == control_setting.getKey(ControlSetting::JUMP)) {
        emit sJump(false);
    } else if (code == control_setting.getKey(ControlSetting::LEFTWARD)) {
        isMoveRelated = true;

        emit sMove(Entity::MoveType::LEFTWARD, false);
    } else if (code == control_setting.getKey(ControlSetting::RIGHTWARD)) {
        isMoveRelated = true;

        emit sMove(Entity::MoveType::RIGHTWARD, false);
    } else if (code == control_setting.getKey(ControlSetting::SPRINT)) {
        emit sSpeedUp(false);
    }

    if (isMoveRelated) {
        if (input_mgr->isPressed(code)) {
            emit sMove(Entity::MoveType::STOP, true);
        }
    }
}

void HumanAgent::__onMouseMove(const OIS::MouseEvent& event) {
    // �ο���Ducttape��AdvancedPlayerComponent�е�_handleMouseMove������
    auto config_mgr = ConfigurationManager::getInstance();
    ControlSetting control_setting = config_mgr->getControlSetting();

    float factor = control_setting.getSensitivity() * -0.01;

    float dx = event.state.X.rel * factor;
    float dy = event.state.Y.rel * factor * (control_setting.getYInverted() ? -1 : 1);

    if (dx != 0 || dy != 0) {
        // watch out for da gimbal lock !!

        Ogre::Matrix3 orientMatrix;
        getRotation(dt::Node::SCENE).ToRotationMatrix(orientMatrix);

        Ogre::Radian yaw, pitch, roll;
        orientMatrix.ToEulerAnglesYXZ(yaw, pitch, roll);

        pitch += Ogre::Radian(dy);
        yaw += Ogre::Radian(dx);

        // do not let it look completely vertical, or the yaw will break
        if (pitch > Ogre::Degree(89.9))
            pitch = Ogre::Degree(89.9);

        if (pitch < Ogre::Degree(-89.9))
            pitch = Ogre::Degree(-89.9);

        orientMatrix.FromEulerAnglesYXZ(yaw, pitch, roll);

        Ogre::Quaternion rot;
        rot.FromRotationMatrix(orientMatrix);
        setRotation(rot, dt::Node::SCENE);

        emit sLookAround(rot);
    }
}