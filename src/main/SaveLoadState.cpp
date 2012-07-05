#include "SaveLoadState.h"
#include "MenuState.h"
#include "RecordManager.h"

#include <Core/Root.hpp>
#include <Scene/StateManager.hpp>
#include <Core/ResourceManager.hpp>
#include <Graphics/CameraComponent.hpp>
#include <Gui/GuiManager.hpp>
#include <iostream>

void SaveLoadState::onInitialize() {
    auto scene = addScene(new dt::Scene("saveload_state_scene"));

    auto camnode = scene->addChildNode(new dt::Node("camera_node"));
    camnode->setPosition(Ogre::Vector3(0, 5, 10));
    camnode->addComponent(new dt::CameraComponent("cam"))->lookAt(Ogre::Vector3(0, 0, 0));

    // GUI
    dt::GuiRootWindow& win = dt::GuiManager::get()->getRootWindow();

    auto background_imagebox = win.addChildWidget(new dt::GuiImageBox("background_imagebox"));
    background_imagebox->setPosition(0,0);
    background_imagebox->setSize(1.0f,1.0f);
    background_imagebox->setImageTexture("Space.png");

	auto coordination = win.getMyGUIWidget()->getAbsoluteCoord();		//∆¡ƒª≥ﬂ¥Á

    auto logo = win.addChildWidget(new dt::GuiImageBox("save_load_logo"));
    logo->setPosition(coordination.width / 15, coordination.height / 13);
    logo->setSize(0.5f, 0.15f);
    logo->setImageTexture("logo.png");

	auto delete_button = win.addChildWidget(new dt::GuiButton("delete_button"));
	delete_button->setCaption(QString::fromLocal8Bit("…æ≥˝¥Êµµ"));
    delete_button->setPosition(int(coordination.width * 0.64), int(coordination.height * 0.88));
    delete_button->setSize(0.1f, 0.08f);
    delete_button->getMyGUIWidget()->eventMouseButtonClick += MyGUI::newDelegate(this, &SaveLoadState::onClick);

    auto load_button = win.addChildWidget(new dt::GuiButton("load_button"));
	load_button->setCaption(QString::fromLocal8Bit("º”‘ÿ”Œœ∑"));
    load_button->setPosition(int(coordination.width * 0.75), int(coordination.height * 0.88));
    load_button->setSize(0.1f, 0.08f);
    load_button->getMyGUIWidget()->eventMouseButtonClick += MyGUI::newDelegate(this, &SaveLoadState::onClick);

    auto return_button = win.addChildWidget(new dt::GuiButton("return_button"));
    return_button->setCaption(QString::fromLocal8Bit("∑µªÿ"));
    return_button->setPosition(int(coordination.width * 0.86), int(coordination.height * 0.88));
    return_button->setSize(0.1f, 0.08f);
    return_button->getMyGUIWidget()->eventMouseButtonClick += MyGUI::newDelegate(this, &SaveLoadState::onClick);

    mRecordList = win.addChildWidget(new dt::GuiListBox("record_list")).get();
    mRecordList->setPosition(int(coordination.width * 0.08), int(coordination.height * 0.5));
    mRecordList->setSize(0.25f, 0.45f);
    dynamic_cast<MyGUI::ListBox*>(mRecordList->getMyGUIWidget())->eventListChangePosition += MyGUI::newDelegate(this, &SaveLoadState::onListClick);

	//∂¡»°¥Êµµ
	auto record_mgr = RecordManager::get();
	std::map<unsigned, RecordManager::RecordInfo> records = record_mgr->getRecords();
	for(auto iter = records.begin(); iter != records.end(); ++iter) {
		mRecordList->addItem(iter->second.mLevelID);
	}
}

void SaveLoadState::onClick(MyGUI::Widget* sender) {
    if (sender->getName() == "Gui.load_button") {
		if(mRecordList->getIndexSelected() > 100)
			return;
		QString level_id = mRecordList->getItemNameAt(mRecordList->getIndexSelected());
		if(level_id != "") {
			dt::StateManager::get()->setNewState(new BattleState(level_id));
		}
    } else if (sender->getName() == "Gui.return_button") {
        dt::StateManager::get()->setNewState(new MenuState());
    } else if (sender->getName() == "Gui.delete_button") {
		if(mRecordList->getIndexSelected() > 100)
			return;
		auto record_mgr = RecordManager::get();
		record_mgr->remove(mRecordList->getIndexSelected());
		mRecordList->removeItemAt(mRecordList->getIndexSelected());
    }
}

void SaveLoadState::onListClick(MyGUI::ListBox* sender, size_t index) {
    if(sender->getName() == "Gui.record_list") {
        //
    }
}

void SaveLoadState::updateStateFrame(double simulation_frame_time) {
}