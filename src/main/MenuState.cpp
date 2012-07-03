#include "MenuState.h"
#include "OptionState.h"
#include "CreaditState.h"
#include "BattleStateTest.h"
#include "BattleState.h"
#include "AnimationState.h"
#include "ConfigurationManager.h"
#include <Core/Root.hpp>
#include <Scene/StateManager.hpp>
#include <Core/ResourceManager.hpp>
#include <Graphics/DisplayManager.hpp>
#include <Graphics/CameraComponent.hpp>
#include <Gui/GuiManager.hpp>
#include <Audio/MusicComponent.hpp>

void MenuState::onInitialize() {
    auto scene = addScene(new dt::Scene("menu_state_scene"));

    auto camnode = scene->addChildNode(new dt::Node("camera_node"));
    camnode->setPosition(Ogre::Vector3(0, 5, 10));
    camnode->addComponent(new dt::CameraComponent("cam"))->lookAt(Ogre::Vector3(0, 0, 0));

	//music
	auto conf_mgr = ConfigurationManager::getInstance() ;
    SoundSetting sound_setting = conf_mgr->getSoundSetting();
	auto bg_menu = camnode->addComponent<dt::SoundComponent>(new dt::SoundComponent("musics/bg_menu.wav", "bg_munu"));
	bg_menu->setVolume((float)sound_setting.getMusic());
	bg_menu->getSound().setLoop(true);
	bg_menu->playSound();

	mButtonClickSound = camnode->addComponent<dt::SoundComponent>(new dt::SoundComponent("musics/bg_mouse_click.wav", "munu_button_sound")).get();
	mButtonClickSound->setVolume((float)sound_setting.getSoundEffect());
	mButtonClickSound->getSound().setLoop(false);

    // GUI
    dt::GuiRootWindow& win = dt::GuiManager::get()->getRootWindow();

    auto background_imagebox = win.addChildWidget(new dt::GuiImageBox("menu_background_imagebox"));
    background_imagebox->setPosition(0,0);
    background_imagebox->setSize(1.0f,1.0f);
    background_imagebox->setImageTexture("Space.png");

    mLogo = win.addChildWidget(new dt::GuiImageBox("menu_logo")).get();
    mLogo->setImageTexture("logo.png");

    mNewGameButton = win.addChildWidget(new GuiImageButton("new_game_button","Button_GameStart_Normal.png",
        "Button_GameStart_Hover.png","Button_GameStart_Pressed.png")).get();
    mNewGameButton->getMyGUIWidget()->eventMouseButtonClick += MyGUI::newDelegate(this, &MenuState::onClick);

    mLoadRecordButton = win.addChildWidget(new GuiImageButton("load_record_button","Button_GameLoad_Normal.png",
        "Button_GameLoad_Hover.png","Button_GameLoad_Pressed.png")).get();
    mLoadRecordButton->getMyGUIWidget()->eventMouseButtonClick += MyGUI::newDelegate(this, &MenuState::onClick);

    mMultiPlayerButton = win.addChildWidget(new GuiImageButton("multi_player_button","Button_GameOnline_Normal.png",
        "Button_GameOnline_Hover.png","Button_GameOnline_Pressed.png")).get();
    mMultiPlayerButton->getMyGUIWidget()->eventMouseButtonClick += MyGUI::newDelegate(this, &MenuState::onClick);

    mSettingButton = win.addChildWidget(new GuiImageButton("settings_button","Button_GameOption_Normal.png",
        "Button_GameOption_Hover.png","Button_GameOption_Pressed.png")).get();
    mSettingButton->getMyGUIWidget()->eventMouseButtonClick += MyGUI::newDelegate(this, &MenuState::onClick);

    mCreditButton = win.addChildWidget(new GuiImageButton("credit_button","Button_GameCredits_Normal.png",
        "Button_GameCredits_Hover.png","Button_GameCredits_Pressed.png")).get();
    mCreditButton->getMyGUIWidget()->eventMouseButtonClick += MyGUI::newDelegate(this, &MenuState::onClick);

    mExitButton = win.addChildWidget(new GuiImageButton("exit_button","Button_GameExit_Normal.png",
        "Button_GameExit_Hover.png","Button_GameExit_Pressed.png")).get();
    mExitButton->getMyGUIWidget()->eventMouseButtonClick += MyGUI::newDelegate(this, &MenuState::onClick);

    __resetGui();
}

void MenuState::onClick(MyGUI::Widget* sender) {
	mButtonClickSound->playSound();
    if (sender->getName() == "Gui.multi_player_button") {
       
    } else if (sender->getName() == "Gui.settings_button") {
        dt::StateManager::get()->setNewState(new OptionState());
    } else if (sender->getName() == "Gui.new_game_button") {
        dt::StateManager::get()->setNewState(new AnimationState("videos/begin.mp4", 35));
    } else if (sender->getName() == "Gui.load_record_button") {
        dt::StateManager::get()->setNewState(new BattleStateTest());
    } else if (sender->getName() == "Gui.credit_button") {
        dt::StateManager::get()->setNewState(new CreaditState());
    } else if (sender->getName() == "Gui.exit_button") {
        exit(0);
    }
}

void MenuState::updateStateFrame(double simulation_frame_time) {
}

void MenuState::__resetGui() {
    dt::GuiRootWindow& root_win = dt::GuiManager::get()->getRootWindow();
    auto coordination = root_win.getMyGUIWidget()->getAbsoluteCoord();

    int gap_h = (float)coordination.width / 15.0f;       //按钮间隔 
    int gap_v = (float)coordination.height / 15.0f;
    int size_h = (float)coordination.width / 7.0f;       //按钮size
    int size_v = (float)coordination.height / 20.0f;
    int position_h = (float)coordination.width * 0.75f;  //按钮position
    int position_v = (float)coordination.height *0.55f;

    mLogo->setPosition(coordination.width / 15, coordination.height / 13);
    mLogo->setSize(0.5f, 0.15f);

    mNewGameButton->setPosition(position_h, position_v);
    mLoadRecordButton->setPosition(position_h, position_v + gap_v);
    mMultiPlayerButton->setPosition(position_h, position_v + gap_v * 2);
    mSettingButton->setPosition(position_h, position_v + gap_v * 3);
    mCreditButton->setPosition(position_h, position_v + gap_v * 4);
    mExitButton->setPosition(position_h, position_v + gap_v * 5);

    mNewGameButton->setSize(size_h, size_v);
    mLoadRecordButton->setSize(size_h, size_v);
    mMultiPlayerButton->setSize(size_h, size_v);
    mSettingButton->setSize(size_h, size_v);
    mCreditButton->setSize(size_h, size_v);
    mExitButton->setSize(size_h, size_v);

} 