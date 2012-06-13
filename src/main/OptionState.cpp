#include "OptionState.h"

#include <Core/Root.hpp>
#include <Scene/StateManager.hpp>
#include <Core/ResourceManager.hpp>
#include <Graphics/CameraComponent.hpp>
#include <Gui/GuiManager.hpp>

void OptionState::onInitialize() {
	auto scene = addScene(new dt::Scene("option_state_scene"));

	dt::ResourceManager::get()->addDataPath(QDir("data"));
	dt::ResourceManager::get()->addResourceLocation("images","FileSystem", true);
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

	auto camnode = scene->addChildNode(new dt::Node("camera_node"));
    camnode->setPosition(Ogre::Vector3(0, 5, 10));
    camnode->addComponent(new dt::CameraComponent("cam"))->lookAt(Ogre::Vector3(0, 0, 0));

	// GUI
	
}

void OptionState::onClick(MyGUI::Widget* sender) {
}

void OptionState::onScrollChangePosition(MyGUI::ScrollBar* sender, size_t position) {
}