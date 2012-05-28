#include "ControllerComponent.h"
#include "ConfigurationManager.h"

#include <Scene/Node.hpp>
#include <Scene/Scene.hpp>

#include <BtOgreGP.h>

ControllerComponent::ControllerComponent(QString name)
    : Component(name),
      mBtController(nullptr),
      mBtGhostObject(nullptr),
      mMouseSensitivity(1.0),
      mMouseYInversed(false),
      mMove(0, 0, 0),
      mMoveSpeed(5.0f) {}

void ControllerComponent::onInitialize()
{
    btTransform  start_trans;
    start_trans.setIdentity();
    start_trans.setOrigin(BtOgre::Convert::toBullet(getNode()->getPosition(Node::SCENE)));
    start_trans.setRotation(BtOgre::Convert::toBullet(getNode()->getRotation(Node::SCENE)));

    btScalar character_height = 1.75;
    btScalar character_width = 0.44;
    btConvexShape* capsule = new btCapsuleShape(character_width, character_height);

    mBtGhostObject = std::shared_ptr<btPairCachingGhostObject>(new btPairCachingGhostObject());
    mBtGhostObject->setWorldTransform(start_trans);
    mBtGhostObject->setCollisionShape(capsule);
    mBtGhostObject->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);
    /////////////////////////////////////////////////////////////////////////////////////////////////
    //TODO: Make a better way to distinguish between AdvancedPlayerComponent and PhysicsBodyComponent.
    //For now, it's just a null pointer check to do this.
    mBtGhostObject->setUserPointer(nullptr);
    /////////////////////////////////////////////////////////////////////////////////////////////////
    mBtController = std::shared_ptr<btKinematicCharacterController>
        (new btKinematicCharacterController(mBtGhostObject.get(), capsule, 1));
    getNode()->getScene()->getPhysicsWorld()->getBulletWorld()->addCollisionObject(mBtGhostObject.get());
    getNode()->getScene()->getPhysicsWorld()->getBulletWorld()->addAction(mBtController.get());

    if(!QObject::connect(dt::InputManager::get(), SIGNAL(sPressed(dt::InputManager::InputCode, const OIS::EventArg&)),
        this,                SLOT(__onButtonDown(dt::InputManager::InputCode, const OIS::EventArg&)), Qt::DirectConnection))
    {
            Logger::get().error("Cannot connect signal sPressed with " + getName()
                + "'s input handling slot.");
    }

    if(!QObject::connect(dt::InputManager::get(), SIGNAL(sReleased(dt::InputManager::InputCode, const OIS::EventArg&)),
                         this,                SLOT(__onButtonUp(dt::InputManager::InputCode, const OIS::EventArg&)), Qt::DirectConnection))
    {
            Logger::get().error("Cannot connect signal sReleased with " + getName()
                + "'s input handling slot.");
    }

    if(!QObject::connect(dt::InputManager::get(), SIGNAL(sMouseMoved(const OIS::MouseEvent&)),
                         this,                SLOT(__onMouseMove(const OIS::MouseEvent&)), Qt::DirectConnection))
    {
            Logger::get().error("Cannot connect signal sMouseMoved with " + getName()
                + "'s input handling slot.");
    }
}

void ControllerComponent::onDeinitialize()
{
    if(!QObject::disconnect(this, SLOT(__onButtonDown(dt::InputManager::InputCode, const OIS::EventArg&))))
    {
            Logger::get().error("Cannot disconnect signal sPressed with " + getName()
                + "'s input handling slot.");
    }

    if(!QObject::disconnect(this, SLOT(__onButtonUp(dt::InputManager::InputCode, const OIS::EventArg&))))
    {
            Logger::get().error("Cannot disconnect signal sReleased with " + getName()
                + "'s input handling slot.");
    }

    if(!QObject::disconnect(this, SLOT(__onMouseMove(const OIS::MouseEvent&))))
    {
            Logger::get().error("Cannot disconnect signal sMouseMoved with " + getName()
                + "'s input handling slot.");
    }
}

void ControllerComponent::onEnable()
{
    // Connect all the signals/slots.
    if(!QObject::connect(dt::InputManager::get(), SIGNAL(sPressed(dt::InputManager::InputCode, const OIS::EventArg&)),
                         this,                SLOT(__onButtonDown(dt::InputManager::InputCode, const OIS::EventArg&))))
    {
            Logger::get().error("Cannot connect signal sPressed with " + getName()
                + "'s input handling slot.");
    }

    if(!QObject::connect(dt::InputManager::get(), SIGNAL(sReleased(dt::InputManager::InputCode, const OIS::EventArg&)),
                         this,                SLOT(__onButtonUp(dt::InputManager::InputCode, const OIS::EventArg&))))
    {
            Logger::get().error("Cannot connect signal sReleased with " + getName()
                + "'s input handling slot.");
    }

    if(!QObject::connect(dt::InputManager::get(), SIGNAL(sMouseMoved(const OIS::MouseEvent&)),
                         this,                SLOT(__onMouseMove(const OIS::MouseEvent&))))
    {
            Logger::get().error("Cannot connect signal sMouseMoved with " + getName()
                + "'s input handling slot.");
    }

    //Re-sychronize it.
    btTransform transform;
    transform.setIdentity();
    transform.setOrigin(BtOgre::Convert::toBullet(getNode()->getPosition(Node::SCENE)));
    transform.setRotation(BtOgre::Convert::toBullet(getNode()->getRotation(Node::SCENE)));

    mBtGhostObject->setWorldTransform(transform);
    getNode()->getScene()->getPhysicsWorld()->getBulletWorld()->addCollisionObject(mBtGhostObject.get());
}

void ControllerComponent::onDisable()
{
    // Disconnect all the signals/slots.
    if(!QObject::disconnect(this, SLOT(__onButtonDown(dt::InputManager::InputCode, const OIS::EventArg&))))
    {
            Logger::get().error("Cannot disconnect signal sPressed with " + getName()
                + "'s input handling slot.");
    }

    if(!QObject::disconnect(this, SLOT(__onButtonUp(dt::InputManager::InputCode, const OIS::EventArg&))))
    {
            Logger::get().error("Cannot disconnect signal sReleased with " + getName()
                + "'s input handling slot.");
    }

    if(!QObject::disconnect(this, SLOT(__onMouseMove(const OIS::MouseEvent&))))
    {
            Logger::get().error("Cannot disconnect signal sMouseMoved with " + getName()
                + "'s input handling slot.");
    }

    getNode()->getScene()->getPhysicsWorld()->getBulletWorld()->removeCollisionObject(mBtGhostObject.get());
}

void ControllerComponent::onUpdate(double time_diff)
{
    Ogre::Vector3 move;
    Ogre::Quaternion quaternion;
    btTransform trans;

    quaternion = Ogre::Quaternion(getNode()->getRotation().getYaw(), Ogre::Vector3(0.0, 1.0, 0.0));
    move = quaternion * BtOgre::Convert::toOgre(mMove);
    move.normalise();
    move *= mMoveSpeed;
    
    mBtController->setVelocityForTimeInterval(BtOgre::Convert::toBullet(move), 0.5);

    trans = mBtGhostObject->getWorldTransform();

    getNode()->setPosition(BtOgre::Convert::toOgre(trans.getOrigin()), Node::SCENE);
}

void ControllerComponent::setMoveSpeed(float move_speed)
{
    mMoveSpeed = move_speed;
}

float ControllerComponent::getMoveSpeed() const
{
    return mMoveSpeed;
}

void ControllerComponent::setMouseSensitivity(float mouse_sensitivity)
{
    mMouseSensitivity = mouse_sensitivity;
}

float ControllerComponent::getMouseSensitivity() const
{
    return mMouseSensitivity;
}

void ControllerComponent::setMouseYInversed(bool mouse_y_inversed)
{
    mMouseYInversed = mouse_y_inversed;
}

bool ControllerComponent::getMouseYInversed() const
{
    return mMouseYInversed;
}

void ControllerComponent::__onButtonDown(dt::InputManager::InputCode input_code, const OIS::EventArg& event)
{
    ConfigurationManager* config_mgr = ConfigurationManager::getInstance();
    ControlSetting control_setting = config_mgr->getControlSetting();

    dt::InputManager::InputCode forward = control_setting.getKey(ControlSetting::FORWARD);
    dt::InputManager::InputCode backward = control_setting.getKey(ControlSetting::BACKWARD);
    dt::InputManager::InputCode leftward = control_setting.getKey(ControlSetting::LEFTWARD);
    dt::InputManager::InputCode rightward = control_setting.getKey(ControlSetting::RIGHTWARD);
    dt::InputManager::InputCode jump = control_setting.getKey(ControlSetting::JUMP);
    dt::InputManager::InputCode attack = control_setting.getKey(ControlSetting::ATTACK);
    dt::InputManager::InputCode defend = control_setting.getKey(ControlSetting::DEFENSE);
    dt::InputManager::InputCode activate = control_setting.getKey(ControlSetting::ACTIVATE);
    dt::InputManager::InputCode arm1 = control_setting.getKey(ControlSetting::ARM1);
    dt::InputManager::InputCode arm2 = control_setting.getKey(ControlSetting::ARM2);
    dt::InputManager::InputCode arm3 = control_setting.getKey(ControlSetting::ARM3);
    dt::InputManager::InputCode arm4 = control_setting.getKey(ControlSetting::ARM4);
    dt::InputManager::InputCode dash = control_setting.getKey(ControlSetting::SPRINT);
    dt::InputManager::InputCode quick_switch = control_setting.getKey(ControlSetting::SWITCH);

    if(input_code == forward)
    {
        mMove.setZ(mMove.getZ() - 1.0f);

        emit sForward(true);
    }
    else if(input_code == backward)
    {
        mMove.setZ(mMove.getZ() + 1.0f);

        emit sBackward(true);
    }
    else if(input_code == leftward)
    {
        mMove.setX(mMove.getX() - 1.0f);

        emit sLeftward(true);
    }
    else if(input_code == rightward)
    {
        mMove.setX(mMove.getX() + 1.0f);

        emit sRightward(true);
    }
    else if(input_code == attack)
    {
        emit sAttack(true);
    }
    else if(input_code == defend)
    {
        emit sDefend(true);
    }
    else if(input_code == activate)
    {
        emit sActivate(true);
    }
    else if(input_code == arm1)
    {
        emit sSwitchWeapon(1);
    }
    else if(input_code == arm2)
    {
        emit sSwitchWeapon(2);
    }
    else if(input_code == arm3)
    {
        emit sSwitchWeapon(3);
    }
    else if(input_code == arm4)
    {
        emit sSwitchWeapon(4);
    }
    else if(input_code == quick_switch)
    {
        emit sQuickSwitch();
    }
    else if(input_code == dash)
    {
        emit sDash(true);
    }

    if(input_code == jump && mBtController->onGround())
    {
        mBtController->jump();

        emit sJump(true);
    }
}

void ControllerComponent::__onMouseMove(const OIS::MouseEvent& event) {
    float factor = mMouseSensitivity * -0.01;

    float dx = event.state.X.rel * factor;
    float dy = event.state.Y.rel * factor * (mMouseYInversed ? -1 : 1);

    if(dx != 0 || dy != 0) {
        // watch out for da gimbal lock !!

        Ogre::Matrix3 orientMatrix;
        getNode()->getRotation().ToRotationMatrix(orientMatrix);

        Ogre::Radian yaw, pitch, roll;
        orientMatrix.ToEulerAnglesYXZ(yaw, pitch, roll);

        pitch += Ogre::Radian(dy);
        yaw += Ogre::Radian(dx);

        // do not let it look completely vertical, or the yaw will break
        if(pitch > Ogre::Degree(89.9))
            pitch = Ogre::Degree(89.9);

        if(pitch < Ogre::Degree(-89.9))
            pitch = Ogre::Degree(-89.9);

        orientMatrix.FromEulerAnglesYXZ(yaw, pitch, roll);

        Ogre::Quaternion rot;
        rot.FromRotationMatrix(orientMatrix);
        getNode()->setRotation(rot);
    }
}

void ControllerComponent::__onButtonUp(dt::InputManager::InputCode input_code, const OIS::EventArg& event) {
    ConfigurationManager* config_mgr = ConfigurationManager::getInstance();
    ControlSetting control_setting = config_mgr->getControlSetting();

    dt::InputManager::InputCode forward = control_setting.getKey(ControlSetting::FORWARD);
    dt::InputManager::InputCode backward = control_setting.getKey(ControlSetting::BACKWARD);
    dt::InputManager::InputCode leftward = control_setting.getKey(ControlSetting::LEFTWARD);
    dt::InputManager::InputCode rightward = control_setting.getKey(ControlSetting::RIGHTWARD);
    dt::InputManager::InputCode attack = control_setting.getKey(ControlSetting::ATTACK);
    dt::InputManager::InputCode defend = control_setting.getKey(ControlSetting::DEFENSE);
    dt::InputManager::InputCode activate = control_setting.getKey(ControlSetting::ACTIVATE);
    dt::InputManager::InputCode dash = control_setting.getKey(ControlSetting::SPRINT);

    if(input_code == forward) {
        mMove.setZ(mMove.getZ() + 1.0f);
    }
    else if(input_code == backward) {
        mMove.setZ(mMove.getZ() - 1.0f);
    }
    else if(input_code == leftward) {
        mMove.setX(mMove.getX() + 1.0f);
    }
    else if(input_code == rightward) {
        mMove.setX(mMove.getX() - 1.0f);
    }
    else if(input_code == attack)
    {
        emit sAttack(false);
    }
    else if(input_code == defend)
    {
        emit sDefend(false);
    }
    else if(input_code == activate)
    {
        emit sActivate(false);
    }
    else if(input_code == dash)
    {
        emit sDash(false);
    }
}
