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
    int position_v_func = (float)coordination.height *0.3f;
    int position_h_key = (float)coordination.width * 0.1f;  //key position
    int position_v_key = (float)coordination.height *0.6f;

    //ͼƬ========================================================================
    auto logo = win.addChildWidget(new dt::GuiImageBox("option_settings_image_box"));
    logo->setPosition(coordination.width / 15, coordination.height / 13);
    logo->setSize(0.5f, 0.15f);
    logo->setImageTexture("logo.png");

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
    confirm_button->setTextColour(MyGUI::Colour(0.0,0.9,0.9));
    confirm_button->setSize(size_h_large,size_v_medium);
    confirm_button->setPosition(size_h_large * 7.5, size_v_large * 9);
    confirm_button->getMyGUIWidget()->eventMouseButtonClick += MyGUI::newDelegate(this, &OptionState::onClick);

    auto cancel_button = win.addChildWidget(new dt::GuiButton("cancel_button"));
    cancel_button->setCaption(QString::fromLocal8Bit("����"));
    cancel_button->setTextColour(MyGUI::Colour(0.0,0.9,0.9));
    cancel_button->setSize(size_h_large,size_v_medium);
    cancel_button->setPosition(size_h_large * 8.6, size_v_large * 9);
    cancel_button->getMyGUIWidget()->eventMouseButtonClick += MyGUI::newDelegate(this, &OptionState::onClick);

    //���ư�������======================================================================================
    addNewFuncButton("forward_button", QString::fromLocal8Bit("ǰ��"), 0, 0);
    addNewFuncButton("backward_button", QString::fromLocal8Bit("����"), 0, 1);
    addNewFuncButton("leftward_button", QString::fromLocal8Bit("��ת"), 0, 2);
    addNewFuncButton("rightward_button", QString::fromLocal8Bit("��ת"), 0, 3);
    addNewFuncButton("jump_button", QString::fromLocal8Bit("����"), 0, 4);
    addNewFuncButton("sprint_button", QString::fromLocal8Bit("�ܲ�"), 0, 5);
    addNewFuncButton("reload_button", QString::fromLocal8Bit("װ��"), 0, 6);
    addNewFuncButton("arm1_button", QString::fromLocal8Bit("����1"), 1, 0);
    addNewFuncButton("arm2_button", QString::fromLocal8Bit("����2"), 1, 1);
    addNewFuncButton("arm3_button", QString::fromLocal8Bit("����3"), 1, 2);
    addNewFuncButton("attack_button", QString::fromLocal8Bit("����"), 1, 3);
    addNewFuncButton("throw_button", QString::fromLocal8Bit("ը��"), 1, 4);
    addNewFuncButton("getoff_button", QString::fromLocal8Bit("�³�"), 1, 5);
    addNewFuncButton("activate_button", QString::fromLocal8Bit("ʰȡ"), 1, 6);

}

void OptionState::addNewFuncButton(const QString name, const QString font_text, int x, int y) {
    dt::GuiRootWindow& win = dt::GuiManager::get()->getRootWindow();
    auto new_button = win.addChildWidget(new dt::GuiButton(name));
    new_button->setCaption(font_text);
    new_button->setTextColour(MyGUI::Colour(0.0,0.9,0.9));
    new_button->getMyGUIWidget()->eventMouseButtonClick += MyGUI::newDelegate(this, &OptionState::onSettingsButtonClick);

    auto coordination = win.getMyGUIWidget()->getAbsoluteCoord();
    int size_h = (float)coordination.width / 15.0f;
    int size_v = (float)coordination.height / 30.0f;
    int position_h = (float)coordination.width * 0.1f;  //key position
    int position_v = (float)coordination.height *0.6f;
    int gap_h = (float)coordination.width / 12.0f;
    int gap_v = (float)coordination.height / 20.0f;

    new_button->setSize(size_h, size_v);
    new_button->setPosition(position_h + gap_h * x, position_v + gap_v * y);
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