#include "OptionState.h"
#include "MenuState.h"
#include "ConfigurationManager.h"
#include <iostream>

#include <Core/Root.hpp>
#include <Scene/StateManager.hpp>
#include <Core/ResourceManager.hpp>
#include <Graphics/CameraComponent.hpp>
#include <Graphics/DisplayManager.hpp>
#include <Gui/GuiManager.hpp>

#include <SFML/Audio/Listener.hpp>

OptionState::OptionState():
mActionButton(nullptr) {
}

void OptionState::onInitialize() {
    auto scene = addScene(new dt::Scene("option_state_scene"));

    auto camnode = scene->addChildNode(new dt::Node("camera_node"));
    camnode->setPosition(Ogre::Vector3(0, 5, 10));
    camnode->addComponent(new dt::CameraComponent("cam"))->lookAt(Ogre::Vector3(0, 0, 0));

    QObject::connect(dt::InputManager::get(), SIGNAL(sPressed(dt::InputManager::InputCode, const OIS::EventArg&)),
                          this, SLOT(onKeyDown(dt::InputManager::InputCode, const OIS::EventArg&)));

    // ConfigurationManager
    ConfigurationManager* config_mgr = ConfigurationManager::getInstance();
    config_mgr->loadConfig();

    mControlSettings = config_mgr->getControlSetting();
    mScreenSettings = config_mgr->getScreenSetting();
    mSoundSettings = config_mgr->getSoundSetting();
    mQASettings = config_mgr->getQASetting();

	//music
    SoundSetting sound_setting = config_mgr->getSoundSetting();
	auto bg_menu = camnode->addComponent<dt::SoundComponent>(new dt::SoundComponent("musics/bg_menu.wav", "bg_optionstate"));
	bg_menu->setVolume((float)sound_setting.getMusic());
	bg_menu->getSound().setLoop(true);
	bg_menu->playSound();

	mButtonClickSound = camnode->addComponent<dt::SoundComponent>(new dt::SoundComponent("musics/bg_mouse_click.wav", "optionstate_button_sound")).get();
	mButtonClickSound->setVolume((float)sound_setting.getSoundEffect());
	mButtonClickSound->getSound().setLoop(false);

    // GUI
    dt::GuiRootWindow& win = dt::GuiManager::get()->getRootWindow();

    //先加入背景图片控件，防止getMyGUIWidget()->getAbsoluteCoord()出错
    auto background_imagebox = win.addChildWidget(new dt::GuiImageBox("background_imagebox"));
    background_imagebox->setPosition(0,0);
    background_imagebox->setSize(1.0f,1.0f);
    background_imagebox->setImageTexture("Space.png");

    auto coordination = win.getMyGUIWidget()->getAbsoluteCoord();
    int gap_h_large = (float)coordination.width / 8.0f;
    int gap_v_large = (float)coordination.height / 15.0f;
    int size_h_large = (float)coordination.width / 10.0f;
    int size_v_large = (float)coordination.height / 10.0f;

    int gap_h_medium = (float)coordination.width / 12.0f;
    int gap_v_medium = (float)coordination.height / 20.0f;
    int size_h_medium = (float)coordination.width / 15.0f;
    int size_v_medium = (float)coordination.height / 15.0f;

    int gap_h_small = (float)coordination.width / 30.0f;
    int gap_v_small = (float)coordination.height / 30.0f;
    int size_h_small = (float)coordination.width / 30.0f;
    int size_v_small = (float)coordination.height / 30.0f;

    int position_h_func = (float)coordination.width * 0.1f;  //功能设置position
    int position_v_func = (float)coordination.height *0.3f;
    int position_h_key = (float)coordination.width * 0.1f;  //key position
    int position_v_key = (float)coordination.height *0.6f;

    //图片========================================================================
    auto logo = win.addChildWidget(new dt::GuiImageBox("option_settings_image_box"));
    logo->setPosition(coordination.width / 15, coordination.height / 13);
    logo->setSize(0.5f, 0.15f);
    logo->setImageTexture("logo.png");

    //功能设置============================================================================
    mQASettingCheckBox = win.addChildWidget(new dt::GuiCheckBox("QASettingCheckBox")).get();
    mQASettingCheckBox->setSize(size_h_large * 2, size_v_large);
    mQASettingCheckBox->setPosition(position_h_func, position_v_func - gap_v_small);
    mQASettingCheckBox->setCaption(QString::fromLocal8Bit("开启问答系统"));
    mQASettingCheckBox->setTextColour(MyGUI::Colour(0.0,0.9,0.9));
    mQASettingCheckBox->setStateSelected(mQASettings.getIsQAEnable());
    mQASettingCheckBox->getMyGUIWidget()->eventMouseButtonClick += MyGUI::newDelegate(this, &OptionState::onClick);

    mDisplaySettingCheckBox = win.addChildWidget(new dt::GuiCheckBox("DisplaySettingsCheckBox")).get();
    mDisplaySettingCheckBox->setSize(size_h_medium, size_v_medium);
    mDisplaySettingCheckBox->setPosition(position_h_func, position_v_func + gap_v_small);
    mDisplaySettingCheckBox->setCaption(QString::fromLocal8Bit("全屏"));
    mDisplaySettingCheckBox->setTextColour(MyGUI::Colour(0.0,0.9,0.9));
    mDisplaySettingCheckBox->setStateSelected(mScreenSettings.getFullScreen());
    mDisplaySettingCheckBox->getMyGUIWidget()->eventMouseButtonClick += MyGUI::newDelegate(this, &OptionState::onClick);

    mSoundVolumeScrollBar = win.addChildWidget(new dt::GuiScrollBar("SoundVolumeScrollBar")).get();
    mSoundVolumeScrollBar->setSize(size_h_large * 2, 15);
    mSoundVolumeScrollBar->setPosition(position_h_func, position_v_func + gap_v_large * 3);
    mSoundVolumeScrollBar->setScrollRange(101);
    mSoundVolumeScrollBar->setScrollPosition(mSoundSettings.getSoundEffect());
    dynamic_cast<MyGUI::ScrollBar*>(mSoundVolumeScrollBar->getMyGUIWidget())->eventScrollChangePosition += MyGUI::newDelegate(this, &OptionState::onScrollChangePosition);

    mMusicVolumeScrollBar = win.addChildWidget(new dt::GuiScrollBar("MusicVolumeScrollBar")).get();
    mMusicVolumeScrollBar->setSize(size_h_large * 2, 15);
    mMusicVolumeScrollBar->setPosition(position_h_func, position_v_func + gap_v_large * 4);
    mMusicVolumeScrollBar->setScrollRange(101);
    mMusicVolumeScrollBar->setScrollPosition(mSoundSettings.getMusic());
    dynamic_cast<MyGUI::ScrollBar*>(mMusicVolumeScrollBar->getMyGUIWidget())->eventScrollChangePosition += MyGUI::newDelegate(this, &OptionState::onScrollChangePosition);

    mMasterVolumeScrollBar = win.addChildWidget(new dt::GuiScrollBar("MasterVolumeScrollBar")).get();
    mMasterVolumeScrollBar->setSize(size_h_large * 2, 15);
    mMasterVolumeScrollBar->setPosition(position_h_func, position_v_func + gap_v_large * 2);
    mMasterVolumeScrollBar->setScrollRange(101);
    mMasterVolumeScrollBar->setScrollPosition(mSoundSettings.getMainVolume());
    dynamic_cast<MyGUI::ScrollBar*>(mMasterVolumeScrollBar->getMyGUIWidget())->eventScrollChangePosition += MyGUI::newDelegate(this, &OptionState::onScrollChangePosition);

    mMouseSensitivityScrollBar = win.addChildWidget(new dt::GuiScrollBar("MouseSensitivityScrollBar")).get();
    mMouseSensitivityScrollBar->setSize(size_h_large * 2, 15);
    mMouseSensitivityScrollBar->setPosition(position_h_func, size_v_large * 9.5);
    mMouseSensitivityScrollBar->setScrollRange(101);
    mMouseSensitivityScrollBar->setScrollPosition((size_t)(mControlSettings.getSensitivity() * 100.0f));
    dynamic_cast<MyGUI::ScrollBar*>(mMouseSensitivityScrollBar->getMyGUIWidget())->eventScrollChangePosition += MyGUI::newDelegate(this, &OptionState::onScrollChangePosition);

    mMasterVolumeLabel = win.addChildWidget(new dt::GuiLabel("MasterVolumeLabel")).get();
    mMasterVolumeLabel->setSize(size_h_medium, size_v_small * 0.8);
    mMasterVolumeLabel->setPosition(position_h_func, position_v_func + gap_v_large * 2 - gap_v_small);
    mMasterVolumeLabel->setCaption(QString::fromLocal8Bit("主音量："));
    mMasterVolumeLabel->setTextColour(MyGUI::Colour(0.0, 0.9, 0.9));

    mSoundVolumeLabel = win.addChildWidget(new dt::GuiLabel("SoundVolumeLabel")).get();
    mSoundVolumeLabel->setSize(size_h_medium, size_v_small * 0.8);
    mSoundVolumeLabel->setPosition(position_h_func, position_v_func + gap_v_large * 3 - gap_v_small);
    mSoundVolumeLabel->setCaption(QString::fromLocal8Bit("音效："));
    mSoundVolumeLabel->setTextColour(MyGUI::Colour(0.0,0.9,0.9));

    mMusicVolumeLabel = win.addChildWidget(new dt::GuiLabel("MusicVolumeLabel")).get();
    mMusicVolumeLabel->setSize(size_h_large, size_v_small * 0.8);
    mMusicVolumeLabel->setPosition(position_h_func, position_v_func + gap_v_large * 4 - gap_v_small);
    mMusicVolumeLabel->setCaption(QString::fromLocal8Bit("背景音乐："));
    mMusicVolumeLabel->setTextColour(MyGUI::Colour(0.0,0.9,0.9));

    mMouseSensitivityLabel = win.addChildWidget(new dt::GuiLabel("MouseSensitivityLabel")).get();
    mMouseSensitivityLabel->setSize(size_h_large, size_v_small * 0.8);
    mMouseSensitivityLabel->setPosition(position_h_func, size_v_large * 9.0);
    mMouseSensitivityLabel->setCaption(QString::fromLocal8Bit("鼠标灵敏度："));
    mMouseSensitivityLabel->setTextColour(MyGUI::Colour(0.0,0.9,0.9));

    mMessageLabel = win.addChildWidget(new dt::GuiLabel("MessageLabel")).get();
    mMessageLabel->setSize(size_h_large * 2, size_v_small * 0.8);
    mMessageLabel->setPosition(0.9f, 0.9f);
    mMessageLabel->setCaption(QString::fromLocal8Bit("你可以设置音视频和控制方式"));

    auto confirm_button = win.addChildWidget(new dt::GuiButton("confirm_button"));
    confirm_button->setCaption(QString::fromLocal8Bit("确定"));
    confirm_button->setTextColour(MyGUI::Colour(0.0,0.9,0.9));
    confirm_button->setSize(size_h_large,size_v_medium);
    confirm_button->setPosition(size_h_large * 7.5, size_v_large * 9);
    confirm_button->getMyGUIWidget()->eventMouseButtonClick += MyGUI::newDelegate(this, &OptionState::onClick);

    auto cancel_button = win.addChildWidget(new dt::GuiButton("cancel_button"));
    cancel_button->setCaption(QString::fromLocal8Bit("返回"));
    cancel_button->setTextColour(MyGUI::Colour(0.0,0.9,0.9));
    cancel_button->setSize(size_h_large,size_v_medium);
    cancel_button->setPosition(size_h_large * 8.6, size_v_large * 9);
    cancel_button->getMyGUIWidget()->eventMouseButtonClick += MyGUI::newDelegate(this, &OptionState::onClick);

    //控制按键设置======================================================================================
    addNewFuncButton("forward_button", QString::fromLocal8Bit("前进"), 0, 0);
    addNewFuncButton("backward_button", QString::fromLocal8Bit("后退"), 0, 1);
    addNewFuncButton("leftward_button", QString::fromLocal8Bit("左转"), 0, 2);
    addNewFuncButton("rightward_button", QString::fromLocal8Bit("右转"), 0, 3);
    addNewFuncButton("jump_button", QString::fromLocal8Bit("跳起"), 0, 4);
    addNewFuncButton("sprint_button", QString::fromLocal8Bit("跑步"), 0, 5);
    addNewFuncButton("reload_button", QString::fromLocal8Bit("装弹"), 0, 6);
    addNewFuncButton("arm1_button", QString::fromLocal8Bit("武器1"), 1, 0);
    addNewFuncButton("arm2_button", QString::fromLocal8Bit("武器2"), 1, 1);
    addNewFuncButton("arm3_button", QString::fromLocal8Bit("武器3"), 1, 2);
    addNewFuncButton("attack_button", QString::fromLocal8Bit("攻击"), 1, 3);
    addNewFuncButton("throw_button", QString::fromLocal8Bit("丢弃"), 1, 4);
    addNewFuncButton("getoff_button", QString::fromLocal8Bit("下车"), 1, 5);
    addNewFuncButton("activate_button", QString::fromLocal8Bit("拾取"), 1, 6);

}

void OptionState::addNewFuncButton(const QString name, const QString font_text, int x, int y) {
    dt::GuiRootWindow& win = dt::GuiManager::get()->getRootWindow();
    auto new_button = win.addChildWidget(new dt::GuiButton(name));
    new_button->setCaption(font_text);
    new_button->setTextColour(MyGUI::Colour(0.0,0.9,0.9));
    new_button->getMyGUIWidget()->eventMouseButtonClick += MyGUI::newDelegate(this, &OptionState::onSettingsButtonClick);

    auto coordination = win.getMyGUIWidget()->getAbsoluteCoord();
    int size_h = (float)coordination.width / 15.0f;
    int size_v = (float)coordination.height / 35.0f;
    int position_h = (float)coordination.width * 0.1f;  //key position
    int position_v = (float)coordination.height * 0.6f;
    int gap_h = (float)coordination.width / 9.0f;
    int gap_v = (float)coordination.height / 25.0f;

    new_button->setSize(size_h, size_v);
    new_button->setPosition(position_h + gap_h * x, position_v + gap_v * y);
}

void OptionState::onClick(MyGUI::Widget* sender) {
	mButtonClickSound->playSound();
    if (sender->getName() == "Gui.confirm_button") {
        ConfigurationManager* cfg = ConfigurationManager::getInstance();

        mSoundSettings.setMainVolume(mMasterVolumeScrollBar->getScrollPosition());
        mSoundSettings.setMusic(mMusicVolumeScrollBar->getScrollPosition());
        mSoundSettings.setSoundEffect(mSoundVolumeScrollBar->getScrollPosition());

        mQASettings.setIsQAEnable(mQASettingCheckBox->getStateSelected());
        
        sf::Listener::setGlobalVolume((float)mMasterVolumeScrollBar->getScrollPosition());
        if (mScreenSettings.getFullScreen() != mDisplaySettingCheckBox->getStateSelected()) {
            mScreenSettings.setFullScreen(mDisplaySettingCheckBox->getStateSelected());
            dt::DisplayManager::get()->setFullscreen(mDisplaySettingCheckBox->getStateSelected());

            if (!mScreenSettings.getFullScreen()) {
                dt::DisplayManager::get()->setWindowSize(640, 400);
            }
        }

        mControlSettings.setSensitivity((float)mMouseSensitivityScrollBar->getScrollPosition() / 100.0f);

        dt::GuiRootWindow& root = dt::GuiManager::get()->getRootWindow();
        auto win = dt::DisplayManager::get()->getRenderWindow();
        root.setSize((int)win->getWidth(), (int)win->getHeight());

        cfg->setControlSetting(mControlSettings);
        cfg->setQASetting(mQASettings);
        cfg->setScreenSetting(mScreenSettings);
        cfg->setSoundSetting(mSoundSettings);

        cfg->saveConfig();

        dt::StateManager::get()->setNewState(new MenuState());
    } else if (sender->getName() == "Gui.cancel_button") {
        dt::StateManager::get()->setNewState(new MenuState());
    } else if (sender->getName() == "Gui.QASettingCheckBox" || sender->getName() == "Gui.DisplaySettingsCheckBox") {
        bool state_value = dynamic_cast<MyGUI::Button*>(sender)->getStateSelected();
        dynamic_cast<MyGUI::Button*>(sender)->setStateSelected(!state_value);
    }
}

void OptionState::onSettingsButtonClick(MyGUI::Widget* sender) {
    //按钮有按下和弹起两种状态
    if(mActionButton != nullptr) return;
    static_cast<MyGUI::Button*>(sender)->setStateSelected(true);
    mActionButton = sender;
    mMessageLabel->setCaption(QString::fromLocal8Bit("请在键盘上按下要设置的按键"));
}

void OptionState::onKeyDown(dt::InputManager::InputCode code, const OIS::EventArg& event) {
    if (mActionButton == nullptr)   return;

    if (mActionButton->getName() == "Gui.forward_button") {
        mActionToChange = ControlSetting::FORWARD;
    } else if (mActionButton->getName() == "Gui.backward_button") {
        mActionToChange = ControlSetting::BACKWARD;
    } else if (mActionButton->getName() == "Gui.leftward_button") {
        mActionToChange = ControlSetting::LEFTWARD;
    } else if (mActionButton->getName() == "Gui.rightward_button") {
        mActionToChange = ControlSetting::RIGHTWARD;
    } else if (mActionButton->getName() == "Gui.jump_button") {
        mActionToChange = ControlSetting::JUMP;
    } else if (mActionButton->getName() == "Gui.sprint_button") {
        mActionToChange = ControlSetting::SPRINT;
    } else if (mActionButton->getName() == "Gui.arm1_button") {
        mActionToChange = ControlSetting::ARM1;
    } else if (mActionButton->getName() == "Gui.arm2_button") {
        mActionToChange = ControlSetting::ARM2;
    } else if (mActionButton->getName() == "Gui.arm3_button") {
        mActionToChange = ControlSetting::ARM3;
    } else if (mActionButton->getName() == "Gui.attack_button") {
        mActionToChange = ControlSetting::ATTACK;
    } else if (mActionButton->getName() == "Gui.reload_button") {
        mActionToChange = ControlSetting::RELOAD;
    }else if (mActionButton->getName() == "Gui.throw_button") {
        mActionToChange = ControlSetting::THROW;
    } else if (mActionButton->getName() == "Gui.getoff_button") {
        mActionToChange = ControlSetting::GET_OFF;
    } else if (mActionButton->getName() == "Gui.activate_button") {
        mActionToChange = ControlSetting::ACTIVATE;
    }

    mControlSettings.setKey(mActionToChange, code);
    dynamic_cast<MyGUI::Button*>(mActionButton)->setStateSelected(false);
    mActionButton = nullptr;
    mMessageLabel->setCaption(QString::fromLocal8Bit("设置成功：") + ConfigurationManager::getInstance()->getControlSetting().getKeyName(mActionToChange));

}

void OptionState::onScrollChangePosition(MyGUI::ScrollBar* sender, size_t position) {
    if (sender->getName() == "Gui.SoundVolumeScrollBar") {
        mSoundVolumeLabel->setCaption(QString::fromLocal8Bit("音效：") + dt::Utils::toString(position));
    } else if (sender->getName() == "Gui.MusicVolumeScrollBar") {
        mMusicVolumeLabel->setCaption(QString::fromLocal8Bit("背景音乐：") + dt::Utils::toString(position));
    }
}

void OptionState::updateStateFrame(double simulation_frame_time) {
}                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           