#ifndef CONTROLLER_COMPONENT_H
#define CONTROLLER_COMPONENT_H

#include <Scene/Component.hpp>
#include <Input/InputManager.hpp>

#include <BulletCollision/CollisionShapes/btConvexHullShape.h>
#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>
#include <BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <BulletDynamics/Character/btKinematicCharacterController.h>

#include <QString>

using namespace dt;

class ControllerComponent : public Component
{
    Q_OBJECT
public:
    ControllerComponent(QString name);

    void onInitialize();
    void onDeinitialize();
    void onEnable();
    void onDisable();
    void onUpdate(double time_diff);

    /**
      * Sets the speed the player moves at.
      * @param move_speed The maximum speed, in units per second.
      */
    void setMoveSpeed(float move_speed);

    /**
      * Gets the speed the player moves at.
      * @returns The player speed, in units per second.
      */
    float getMoveSpeed() const;

    /**
      * Sets the mouse sensitivity. Default: 1.0.
      * @param mouse_sensitivity The mouse sensitivity. Default: 1.0.
      */
    void setMouseSensitivity(float mouse_sensitivity);

    /**
      * Gets the mouse sensitivity. Default: 1.0.
      * @returns The mouse sensitivity. Default: 1.0.
      */
    float getMouseSensitivity() const;

    /**
      * Sets whether the mouse y-axis should be inversed.
      * @param mouse_y_inversed Whether the mouse y-axis should be inversed.
      */
    void setMouseYInversed(bool mouse_y_inversed);

    /**
      * Gets whether the mouse y-axis should be inversed.
      * @returns Whether the mouse y-axis should be inversed.
      */
    bool getMouseYInversed() const;

private slots:
    void __onButtonDown(dt::InputManager::InputCode input_code, const OIS::EventArg& event);

    void __onButtonUp(dt::InputManager::InputCode input_code, const OIS::EventArg& event);

    void __onMouseMove(const OIS::MouseEvent& event);
    
signals:
    void sForward(bool is_pressed);
    void sBackward(bool is_pressed);
    void sLeftward(bool is_pressed);
    void sRightward(bool is_pressed);
    void sAttack(bool is_pressed);
    void sJump(bool is_pressed);
    void sActivate(bool is_pressed);
    void sSwitchWeapon(uint32_t weapon_index);
    void sQuickSwitch();
    void sDefend(bool is_pressed);
    void sDash(bool is_pressed);

private:
    std::shared_ptr<btKinematicCharacterController> mBtController;       //!< The bullet's built-in character controller.
    std::shared_ptr<btPairCachingGhostObject> mBtGhostObject;            //!< The bullet's ghost object.;
    float mMouseSensitivity;    //!< The sensitivity of the mouse. Default: 1.0.
    bool mMouseYInversed;       //!< True if the mouse's y-axis should be inversed.
    btVector3 mMove;            //!< Used to keep track of the movement.
    float mMoveSpeed;           //!< The speed to move at. Default: 5.0. In units per second.
};

#endif