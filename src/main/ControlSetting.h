# ifndef _CONTROL_SETTING_H
# define _CONTROL_SETTING_H

#include <Input/InputManager.hpp>

#include <map>

using namespace dt;
using namespace std;

class ControlSetting
{
public:
    ControlSetting();

    //KeySetting
    enum KeyFunction
    {
        FORWARD,
        BACKWARD,
        LEFTWARD,
        RIGHTWARD,
        JUMP,
        SPRINT,
        ARM1,
        ARM2,
        ARM3,
        ATTACK,
        RELOAD,
        THROW,
        GET_OFF,
        ACTIVATE
    };

    void setKey(KeyFunction KeyFunction, InputManager::InputCode key);

    InputManager::InputCode getKey(KeyFunction KeyFunction) const;

    QString getKeyName(KeyFunction KeyFunction) const;

    KeyFunction keyBegin() const;

    KeyFunction keyEnd() const;

    //YInvertedSetting
    bool getYInverted() const;

    void setYInverted(bool y_inverted);

    //SensitivitySetting
    float getSensitivity() const;

    void setSensitivity(float sensitivity);


private:
    map<KeyFunction, pair<QString, InputManager::InputCode>> mKeyMap;
    bool mYInverted;
    float mSensitivity;
};

#endif