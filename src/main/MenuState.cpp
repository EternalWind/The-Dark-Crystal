#include "MenuState.h"
#include "OptionState.h"
#include "CreaditState.h"

#include <Core/Root.hpp>
#include <Scene/StateManager.hpp>
#include <Core/ResourceManager.hpp>
#include <Graphics/CameraComponent.hpp>
#include <Graphics/DisplayManager.hpp>
#include <Gui/GuiManager.hpp>

void MenuState::onInitialize() {
    auto scene = addScene(new dt::Scene("menu_state_scene"));
//	dt::DisplayManager::get()->setFullscreen(true);

    dt::ResourceManager::get()->addDataPath(QDir("data"));
    dt::ResourceManager::get()->addResourceLocation("gui", "FileSystem");
    dt::ResourceManager::get()->addResourceLocation("images","FileSystem", true);
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    auto camnode = scene->addChildNode(new dt::Node("camera_node"));
    camnode->setPosition(Ogre::Vector3(0, 5, 10));
    camnode->addComponent(new dt::CameraComponent("cam"))->lookAt(Ogre::Vector3(0, 0, 0));

    // GUI
    dt::GuiRootWindow& win = dt::GuiManager::get()->getRootWindow();

    auto background_imagebox = win.addChildWidget(new dt::GuiImageBox("background_imagebox"));
    background_imagebox->setPosition(0,0);
    background_imagebox->setSize(1.0f,1.0f);
    background_imagebox->setImageTexture("Space.png");

    mLogo = win.addChildWidget(new dt::GuiImageBox("logo")).get();
    mLogo->setImageTexture("logo.png");

    mNewGameButton = win.addChildWidget(new dt::GuiButton("new_game_button")).get();
    mNewGameButton->setCaption(QString::fromLocal8Bit("开启新旅程"));
    mNewGameButton->getMyGUIWidget()->eventMouseButtonClick += MyGUI::newDelegate(this, &MenuState::onClick);

    mLoadRecordButton = win.addChildWidget(new dt::GuiButton("load_record_button")).get();
    mLoadRecordButton->setCaption(QString::fromLocal8Bit("继续征程"));
    mLoadRecordButton->getMyGUIWidget()->eventMouseButtonClick += MyGUI::newDelegate(this, &MenuState::onClick);

    mMultiPlayerButton = win.addChildWidget(new dt::GuiButton("multi_player_button")).get();
    mMultiPlayerButton->setCaption(QString::fromLocal8Bit("多人游戏"));
    mMultiPlayerButton->getMyGUIWidget()->eventMouseButtonClick += MyGUI::newDelegate(this, &MenuState::onClick);

    mSettingButton = win.addChildWidget(new dt::GuiButton("settings_button")).get();
    mSettingButton->setCaption(QString::fromLocal8Bit("设置"));
    mSettingButton->getMyGUIWidget()->eventMouseButtonClick += MyGUI::newDelegate(this, &MenuState::onClick);

    mCreditButton = win.addChildWidget(new dt::GuiButton("credit_button")).get();
    mCreditButton->setCaption(QString::fromLocal8Bit("制作团队"));
    mCreditButton->getMyGUIWidget()->eventMouseButtonClick += MyGUI::newDelegate(this, &MenuState::onClick);

    mExitButton = win.addChildWidget(new dt::GuiButton("exit_button")).get();
    mExitButton->setCaption(QString::fromLocal8Bit("退出游戏"));
    mExitButton->getMyGUIWidget()->eventMouseButtonClick += MyGUI::newDelegate(this, &MenuState::onClick);

    __resetGui();
}

void MenuState::onClick(MyGUI::Widget* sender) {
    if (sender->getName() == "Gui.multi_player_button") {
        //
    } else if (sender->getName() == "Gui.settings_button") {
        dt::StateManager::get()->pop();
        dt::StateManager::get()->setNewState(new OptionState());
    } else if (sender->getName() == "Gui.new_game_button") {
        dt::StateManager::get()->pop();
        //dt::StateManager::get()->setNewState(new MenuState());
    } else if (sender->getName() == "Gui.load_record_button") {
        //
    } else if (sender->getName() == "Gui.credit_button") {
        dt::StateManager::get()->pop();
        dt::StateManager::get()->setNewState(new CreaditState());
    } else if (sender->getName() == "Gui.exit_button") {
        dt::StateManager::get()->pop();
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
    int position_h = (float)coordination.width * 0.7f;  //按钮position
    int position_v = (float)coordination.height *0.6f;

    mLogo->setPosition(coordination.width / 15, coordination.height / 15);
    mLogo->setSize(0.5f, 0.2f);

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