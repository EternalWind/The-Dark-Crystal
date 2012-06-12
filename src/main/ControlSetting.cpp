#include "ControlSetting.h"


ControlSetting::ControlSetting()
{
    mKeyMap[FORWARD] = pair<QString, InputManager::InputCode>("Forward", InputManager::KC_W);
    mKeyMap[BACKWARD] = pair<QString, InputManager::InputCode>("Backward", InputManager::KC_S);
    mKeyMap[LEFTWARD] = pair<QString, InputManager::InputCode>("Leftward", InputManager::KC_A);
    mKeyMap[RIGHTWARD] = pair<QString, InputManager::InputCode>("Rightward", InputManager::KC_D);
    mKeyMap[JUMP] = pair<QString, InputManager::InputCode>("Jump", InputManager::KC_SPACE);
    mKeyMap[SPRINT] = pair<QString, InputManager::InputCode>("Sprint", InputManager::KC_LSHIFT);
    mKeyMap[ARM1] = pair<QString, InputManager::InputCode>("Arm1", InputManager::KC_1);
    mKeyMap[ARM2] = pair<QString, InputManager::InputCode>("Arm2", InputManager::KC_2);
    mKeyMap[ARM3] = pair<QString, InputManager::InputCode>("Arm3", InputManager::KC_3);
    mKeyMap[ATTACK] = pair<QString, InputManager::InputCode>("Attack", InputManager::MC_LEFT);
    mKeyMap[RELOAD] = pair<QString, InputManager::InputCode>("Reload", InputManager::KC_P);
    mKeyMap[GET_OFF] = pair<QString, InputManager::InputCode>("GetOff", InputManager::KC_R);
    mKeyMap[ACTIVATE] = pair<QString, InputManager::InputCode>("Activate", InputManager::KC_E);

    mYInverted = true;

    mSentivity = 10.0f;

}

void ControlSetting::setKey(KeyFunction KeyFunction, InputManager::InputCode key)
{
    mKeyMap[KeyFunction].second = key;
}

InputManager::InputCode ControlSetting::getKey(KeyFunction KeyFunction) const
{
    return mKeyMap.at(KeyFunction).second;
}

QString ControlSetting::getKeyName(KeyFunction KeyFunction) const
{
    return mKeyMap.at(KeyFunction).first;
}

ControlSetting::KeyFunction ControlSetting::keyBegin() const
{
    return FORWARD;
}

ControlSetting::KeyFunction ControlSetting::keyEnd() const
{
    return ACTIVATE;
}

bool ControlSetting::getYInverted() const
{
    return mYInverted;
}

void ControlSetting::setYInverted(bool y_inverted)
{
    mYInverted = y_inverted;
}


float ControlSetting::getSensitivity() const
{
    return mSensitivity;
}

void ControlSetting::setSensitivity(float sensitivity)
{
    mSensitivity = sensitivity;
}