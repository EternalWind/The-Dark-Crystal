#include "ConfigurationManager.h"
#include <Input/InputManager.hpp>
#include "iostream"

using namespace std;

void main()
{
    ConfigurationManager*conf = ConfigurationManager::getInstance();
    
    if (conf->loadConfig())
    {
        
        cout<<conf->getScreenSetting().getFullScreen()<<endl;

        
        SoundSetting*sound = new SoundSetting();
        sound->setMusic(99);
        conf->setSoundSetting(*sound);
        SoundSetting mSound = conf->getSoundSetting();

        conf->saveConfig();
        cout<<conf->getSoundSetting().getSoundEffect()<<endl;
        cout<<conf->getSoundSetting().getMusic()<<endl;
        cout<<conf->getSoundSetting().getMainVolume()<<endl;

        cout<<conf->getControlSetting().getKey((ControlSetting::KeyFunction)4)<<endl;
        QString m = conf->getControlSetting().getKeyName((ControlSetting::KeyFunction)4);
        QByteArray da(m.toAscii());//toAscii()返回一个QByteArray类型   
        std::cout << da.data()<<endl;

        cout<<conf->getControlSetting().getSensitivity()<<endl;
        cout<<conf->getControlSetting().getYInverted()<<endl;


    }

    conf->saveConfig();
    int a;
    cin>>a;
}