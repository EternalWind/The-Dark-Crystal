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

    mDisplaySettingsScrollBar = win.addChildWidget(new dt::GuiScrollBar("DisplaySettingsScrollBar")).get();
    mDisplaySettingsScrollBar->setPosition(410,70);
	mDisplaySettingsScrollBar->setSize(200,15);
    mDisplaySettingsScrollBar->setScrollRange(100);
    mDisplaySettingsScrollBar->setScrollPosition(30);
    dynamic_cast<MyGUI::ScrollBar*>(mDisplaySettingsScrollBar->getMyGUIWidget())->eventScrollChangePosition += MyGUI::newDelegate(this, &OptionState::onScrollChangePosition);

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

    auto credit_button = win.addChildWidget(new dt::GuiButton("credit_button"));
    credit_button->setCaption("credit");
    credit_button->setPosition(380, 10);
    credit_button->setSize(100, 30);
	credit_button->getMyGUIWidget()->eventMouseButtonClick += MyGUI::newDelegate(this, &OptionState::onClick);
}

void OptionState::onClick(MyGUI::Widget* sender) {
    if(sender->getName() == "Gui.confirm_button") {
        //
    } else if(sender->getName() == "Gui.cancel_button") {
        //
	} else if(sender->getName() == "Gui.credit_button") {
        //
	}
}

void OptionState::onScrollChangePosition(MyGUI::ScrollBar* sender, size_t position) {
    if(sender->getName() == "Gui.DisplaySettingsScrollBar") {
        mDisplaySettingsLabel->setCaption(dt::Utils::toString(position));
    } else if(sender->getName() == "Gui.SoundVolumeLabel") {
        mSoundVolumeLabel->setCaption(dt::Utils::toString(position));
	} else if(sender->getName() == "Gui.MusicVolumeLabel") {
        mMusicVolumeLabel->setCaption(dt::Utils::toString(position));
	}
}

void OptionState::onKeyDown(dt::InputManager::InputCode code, OIS::EventArg& event) {
}