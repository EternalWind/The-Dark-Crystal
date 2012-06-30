#include "InitialState.h"

#include "ConfigurationManager.h"
#include "MenuState.h"

#include <Graphics/DisplayManager.hpp>
#include <Core/ResourceManager.hpp>
#include <Scene/StateManager.hpp>

#include <SFML/Audio/Listener.hpp>

InitialState::InitialState() {}

void InitialState::onInitialize() {
    dt::ResourceManager::get()->addResourceLocation("", "FileSystem");
    dt::ResourceManager::get()->addResourceLocation("gui", "FileSystem");
    dt::ResourceManager::get()->addResourceLocation("gui/digits", "FileSystem");
    dt::ResourceManager::get()->addResourceLocation("images", "FileSystem");
    dt::ResourceManager::get()->addResourceLocation("musics", "FileSystem");
    dt::ResourceManager::get()->addResourceLocation("sounds", "FileSystem");
    dt::ResourceManager::get()->addResourceLocation("texts", "FileSystem");
    dt::ResourceManager::get()->addResourceLocation("models", "FileSystem");
    dt::ResourceManager::get()->addResourceLocation("scripts", "FileSystem");

    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    dt::Scene::SceneSP scene = addScene(new dt::Scene("initial_scene"));
    
    auto camnode = scene->addChildNode(new dt::Node("camera"));
    camnode->addComponent(new dt::CameraComponent("camera_component"));

    ConfigurationManager* config_mgr = ConfigurationManager::getInstance();
    config_mgr->loadConfig();

    auto display_settings = config_mgr->getScreenSetting();
    auto sound_settings = config_mgr->getSoundSetting();

    if (display_settings.getFullScreen()) {
        dt::DisplayManager::get()->setFullscreen(true);
    } else {
        dt::DisplayManager::get()->setWindowSize(640, 400);
    }

    sf::Listener::setGlobalVolume(sound_settings.getMainVolume());
}

void InitialState::onDeinitialize() {}

void InitialState::updateStateFrame(double simulation_frame_time) {
        dt::StateManager::get()->setNewState(new MenuState());
}