#include "SaveLoadState.h"
#include "MenuState.h"

#include <Core/Root.hpp>
#include <Scene/StateManager.hpp>
#include <Core/ResourceManager.hpp>
#include <Graphics/CameraComponent.hpp>
#include <Gui/GuiManager.hpp>

void SaveLoadState::onInitialize() {
    auto scene = addScene(new dt::Scene("saveload_state_scene"));

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

    auto continue_game_imagebox = win.addChildWidget(new dt::GuiImageBox("continue_game_imagebox"));
    continue_game_imagebox->setPosition(410,70);
    continue_game_imagebox->setSize(200,200);
    continue_game_imagebox->setImageTexture("MyGUI_BlueWhiteSkins.png");

    auto screen_shot_imagebox = win.addChildWidget(new dt::GuiImageBox("screen_shot_imagebox"));
    screen_shot_imagebox->setPosition(410,70);
    screen_shot_imagebox->setSize(200,200);
    screen_shot_imagebox->setImageTexture("MyGUI_BlueWhiteSkins.png");

    auto load_button = win.addChildWidget(new dt::GuiButton("load_button"));
    load_button->setCaption(QString::fromStdWString(L"加载游戏"));
    load_button->setPosition(10, 50);
    load_button->setSize(200, 30);
    load_button->getMyGUIWidget()->eventMouseButtonClick += MyGUI::newDelegate(this, &SaveLoadState::onClick);

    auto return_button = win.addChildWidget(new dt::GuiButton("return_button"));
    return_button->setCaption(QString::fromStdWString(L"返回"));
    return_button->setPosition(10, 50);
    return_button->setSize(200, 30);
    return_button->getMyGUIWidget()->eventMouseButtonClick += MyGUI::newDelegate(this, &SaveLoadState::onClick);

    mRecordList = win.addChildWidget(new dt::GuiListBox("record_list")).get();
    mRecordList->setPosition(10, 320);
    mRecordList->setSize(200, 100);
    mRecordList->addItem(QString::fromStdWString(L"存档一"));
    mRecordList->addItem(QString::fromStdWString(L"存档二"));
    dynamic_cast<MyGUI::ListBox*>(mRecordList->getMyGUIWidget())->eventListChangePosition += MyGUI::newDelegate(this, &SaveLoadState::onListClick);
}

void SaveLoadState::onClick(MyGUI::Widget* sender) {
    if (sender->getName() == "Gui.load_button") {
        //
    } else if (sender->getName() == "Gui.return_button") {
        dt::StateManager::get()->pop();
        dt::StateManager::get()->setNewState(new MenuState());
    }
}

void SaveLoadState::onListClick(MyGUI::ListBox* sender, size_t index) {
    if(sender->getName() == "Gui.record_list") {
        //
    }
}

void SaveLoadState::updateStateFrame(double simulation_frame_time) {
}