#include "OptionState.h"
#include "MenuState.h"
#include "ConfigurationManager.h"
#include <iostream>

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
    dt::ResourceManager::get()->addResourceLocation("gui", "FileSystem");
    dt::ResourceManager::get()->addResourceLocation("images","FileSystem", true);
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    auto camnode = scene->addChildNode(new dt::Node("camera_node"));
    camnode->setPosition(Ogre::Vector3(0, 5, 10));
    camnode->addComponent(new dt::CameraComponent("cam"))->lookAt(Ogre::Vector3(0, 0, 0));

    QObject::connect(dt::InputManager::get(), SIGNAL(sPressed(dt::InputManager::InputCode, const OIS::EventArg&)),
                          this, SLOT(onKeyDown(dt::InputManager::InputCode, const OIS::EventArg&)));

    // GUI
    dt::GuiRootWindow& win = dt::GuiManager::get()->getRootWindow();

    //�ȼ��뱳��ͼƬ�ؼ�����ֹgetMyGUIWidget()->getAbsoluteCoord()����
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

    int position_h_func = (float)coordination.width * 0.1f;  //��������position
    int position_v_func = (float)coordination.height *0.2f;
    int position_h_key = (float)coordination.width * 0.1f;  //key position
    int position_v_key = (float)coordination.height *0.6f;

    //ͼƬ=======================================================================
    auto option_settings_image_box = win.addChildWidget(new dt::GuiImageBox("option_settings_image_box"));
    option_settings_image_box->setPosition(410,70);
    option_settings_image_box->setSize(200,200);
    option_settings_image_box->setImageTexture("logo.png");

    //��������============================================================================
    mQASettingCheckBox = win.addChildWidget(new dt::GuiCheckBox("QASettingCheckBox")).get();
    mQASettingCheckBox->setSize(size_h_large, size_v_large);
    mQASettingCheckBox->setPosition(position_h_func, position_v_func);
    mQASettingCheckBox->setCaption(QString::fromLocal8Bit("�����ʴ�ϵͳ"));
    mQASettingCheckBox->setTextColour(MyGUI::Colour(0.0,0.9,0.9));
    mQASettingCheckBox->getMyGUIWidget()->eventMouseButtonClick += MyGUI::newDelegate(this, &OptionState::onClick);

    mDisplaySettingsCheckBox = win.addChildWidget(new dt::GuiCheckBox("DisplaySettingsCheckBox")).get();
    mDisplaySettingsCheckBox->setSize(size_h_medium, size_v_medium);
    mDisplaySettingsCheckBox->setPosition(position_h_func, position_v_func + gap_v_large);
    mDisplaySettingsCheckBox->setCaption(QString::fromLocal8Bit("ȫ��"));
    mDisplaySettingsCheckBox->setTextColour(MyGUI::Colour(0.0,0.9,0.9));
    mDisplaySettingsCheckBox->getMyGUIWidget()->eventMouseButtonClick += MyGUI::newDelegate(this, &OptionState::onClick);

    mSoundVolumeScrollBar = win.addChildWidget(new dt::GuiScrollBar("SoundVolumeScrollBar")).get();
    mSoundVolumeScrollBar->setSize(size_h_large * 2, 15);
    mSoundVolumeScrollBar->setPosition(position_h_func, position_v_func + gap_v_large * 3);
    mSoundVolumeScrollBar->setScrollRange(101);
    mSoundVolumeScrollBar->setScrollPosition(30);
    dynamic_cast<MyGUI::ScrollBar*>(mSoundVolumeScrollBar->getMyGUIWidget())->eventScrollChangePosition += MyGUI::newDelegate(this, &OptionState::onScrollChangePosition);

    mMusicVolumeScrollBar = win.addChildWidget(new dt::GuiScrollBar("MusicVolumeScrollBar")).get();
    mMusicVolumeScrollBar->setSize(size_h_large * 2, 15);
    mMusicVolumeScrollBar->setPosition(position_h_func, position_v_func + gap_v_large * 4);
    mMusicVolumeScrollBar->setScrollRange(101);
    mMusicVolumeScrollBar->setScrollPosition(30);
    dynamic_cast<MyGUI::ScrollBar*>(mMusicVolumeScrollBar->getMyGUIWidget())->eventScrollChangePosition += MyGUI::newDelegate(this, &OptionState::onScrollChangePosition);

    mSoundVolumeLabel = win.addChildWidget(new dt::GuiLabel("SoundVolumeLabel")).get();
    mSoundVolumeLabel->setSize(size_h_medium, size_v_small * 0.6);
    mSoundVolumeLabel->setPosition(position_h_func, position_v_func + gap_v_large * 3 - gap_v_small);
    mSoundVolumeLabel->setCaption(QString::fromLocal8Bit("��Ч��"));
    mSoundVolumeLabel->setTextColour(MyGUI::Colour(0.0,0.9,0.9));

    mMusicVolumeLabel = win.addChildWidget(new dt::GuiLabel("MusicVolumeLabel")).get();
    mMusicVolumeLabel->setSize(size_h_medium, size_v_small * 0.6);
    mMusicVolumeLabel->setPosition(position_h_func, position_v_func + gap_v_large * 4 - gap_v_small);
    mMusicVolumeLabel->setCaption(QString::fromLocal8Bit("�������֣�"));
    mMusicVolumeLabel->setTextColour(MyGUI::Colour(0.0,0.9,0.9));

    mMessageLabel = win.addChildWidget(new dt::GuiLabel("MessageLabel")).get();
    mMessageLabel->setSize(size_h_large * 2, size_v_small * 0.6);
    mMessageLabel->setPosition(size_h_large,size_v_large * 9.5);
    mMessageLabel->setCaption(QString::fromLocal8Bit("�������������Ƶ�Ϳ��Ʒ�ʽ"));

    auto confirm_button = win.addChildWidget(new dt::GuiButton("confirm_button"));
    confirm_button->setCaption(QString::fromLocal8Bit("ȷ��"));
    confirm_button->setSize(size_h_large,size_v_medium);
    confirm_button->setPosition(size_h_large * 7.5, size_v_large * 9);
    confirm_button->getMyGUIWidget()->eventMouseButtonClick += MyGUI::newDelegate(this, &OptionState::onClick);

    auto cancel_button = win.addChildWidget(new dt::GuiButton("cancel_button"));
    cancel_button->setCaption(QString::fromLocal8Bit("����"));
    cancel_button->setSize(size_h_large,size_v_medium);
    cancel_button->setPosition(size_h_large * 8.6, size_v_large * 9);
    cancel_button->getMyGUIWidget()->eventMouseButtonClick += MyGUI::newDelegate(this, &OptionState::onClick);

    //���ư�������======================================================================================
    auto forward_button = win.addChildWidget(new dt::GuiButton("forward_button"));
    forward_button->setSize(size_h_medium,size_v_small);
    forward_button->setPosition(position_h_key,position_v_key);
    forward_button->setCaption(QString::fromLocal8Bit("ǰ��"));
    forward_button->getMyGUIWidget()->eventMouseButtonClick += MyGUI::newDelegate(this, &OptionState::onSettingsButtonClick);

    auto backward_button = win.addChildWidget(new dt::GuiButton("backward_button"));
    backward_button->setSize(size_h_medium, size_v_small);
    backward_button->setPosition(position_h_key, position_v_key + gap_v_medium);
    backward_button->setCaption(QString::fromLocal8Bit("����"));
    backward_button->getMyGUIWidget()->eventMouseButtonClick += MyGUI::newDelegate(this, &OptionState::onSettingsButtonClick);

    auto leftward_button = win.addChildWidget(new dt::GuiButton("leftward_button"));
    leftward_button->setSize(size_h_medium, size_v_small);
    leftward_button->setPosition(position_h_key, position_v_key + gap_v_medium * 2);
    leftward_button->setCaption(QString::fromLocal8Bit("��ת"));
    leftward_button->getMyGUIWidget()->eventMouseButtonClick += MyGUI::newDelegate(this, &OptionState::onSettingsButtonClick);

    auto right_button = win.addChildWidget(new dt::GuiButton("right_button"));
    right_button->setSize(size_h_medium, size_v_small);
    right_button->setPosition(position_h_key, position_v_key + gap_v_medium * 3);
    right_button->setCaption(QString::fromLocal8Bit("��ת"));
    right_button->getMyGUIWidget()->eventMouseButtonClick += MyGUI::newDelegate(this, &OptionState::onSettingsButtonClick);

    auto jump_button = win.addChildWidget(new dt::GuiButton("jump_button"));
    jump_button->setSize(size_h_medium, size_v_small);
    jump_button->setPosition(position_h_key, position_v_key + gap_v_medium * 4);
    jump_button->setCaption(QString::fromLocal8Bit("����"));
    jump_button->getMyGUIWidget()->eventMouseButtonClick += MyGUI::newDelegate(this, &OptionState::onSettingsButtonClick);

    auto sprint_button = win.addChildWidget(new dt::GuiButton("sprint_button"));
    sprint_button->setSize(size_h_medium, size_v_small);
    sprint_button->setPosition(position_h_key, position_v_key + gap_v_medium * 5);
    sprint_button->setCaption(QString::fromLocal8Bit("�ܲ�"));
    sprint_button->getMyGUIWidget()->eventMouseButtonClick += MyGUI::newDelegate(this, &OptionState::onSettingsButtonClick);

    auto reload_button = win.addChildWidget(new dt::GuiButton("reload_button"));
    reload_button->setSize(size_h_medium, size_v_small);
    reload_button->setPosition(position_h_key, position_v_key + gap_v_medium * 6);
    reload_button->setCaption(QString::fromLocal8Bit("װ��"));
    reload_button->getMyGUIWidget()->eventMouseButtonClick += MyGUI::newDelegate(this, &OptionState::onSettingsButtonClick);

    auto arm1_button = win.addChildWidget(new dt::GuiButton("arm1_button"));
    arm1_button->setSize(size_h_medium, size_v_small);
    arm1_button->setPosition(position_h_key + gap_h_medium, position_v_key);
    arm1_button->setCaption(QString::fromLocal8Bit("����1"));
    arm1_button->getMyGUIWidget()->eventMouseButtonClick += MyGUI::newDelegate(this, &OptionState::onSettingsButtonClick);

    auto arm2_button = win.addChildWidget(new dt::GuiButton("arm2_button"));
    arm2_button->setSize(size_h_medium, size_v_small);
    arm2_button->setPosition(position_h_key + gap_h_medium, position_v_key + gap_v_medium);
    arm2_button->setCaption(QString::fromLocal8Bit("����2"));
    arm2_button->getMyGUIWidget()->eventMouseButtonClick += MyGUI::newDelegate(this, &OptionState::onSettingsButtonClick);

    auto arm3_button = win.addChildWidget(new dt::GuiButton("arm3_button"));
    arm3_button->setSize(size_h_medium, size_v_small);
    arm3_button->setPosition(position_h_key + gap_h_medium, position_v_key + gap_v_medium * 2);
    arm3_button->setCaption(QString::fromLocal8Bit("����3"));
    arm3_button->getMyGUIWidget()->eventMouseButtonClick += MyGUI::newDelegate(this, &OptionState::onSettingsButtonClick);

    auto attack_button = win.addChildWidget(new dt::GuiButton("attack_button"));
    attack_button->setSize(size_h_medium, size_v_small);
    attack_button->setPosition(position_h_key + gap_h_medium, position_v_key + gap_v_medium * 3);
    attack_button->setCaption(QString::fromLocal8Bit("����"));
    attack_button->getMyGUIWidget()->eventMouseButtonClick += MyGUI::newDelegate(this, &OptionState::onSettingsButtonClick);

    auto throw_button = win.addChildWidget(new dt::GuiButton("throw_button"));
    throw_button->setSize(size_h_medium, size_v_small);
    throw_button->setPosition(position_h_key + gap_h_medium, position_v_key + gap_v_medium * 4);
    throw_button->setCaption(QString::fromLocal8Bit("ը��"));
    throw_button->getMyGUIWidget()->eventMouseButtonClick += MyGUI::newDelegate(this, &OptionState::onSettingsButtonClick);

    auto getoff_button = win.addChildWidget(new dt::GuiButton("getoff_button"));
    getoff_button->setSize(size_h_medium, size_v_small);
    getoff_button->setPosition(position_h_key + gap_h_medium, position_v_key + gap_v_medium * 5);
    getoff_button->setCaption(QString::fromLocal8Bit("�³�"));
    getoff_button->getMyGUIWidget()->eventMouseButtonClick += MyGUI::newDelegate(this, &OptionState::onSettingsButtonClick);

    auto activate_button = win.addChildWidget(new dt::GuiButton("activate_button"));
    activate_button->setSize(size_h_medium, size_v_small);
    activate_button->setPosition(position_h_key + gap_h_medium, position_v_key + gap_v_medium * 6);
    activate_button->setCaption(QString::fromLocal8Bit("ʰȡ"));
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
    //��ť�а��º͵�������״̬
    if(mActionButton != nullptr) return;
    static_cast<MyGUI::Button*>(sender)->setStateSelected(true);
    mActionButton = sender;
    mMessageLabel->setCaption(QString::fromLocal8Bit("���ڼ����ϰ���Ҫ���õİ���"));
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

    ConfigurationManager::getInstance()->getControlSetting().setKey(mActionToChange,code);
    dynamic_cast<MyGUI::Button*>(mActionButton)->setStateSelected(false);
    mActionButton = nullptr;
    mMessageLabel->setCaption(QString::fromLocal8Bit("���óɹ���") + ConfigurationManager::getInstance()->getControlSetting().getKeyName(mActionToChange));

}

void OptionState::onScrollChangePosition(MyGUI::ScrollBar* sender, size_t position) {
    if (sender->getName() == "Gui.SoundVolumeScrollBar") {
        mSoundVolumeLabel->setCaption(QString::fromLocal8Bit("��Ч��") + dt::Utils::toString(position));
    } else if (sender->getName() == "Gui.MusicVolumeScrollBar") {
        mMusicVolumeLabel->setCaption(QString::fromLocal8Bit("�������֣�") + dt::Utils::toString(position));
    }
}

void OptionState::updateStateFrame(double simulation_frame_time) {
}