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
        emit sChangeWeapon(Weapon::PRIMARY);
    } else if (code == control_setting.getKey(ControlSetting::ARM2)) {
        emit sChangeWeapon(Weapon::SECONDARY);
    } else if (code == control_setting.getKey(ControlSetting::ARM3)) {
        emit sChangeWeapon(Weapon::THROWABLE);
    } else if (code == control_setting.getKey(ControlSetting::ATTACK)) {
        emit sAttack(true);
    } else if (code == control_setting.getKey(ControlSetting::BACKWARD)) {
        emit sMove(Entity::BACKWARD, true);
    } else if (code == control_setting.getKey(ControlSetting::FORWARD)) {
        emit sMove(Entity::FORWARD, true);
    } else if (code == control_setting.getKey(ControlSetting::JUMP)) {
        emit sJump(true);
    } else if (code == control_setting.getKey(ControlSetting::LEFTWARD)) {
        emit sMove(Entity::LEFTWARD, true);
    } else if (code == control_setting.getKey(ControlSetting::RIGHTWARD)) {
        emit sMove(Entity::RIGHTWARD, true);
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

        emit sMove(Entity::BACKWARD, false);
    } else if (code == control_setting.getKey(ControlSetting::FORWARD)) {
        isMoveRelated = true;

        emit sMove(Entity::FORWARD, false);
    } else if (code == control_setting.getKey(ControlSetting::JUMP)) {
        emit sJump(false);
    } else if (code == control_setting.getKey(ControlSetting::LEFTWARD)) {
        isMoveRelated = true;

        emit sMove(Entity::LEFTWARD, false);
    } else if (code == control_setting.getKey(ControlSetting::RIGHTWARD)) {
        isMoveRelated = true;

        emit sMove(Entity::RIGHTWARD, false);
    } else if (code == control_setting.getKey(ControlSetting::SPRINT)) {
        emit sSpeedUp(false);
    }

    if (isMoveRelated) {
        if (!input_mgr->isPressed(control_setting.getKey(ControlSetting::BACKWARD)) && !input_mgr->isPressed(control_setting.getKey(
            ControlSetting::FORWARD)) && !input_mgr->isPressed(control_setting.getKey(ControlSetting::RIGHTWARD)) && !input_mgr->
            isPressed(control_setting.getKey(ControlSetting::LEFTWARD))) {
                emit sMove(Entity::STOP, true);
        }
    }
}

void HumanAgent::__onMouseMove(const OIS::MouseEvent& event) {
    // 参考自Ducttape里AdvancedPlayerComponent中的_handleMouseMove方法。
    auto config_mgr = ConfigurationManager::getInstance();
    ControlSetting control_setting = config_mgr->getControlSetting();

    float factor = control_setting.getSensitivity() * -0.01;

    float dx = event.state.X.rel * factor;
    float dy = event.state.Y.rel * factor * (control_setting.getYInverted() ? -1 : 1);

    if (dx != 0 || dy != 0) {
		Ogre::Radian yaw = Ogre::Radian(dx);
		Ogre::Radian pitch = this->getRotation().getPitch() + Ogre::Radian(dy);

        // do not let it look completely vertical, or the yaw will break
        if (pitch > Ogre::Degree(89.9))
            pitch = Ogre::Degree(89.9);

        if (pitch < Ogre::Degree(-89.9))
            pitch = Ogre::Degree(-89.9);

		emit sLookAround(
			this->getParent()->getRotation() * Ogre::Quaternion(yaw, Ogre::Vector3(0.0f, 1.0f, 0.0f)), 
			Ogre::Quaternion(pitch, Ogre::Vector3(1, 0, 0))
			);
    }
}