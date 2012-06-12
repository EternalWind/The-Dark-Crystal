#include "ConfigurationManager.h"

#include <Utils/Logger.hpp>
#include <Utils/Utils.hpp>

std::shared_ptr<ConfigurationManager> ConfigurationManager::mInstance = 
    std::shared_ptr<ConfigurationManager>(new ConfigurationManager());

ConfigurationManager::ConfigurationManager()
{
}

ConfigurationManager* ConfigurationManager::getInstance()
{
    return mInstance.get();
}

bool ConfigurationManager::loadConfig()
{
    QFile config_file(CONFIG_FILE);
    QDomDocument doc;

    // If the file doesn't exist...
    if(!config_file.open(QIODevice::ReadOnly))
    {
        Logger::get().info("The configuration file doesn't exist. Trying to create a new one.");

        if(!saveConfig())
        {
            Logger::get().error("Failed to open the configuration file.");

            return false;
        }
        else
        {
            Logger::get().info("Created a new configuration file.");

            return true;
        }
    }

    // Else...
    if(doc.setContent(&config_file))
    {
        QDomElement root = doc.documentElement();

        // Only loads the key settings for now.
        // TODO: Add other loading stuff here.
        for(QDomElement config_node = root.firstChildElement() ; !config_node.isNull() ; config_node = config_node.nextSiblingElement())
        {
            QString tag_name = config_node.tagName();

            if (tag_name == SCREEN_SETTING)
            {
                __loadSreenSetting(config_node);
            }
            else if (tag_name == SOUND_SETTING)
            {
                __loadSoundSetting(config_node);
            }
            else if (tag_name == CONTROL_SETTING)
            {
                __loadControlSetting(config_node);
            }
            else if (tag_name == QA_SETTING)
            {
                __loadQASetting(config_node);
            }
        }
    }
    else
    {
        Logger::get().error("Failed to read from the configuration file.");

        return false;
    }

    return true;
}

bool ConfigurationManager::saveConfig() const
{
    QFile config_file(CONFIG_FILE);
    QDomDocument doc;

    if(!config_file.open(QIODevice::WriteOnly))
    {
        Logger::get().error("Cannot create the configuration file.");

        return false;
    }

    // The root.
    auto root = doc.createElement(CONFIG_ROOT);
    doc.appendChild(root);

    // Only saves the key settings for now.
    // TODO: Add other saving stuff here.
    root.appendChild(__saveScreenSetting(doc));
    root.appendChild(__saveSoundSetting(doc));
    root.appendChild(__saveControlSetting(doc));
    root.appendChild(__saveQASetting(doc));
    // Save it to the file.
    QTextStream out(&config_file);
    out << doc.toString();

    // Close the file.
    config_file.close();

    return true;
}

ScreenSetting ConfigurationManager::getScreenSetting() const
{
    return mScreenSetting;
}

void ConfigurationManager::setScreenSetting(ScreenSetting screen_setting)
{
    mScreenSetting = screen_setting;
}

SoundSetting ConfigurationManager::getSoundSetting() const
{
    return mSoundSetting;
}

void ConfigurationManager::setSoundSetting(SoundSetting sound_setting)
{
    mSoundSetting = sound_setting;
}

ControlSetting ConfigurationManager::getControlSetting() const
{
    return mControlSetting;
}

void ConfigurationManager::setControlSetting(ControlSetting control_setting)
{
    mControlSetting = control_setting;
}


QASetting ConfigurationManager::getQASetting() const
{
    return mQASetting;
}

void ConfigurationManager::setQASetting(QASetting qa_setting)
{
    mQASetting = qa_setting;
}

void ConfigurationManager::__loadQASetting(const QDomElement& element)
{
    auto is_enable_node = element.firstChildElement(IS_ENABLE);
    bool is_enable_value = is_enable_node.attribute(VALUE).toUInt();
    mQASetting.setIsQAEnable(is_enable_value);
}

QDomElement ConfigurationManager::__saveQASetting(QDomDocument& doc) const
{
     auto QA_setting = doc.createElement(QA_SETTING);
     auto is_enable = doc.createElement(IS_ENABLE);
     is_enable.setAttribute(VALUE, mQASetting.getIsQAEnable());
     QA_setting.appendChild(is_enable);

     return QA_setting;
}

void ConfigurationManager::__loadSreenSetting(const QDomElement& element)
{

    auto full_screen_node = element.firstChildElement(FULLSCREEN);
    bool full_screen_value = full_screen_node.attribute(VALUE).toInt();
    mScreenSetting.setFullScreen(full_screen_value);
}

QDomElement ConfigurationManager::__saveScreenSetting(QDomDocument& doc) const
{
    auto screen_setting = doc.createElement(SCREEN_SETTING);
    auto full_screen_setting = doc.createElement(FULLSCREEN);
    full_screen_setting.setAttribute(VALUE, mScreenSetting.getFullScreen());
    screen_setting.appendChild(full_screen_setting);
    return screen_setting;
}

void ConfigurationManager::__loadSoundSetting(const QDomElement& element)
{
    auto sound_effect_node = element.firstChildElement(SOUND_EFFECT);
    unsigned sound_effect_value = sound_effect_node.attribute(VALUE).toUInt();
    mSoundSetting.setSoundEffect(sound_effect_value);

    auto music_node = element.firstChildElement(MUSIC);
    unsigned music_value = music_node.attribute(VALUE).toUInt();
    mSoundSetting.setMusic(music_value);

    auto main_volume_node = element.firstChildElement(MAIN_VOLUME);
    unsigned main_volume_value = main_volume_node.attribute(VALUE).toUInt();
    mSoundSetting.setMainVolume(main_volume_value);
}

QDomElement ConfigurationManager::__saveSoundSetting(QDomDocument& doc) const
{
    auto sound_setting = doc.createElement(SOUND_SETTING);

    auto sound_effect_setting = doc.createElement(SOUND_EFFECT);
    sound_effect_setting.setAttribute(VALUE, mSoundSetting.getSoundEffect());
    sound_setting.appendChild(sound_effect_setting);

    auto music_setting = doc.createElement(MUSIC);
    music_setting.setAttribute(VALUE, mSoundSetting.getMusic());
    sound_setting.appendChild(music_setting);

    auto main_volume_setting = doc.createElement(MAIN_VOLUME);
    main_volume_setting.setAttribute(VALUE, mSoundSetting.getMainVolume());
    sound_setting.appendChild(main_volume_setting);

    return sound_setting;
}

void ConfigurationManager::__loadControlSetting(const QDomElement& element)
{
    auto key_setting = element.firstChildElement(KEY);
    for(auto key_node = key_setting.firstChildElement() ; !key_node.isNull() ; key_node = key_node.nextSiblingElement())
    {
        ControlSetting::KeyFunction function_code = (ControlSetting::KeyFunction)key_node.attribute(KEY_FUNCTION).toUInt();
        InputManager::InputCode input_code = (InputManager::InputCode)key_node.attribute(KEY_CODE).toUInt();

        mControlSetting.setKey(function_code, input_code);
    }

    auto y_inverted_setting = element.firstChildElement(Y_INVERTED);
    bool y_inverted_value = y_inverted_setting.attribute(VALUE).toUInt();
    mControlSetting.setYInverted(y_inverted_value);

    auto sensitivity_setting = element.firstChildElement(SENSITIVITY);
    unsigned sensitivity_value = sensitivity_setting.attribute(VALUE).toUInt();
    mControlSetting.setSentivity(sensitivity_value);

}

QDomElement ConfigurationManager::__saveControlSetting(QDomDocument& doc) const
{
    auto control_setting = doc.createElement(CONTROL_SETTING);

    auto key_setting = doc.createElement(KEY);
    for(unsigned key_function = (unsigned)mControlSetting.keyBegin() ; key_function <= (unsigned)mControlSetting.keyEnd(); ++key_function)
    {
        auto element = doc.createElement(mControlSetting.getKeyName((ControlSetting::KeyFunction)key_function));

        element.setAttribute(KEY_FUNCTION, key_function);
        element.setAttribute(KEY_CODE, (unsigned)mControlSetting.getKey((ControlSetting::KeyFunction)key_function));

        key_setting.appendChild(element);
    }
    control_setting.appendChild(key_setting);

    auto YInverted_setting = doc.createElement(Y_INVERTED);
    YInverted_setting.setAttribute(VALUE,mControlSetting.getYInverted());
    control_setting.appendChild(YInverted_setting);

    auto sensitivity_setting = doc.createElement(SENSITIVITY);
    sensitivity_setting.setAttribute(VALUE,mControlSetting.getSensitivity());
    control_setting.appendChild(sensitivity_setting);

    return control_setting;


}