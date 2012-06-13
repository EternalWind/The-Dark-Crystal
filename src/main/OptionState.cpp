#include "OptionState.h"
#include "MenuState.h"
#include "ConfigurationManager.h"

#include <Core/Root.hpp>
#include <Scene/StateManager.hpp>
#include <Core/ResourceManager.hpp>
#include <Graphics/CameraComponent.hpp>
#include <Gui/GuiManager.hpp>

OptionState::OptionState():
mActionButton(nullptr) {
}

void OptionState::onInitialize() {
    auto scene = addScene(new dt::Scene("option_state_scene"));

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

    auto option_settings_image_box = win.addChildWidget(new dt::GuiImageBox("option_settings_image_box"));
    option_settings_image_box->setPosition(410,70);
    option_settings_image_box->setSize(200,200);
    option_settings_image_box->setImageTexture("MyGUI_BlueWhiteSkins.png");

    auto qa_settings_image_box = win.addChildWidget(new dt::GuiImageBox("qa_settings_image_box"));
    qa_settings_image_box->setPosition(410,70);
    qa_settings_image_box->setSize(200,200);
    qa_settings_image_box->setImageTexture("MyGUI_BlueWhiteSkins.png");

    auto display_settings_image_box = win.addChildWidget(new dt::GuiImageBox("display_settings_image_box"));
    display_settings_image_box->setPosition(410,70);
    display_settings_image_box->setSize(200,200);
    display_settings_image_box->setImageTexture("MyGUI_BlueWhiteSkins.png");

    auto sound_settings_image_box = win.addChildWidget(new dt::GuiImageBox("sound_settings_image_box"));
    sound_settings_image_box->setPosition(410,70);
    sound_settings_image_box->setSize(200,200);
    sound_settings_image_box->setImageTexture("MyGUI_BlueWhiteSkins.png");

    auto control_settings_image_box = win.addChildWidget(new dt::GuiImageBox("control_settings_image_box"));
    control_settings_image_box->setPosition(410,70);
    control_settings_image_box->setSize(200,200);
    control_settings_image_box->setImageTexture("MyGUI_BlueWhiteSkins.png");

    mQASettingCheckBox = win.addChildWidget(new dt::GuiCheckBox("QASettingCheckBox")).get();
    mQASettingCheckBox->setPosition(410,70);
    mQASettingCheckBox->setSize(200,200);
    mQASettingCheckBox->setCaption("openQASystem");
    mQASettingCheckBox->getMyGUIWidget()->eventMouseButtonClick += MyGUI::newDelegate(this, &OptionState::onClick);

    mDisplaySettingsCheckBox = win.addChildWidget(new dt::GuiCheckBox("DisplaySettingsCheckBox")).get();
    mDisplaySettingsCheckBox->setPosition(410,70);
    mDisplaySettingsCheckBox->setSize(30,30);
    mDisplaySettingsCheckBox->setCaption("Full Scream");
    mQASettingCheckBox->getMyGUIWidget()->eventMouseButtonClick += MyGUI::newDelegate(this, &OptionState::onClick);

    mSoundVolumeScrollBar = win.addChildWidget(new dt::GuiScrollBar("SoundVolumeScrollBar")).get();
    mSoundVolumeScrollBar->setPosition(410,70);
    mSoundVolumeScrollBar->setSize(200,15);
    mSoundVolumeScrollBar->setScrollRange(100);
    mSoundVolumeScrollBar->setScrollPosition(30);
    dynamic_cast<MyGUI::ScrollBar*>(mSoundVolumeScrollBar->getMyGUIWidget())->eventScrollChangePosition += MyGUI::newDelegate(this, &OptionState::onScrollChangePosition);

    mMusicVolumeScrollBar = win.addChildWidget(new dt::GuiScrollBar("MusicVolumeScrollBar")).get();
    mMusicVolumeScrollBar->setPosition(410,70);
    mMusicVolumeScrollBar->setSize(200,15);
    mMusicVolumeScrollBar->setScrollRange(100);
    mMusicVolumeScrollBar->setScrollPosition(30);
    dynamic_cast<MyGUI::ScrollBar*>(mMusicVolumeScrollBar->getMyGUIWidget())->eventScrollChangePosition += MyGUI::newDelegate(this, &OptionState::onScrollChangePosition);

    mDisplaySettingsLabel = win.addChildWidget(new dt::GuiLabel("DisplaySettingsLabel")).get();
    mDisplaySettingsLabel->setPosition(410,70);
    mDisplaySettingsLabel->setSize(200,15);
    mDisplaySettingsLabel->setCaption("0");

    mSoundVolumeLabel = win.addChildWidget(new dt::GuiLabel("SoundVolumeLabel")).get();
    mSoundVolumeLabel->setPosition(410,70);
    mSoundVolumeLabel->setSize(200,15);
    mSoundVolumeLabel->setCaption("0");

    mMusicVolumeLabel = win.addChildWidget(new dt::GuiLabel("MusicVolumeLabel")).get();
    mMusicVolumeLabel->setPosition(410,70);
    mMusicVolumeLabel->setSize(200,15);
    mMusicVolumeLabel->setCaption("0");

    mMessageLabel = win.addChildWidget(new dt::GuiLabel("MessageLabel")).get();
    mMessageLabel->setPosition(410,70);
    mMessageLabel->setSize(200,15);
    mMessageLabel->setCaption("function move is changing");

    auto confirm_button = win.addChildWidget(new dt::GuiButton("confirm_button"));
    confirm_button->setCaption("confirm");
    confirm_button->setPosition(380, 10);
    confirm_button->setSize(100, 30);
    confirm_button->getMyGUIWidget()->eventMouseButtonClick += MyGUI::newDelegate(this, &OptionState::onClick);

    auto cancel_button = win.addChildWidget(new dt::GuiButton("cancel_button"));
    cancel_button->setCaption("cancel");
    cancel_button->setPosition(380, 10);
    cancel_button->setSize(100, 30);
    cancel_button->getMyGUIWidget()->eventMouseButtonClick += MyGUI::newDelegate(this, &OptionState::onClick);

    //ControlSetting button
    auto forward_button = win.addChildWidget(new dt::GuiButton("forward_button"));
    forward_button->setCaption("FORWARD");
    forward_button->setPosition(380, 10);
    forward_button->setSize(50, 30);
    forward_button->getMyGUIWidget()->eventMouseButtonClick += MyGUI::newDelegate(this, &OptionState::onSettingsButtonClick);

    auto backward_button = win.addChildWidget(new dt::GuiButton("backward_button"));
    backward_button->setCaption("BACKWARD");
    backward_button->setPosition(380, 10);
    backward_button->setSize(50, 30);
    backward_button->getMyGUIWidget()->eventMouseButtonClick += MyGUI::newDelegate(this, &OptionState::onSettingsButtonClick);

    auto leftward_button = win.addChildWidget(new dt::GuiButton("leftward_button"));
    leftward_button->setCaption("LEFTWARD");
    leftward_button->setPosition(380, 10);
    leftward_button->setSize(50, 30);
    leftward_button->getMyGUIWidget()->eventMouseButtonClick += MyGUI::newDelegate(this, &OptionState::onSettingsButtonClick);

    auto right_button = win.addChildWidget(new dt::GuiButton("right_button"));
    right_button->setCaption("RIGHTWARD");
    right_button->setPosition(380, 10);
    right_button->setSize(50, 30);
    right_button->getMyGUIWidget()->eventMouseButtonClick += MyGUI::newDelegate(this, &OptionState::onSettingsButtonClick);

    auto jump_button = win.addChildWidget(new dt::GuiButton("jump_button"));
    jump_button->setCaption("JUMP");
    jump_button->setPosition(380, 10);
    jump_button->setSize(50, 30);
    jump_button->getMyGUIWidget()->eventMouseButtonClick += MyGUI::newDelegate(this, &OptionState::onSettingsButtonClick);

    auto sprint_button = win.addChildWidget(new dt::GuiButton("sprint_button"));
    sprint_button->setCaption("SPRINT");
    sprint_button->setPosition(380, 10);
    sprint_button->setSize(50, 30);
    sprint_button->getMyGUIWidget()->eventMouseButtonClick += MyGUI::newDelegate(this, &OptionState::onSettingsButtonClick);

    auto arm1_button = win.addChildWidget(new dt::GuiButton("arm1_button"));
    arm1_button->setCaption("ARM1");
    arm1_button->setPosition(380, 10);
    arm1_button->setSize(50, 30);
    arm1_button->getMyGUIWidget()->eventMouseButtonClick += MyGUI::newDelegate(this, &OptionState::onSettingsButtonClick);

    auto arm2_button = win.addChildWidget(new dt::GuiButton("arm2_button"));
    arm2_button->setCaption("ARM2");
    arm2_button->setPosition(380, 10);
    arm2_button->setSize(50, 30);
    arm2_button->getMyGUIWidget()->eventMouseButtonClick += MyGUI::newDelegate(this, &OptionState::onSettingsButtonClick);

    auto arm3_button = win.addChildWidget(new dt::GuiButton("arm3_button"));
    arm3_button->setCaption("ARM3");
    arm3_button->setPosition(380, 10);
    arm3_button->setSize(50, 30);
    arm3_button->getMyGUIWidget()->eventMouseButtonClick += MyGUI::newDelegate(this, &OptionState::onSettingsButtonClick);

    auto attack_button = win.addChildWidget(new dt::GuiButton("attack_button"));
    attack_button->setCaption("ATTACK");
    attack_button->setPosition(380, 10);
    attack_button->setSize(50, 30);
    attack_button->getMyGUIWidget()->eventMouseButtonClick += MyGUI::newDelegate(this, &OptionState::onSettingsButtonClick);

    auto reload_button = win.addChildWidget(new dt::GuiButton("reload_button"));
    reload_button->setCaption("RELOAD");
    reload_button->setPosition(380, 10);
    reload_button->setSize(50, 30);
    reload_button->getMyGUIWidget()->eventMouseButtonClick += MyGUI::newDelegate(this, &OptionState::onSettingsButtonClick);

    auto getoff_button = win.addChildWidget(new dt::GuiButton("getoff_button"));
    getoff_button->setCaption("GET_OFF");
    getoff_button->setPosition(380, 10);
    getoff_button->setSize(50, 30);
    getoff_button->getMyGUIWidget()->eventMouseButtonClick += MyGUI::newDelegate(this, &OptionState::onSettingsButtonClick);

    auto activate_button = win.addChildWidget(new dt::GuiButton("activate_button"));
    activate_button->setCaption("ACTIVATE");
    activate_button->setPosition(380, 10);
    activate_button->setSize(50, 30);
    activate_button->getMyGUIWidget()->eventMouseButtonClick += MyGUI::newDelegate(this, &OptionState::onSettingsButtonClick);


}

void OptionState::onClick(MyGUI::Widget* sender) {
    if (sender->getName() == "Gui.confirm_button") {
        ConfigurationManager* cfg = ConfigurationManager::getInstance();
        cfg->getQASetting().setIsQAEnable(mQASettingCheckBox->getStateSelected());
        cfg->getScreenSetting().setFullScreen(mDisplaySettingsCheckBox->getStateSelected());
        cfg->getSoundSetting().setMusic(mMusicVolumeScrollBar->getScrollPosition());
        cfg->getSoundSetting().setSoundEffect(mSoundVolumeScrollBar->getScrollPosition());
    } else if (sender->getName() == "Gui.cancel_button") {
        dt::StateManager::get()->pop();
        dt::StateManager::get()->setNewState(new MenuState());
    } else if (sender->getName() == "Gui.QASettingCheckBox" || sender->getName() == "Gui.DisplaySettingsCheckBox") {
        bool state_value = dynamic_cast<MyGUI::Button*>(sender)->getStateSelected();
        dynamic_cast<MyGUI::Button*>(sender)->setStateSelected(!state_value);
    }
}

void OptionState::onSettingsButtonClick(MyGUI::Widget* sender) {
    //按钮有按下和弹起两种状态
    bool state_value = static_cast<MyGUI::Button*>(sender)->getStateSelected();
    static_cast<MyGUI::Button*>(sender)->setStateSelected(!state_value);
    if (state_value) {
        mActionButton = sender;
        mMessageLabel->setCaption(QString::fromStdWString(L"请按下要设置的按键"));
    } else {
        mActionButton = nullptr;
    } 
}

void OptionState::onKeyDown(dt::InputManager::InputCode code, OIS::EventArg& event) {
    if (mActionButton == nullptr)   return;

    if (mActionButton->getName() == "Gui.forward_button") {
        mActionToChange = ControlSetting::KeyFunction::FORWARD;
    } else if (mActionButton->getName() == "Gui.backward_button") {
        mActionToChange = ControlSetting::KeyFunction::BACKWARD;
    } else if (mActionButton->getName() == "Gui.leftward_button") {
        mActionToChange = ControlSetting::KeyFunction::LEFTWARD;
    } else if (mActionButton->getName() == "Gui.rightward_button") {
        mActionToChange = ControlSetting::KeyFunction::RIGHTWARD;
    } else if (mActionButton->getName() == "Gui.jump_button") {
        mActionToChange = ControlSetting::KeyFunction::JUMP;
    } else if (mActionButton->getName() == "Gui.sprint_button") {
        mActionToChange = ControlSetting::KeyFunction::SPRINT;
    } else if (mActionButton->getName() == "Gui.arm1_button") {
        mActionToChange = ControlSetting::KeyFunction::ARM1;
    } else if (mActionButton->getName() == "Gui.arm2_button") {
        mActionToChange = ControlSetting::KeyFunction::ARM2;
    } else if (mActionButton->getName() == "Gui.arm3_button") {
        mActionToChange = ControlSetting::KeyFunction::ARM3;
    } else if (mActionButton->getName() == "Gui.attack_button") {
        mActionToChange = ControlSetting::KeyFunction::ATTACK;
    } else if (mActionButton->getName() == "Gui.reload_button") {
        mActionToChange = ControlSetting::KeyFunction::RELOAD;
    } else if (mActionButton->getName() == "Gui.getoff_button") {
        mActionToChange = ControlSetting::KeyFunction::GET_OFF;
    } else if (mActionButton->getName() == "Gui.activate_button") {
        mActionToChange = ControlSetting::KeyFunction::ACTIVATE;
    }

	// >>>>>>>>>> AeronZhou 注释： 没有setKey这个函数
    //ConfigurationManager.getInstance()->getControlSetting().setKey(mActionToChange,code);
    bool state_value = dynamic_cast<MyGUI::Button*>(mActionButton)->getStateSelected();
    dynamic_cast<MyGUI::Button*>(mActionButton)->setStateSelected(!state_value);
    //mMessageLabel->setCaption(QString::fromStdWString(L"设置成功：") + ConfigurationManager.getInstance()->getControlSetting().getKeyName());
}

void OptionState::onScrollChangePosition(MyGUI::ScrollBar* sender, size_t position) {
    if (sender->getName() == "Gui.DisplaySettingsScrollBar") {
        mDisplaySettingsLabel->setCaption(dt::Utils::toString(position));
    } else if (sender->getName() == "Gui.SoundVolumeLabel") {
        mSoundVolumeLabel->setCaption(dt::Utils::toString(position));
    } else if (sender->getName() == "Gui.MusicVolumeLabel") {
        mMusicVolumeLabel->setCaption(dt::Utils::toString(position));
    }
}

void OptionState::updateStateFrame(double simulation_frame_time) {
}