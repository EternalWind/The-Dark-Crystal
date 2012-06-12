#include "MenuState.h"
#include "OptionState.h"

#include <Core/Root.hpp>
#include <Scene/StateManager.hpp>
#include <Core/ResourceManager.hpp>
#include <Graphics/CameraComponent.hpp>
#include <Gui/GuiManager.hpp>

void MenuState::onInitialize() {
	auto scene = addScene(new dt::Scene("menu_state_scene"));

	dt::ResourceManager::get()->addDataPath(QDir("data"));
	dt::ResourceManager::get()->addResourceLocation("images","FileSystem", true);
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

	auto camnode = scene->addChildNode(new dt::Node("camera_node"));
    camnode->setPosition(Ogre::Vector3(0, 5, 10));
    camnode->addComponent(new dt::CameraComponent("cam"))->lookAt(Ogre::Vector3(0, 0, 0));

	// GUI
    dt::GuiRootWindow& win = dt::GuiManager::get()->getRootWindow();

	auto background_imagebox = win.addChildWidget(new dt::GuiImageBox("background_imagebox"));
	background_imagebox->setPosition(410,70);
	background_imagebox->setSize(200,200);
	background_imagebox->setImageTexture("MyGUI_BlueWhiteSkins.png");

	auto logo = win.addChildWidget(new dt::GuiImageBox("logo"));
	logo->setPosition(410,70);
	logo->setSize(200,200);
	logo->setImageTexture("MyGUI_BlueWhiteSkins.png");

	auto single_player_button = win.addChildWidget(new dt::GuiButton("single_player_button"));
    single_player_button->setCaption("SinglePlayer");
    single_player_button->setPosition(10, 10);
    single_player_button->setSize(200, 30);
	single_player_button->getMyGUIWidget()->eventMouseButtonClick += MyGUI::newDelegate(this, &MenuState::onClick);

	auto multi_player_button = win.addChildWidget(new dt::GuiButton("multi_player_button"));
    multi_player_button->setCaption("MultiPlayer");
    multi_player_button->setPosition(10, 50);
    multi_player_button->setSize(200, 30);
	multi_player_button->getMyGUIWidget()->eventMouseButtonClick += MyGUI::newDelegate(this, &MenuState::onClick);

	auto settings_button = win.addChildWidget(new dt::GuiButton("settings_button"));
    settings_button->setCaption("Settings");
    settings_button->setPosition(10, 90);
    settings_button->setSize(200, 30);
	settings_button->getMyGUIWidget()->eventMouseButtonClick += MyGUI::newDelegate(this, &MenuState::onClick);

	auto new_game_button = win.addChildWidget(new dt::GuiButton("new_game_button"));
    new_game_button->setCaption("NewGame");
    new_game_button->setPosition(250, 10);
	new_game_button->setSize(100, 30);
	new_game_button->getMyGUIWidget()->eventMouseButtonClick += MyGUI::newDelegate(this, &MenuState::onClick);

	auto load_record_button = win.addChildWidget(new dt::GuiButton("load_record_button"));
    load_record_button->setCaption("LoadRecord");
    load_record_button->setPosition(380, 10);
    load_record_button->setSize(100, 30);
	load_record_button->getMyGUIWidget()->eventMouseButtonClick += MyGUI::newDelegate(this, &MenuState::onClick);

	auto exit_button = win.addChildWidget(new dt::GuiButton("exit_button"));
    exit_button->setCaption("Exit");
    exit_button->setPosition(10, 130);
    exit_button->setSize(200, 30);
	exit_button->getMyGUIWidget()->eventMouseButtonClick += MyGUI::newDelegate(this, &MenuState::onClick);
}

void MenuState::onClick(MyGUI::Widget* sender) {
	if(sender->getName() == "Gui.single_player_button") {
        //
    } else if(sender->getName() == "Gui.multi_player_button") {
        //
	} else if(sender->getName() == "Gui.settings_button") {
        dt::StateManager::get()->pop();
        dt::StateManager::get()->setNewState(new OptionState());
	} else if(sender->getName() == "Gui.new_game_button") {
        //
	} else if(sender->getName() == "Gui.load_record_button") {
        //
	} else if(sender->getName() == "Gui.exit_button") {
        dt::StateManager::get()->pop();
	}
}

void MenuState::updateStateFrame(double simulation_frame_time) {
}