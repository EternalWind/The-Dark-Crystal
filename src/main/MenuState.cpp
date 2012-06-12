#include "MenuState.h"

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

	mSinglePlayerButton = win.addChildWidget(new dt::GuiButton("single_player_button"));
    mSinglePlayerButton->setCaption("SinglePlayer");
    mSinglePlayerButton->setPosition(10, 10);
    mSinglePlayerButton->setSize(200, 30);
	mSinglePlayerButton->getMyGUIWidget()->eventMouseButtonClick += MyGUI::newDelegate(this, &MenuState::onClick);

	mMultiPlayerButton = win.addChildWidget(new dt::GuiButton("multi_player_button"));
    mMultiPlayerButton->setCaption("MultiPlayer");
    mMultiPlayerButton->setPosition(10, 50);
    mMultiPlayerButton->setSize(200, 30);
	mMultiPlayerButton->getMyGUIWidget()->eventMouseButtonClick += MyGUI::newDelegate(this, &MenuState::onClick);

	mSettingsButton = win.addChildWidget(new dt::GuiButton("settings_button"));
    mSettingsButton->setCaption("Settings");
    mSettingsButton->setPosition(10, 90);
    mSettingsButton->setSize(200, 30);
	mSettingsButton->getMyGUIWidget()->eventMouseButtonClick += MyGUI::newDelegate(this, &MenuState::onClick);

	mNewGameButton = win.addChildWidget(new dt::GuiButton("new_game_button"));
    mNewGameButton->setCaption("NewGame");
    mNewGameButton->setPosition(250, 10);
	mNewGameButton->setSize(100, 30);
	mNewGameButton->getMyGUIWidget()->eventMouseButtonClick += MyGUI::newDelegate(this, &MenuState::onClick);

	mLoadRecordButton = win.addChildWidget(new dt::GuiButton("load_record_button"));
    mLoadRecordButton->setCaption("LoadRecord");
    mLoadRecordButton->setPosition(380, 10);
    mLoadRecordButton->setSize(100, 30);
	mLoadRecordButton->getMyGUIWidget()->eventMouseButtonClick += MyGUI::newDelegate(this, &MenuState::onClick);

	mExitButton = win.addChildWidget(new dt::GuiButton("exit_button"));
    mExitButton->setCaption("Exit");
    mExitButton->setPosition(10, 130);
    mExitButton->setSize(200, 30);
	mExitButton->getMyGUIWidget()->eventMouseButtonClick += MyGUI::newDelegate(this, &MenuState::onClick);
}

void MenuState::onClick(MyGUI::Widget* sender) {
	if(sender->getName() == "Gui.single_player_button") {
    } else if(sender->getName() == "Gui.multi_player_button") {
	} else if(sender->getName() == "Gui.settings_button") {
		dt::StateManager::get()->pop();
		//dt::StateManager::get()->setNewState(new FirstState());
	} else if(sender->getName() == "Gui.new_game_button") {
	} else if(sender->getName() == "Gui.load_record_button") {
	} else if(sender->getName() == "Gui.exit_button") {
	}
}