#include "BattleState.h"
#include "Alien.h"
#include "HumanAgent.h"
#include "Car.h"
#include "Spaceship.h"
#include "MenuState.h"
#include "SceneLoader.h"
#include <iostream>

#include <Graphics/CameraComponent.hpp>
#include <Graphics/LightComponent.hpp>
#include <Graphics/MeshComponent.hpp>
#include <Physics/PhysicsBodyComponent.hpp>
#include <Core/ResourceManager.hpp>
#include <OgreProcedural.h>
#include <Scene/Game.hpp>
#include <Gui/GuiRootWindow.hpp>
#include <Gui/GuiManager.hpp>
#include <Scene/StateManager.hpp>
#include <Logic/ScriptComponent.hpp>

#include <OgreProcedural.h>

BattleState::BattleState(const QString stage_name) 
    : mQuestionLabel(nullptr),
      mDialogLabel(nullptr),
      mTotalEnemyNum(0),
      mRemainEnemyNum(0),
      mTotalCrystalNum(0),
      mObtainedCrystalNum(0),
      mStage(stage_name),
      mNextStage("") {}

void BattleState::onInitialize() {
    dt::ResourceManager::get()->addDataPath(QDir("data"));
	dt::ResourceManager::get()->addResourceLocation("models", "FileSystem");
	dt::ResourceManager::get()->addResourceLocation("gui", "FileSystem");
	dt::ResourceManager::get()->addResourceLocation("./models/sinbad.zip", "Zip", true);
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    auto scene = addScene(new dt::Scene("battle_state_scene"));
	OgreProcedural::Root::getInstance()->sceneManager = scene->getSceneManager();

	OgreProcedural::PlaneGenerator().setSizeX(100.0f).setSizeY(100.0f).setUTile(10.0).setVTile(10.0).realizeMesh("Plane");
	auto plane_node = scene->addChildNode(new dt::Node("planenode"));
	plane_node->addComponent(new dt::MeshComponent("Plane", "PrimitivesTest/Pebbles", "plane-mesh"));
	plane_node->addComponent(new dt::PhysicsBodyComponent("plane-mesh", "plane-body",
        dt::PhysicsBodyComponent::CONVEX, 0.0f));

    auto lightnode = scene->addChildNode(new dt::Node("lightnode"));
    lightnode->setPosition(Ogre::Vector3(-20, 20, 10));
    lightnode->addComponent(new dt::LightComponent("light"));
	
	auto camnode = scene->addChildNode(new dt::Node("camnode"));
    camnode->setPosition(Ogre::Vector3(0, 0, 15));
    camnode->addComponent(new dt::CameraComponent("cam"))->lookAt(Ogre::Vector3(0, 0, 0));;

	//auto test_object = scene->addChildNode(new dt::Node("test_object"));
	//test_object->setPosition(Ogre::Vector3(0, 5, -5));
	//test_object->addComponent(new dt::MeshComponent("Sinbad.mesh", "", "test_mesh"))->setCastShadows(true);

	dt::GuiRootWindow& window = dt::GuiManager::get()->getRootWindow();

	auto button1 = window.addChildWidget(new dt::GuiButton("b1"));
    button1->setCaption("Health");
    button1->setPosition(10, 600);
    button1->setSize(200, 30);


	auto alien = new Alien("alien_node",
								"Sinbad.mesh",
								dt::PhysicsBodyComponent::CYLINDER,
								2.0f,
								"walk.wav",
								"walk.wav",
								"walk.wav");
	alien->setPosition(Ogre::Vector3(0, 5, 5));
	alien->setEyePosition(Ogre::Vector3(0, 6, 5));
	scene->addChildNode(alien);

	//auto space_ship = new Spaceship("spceship_node",
	//							"Sinbad.mesh",
 //                               dt::PhysicsBodyComponent::BOX,
	//							5.0f,
 //                               25,
 //                               5.0f,
 //                               1.0f,
 //                               "walk.wav",
	//							"walk.wav",
	//							"walk.wav",
	//							"walk.wav",
 //                               5.0f,
 //                               -1.0f,
 //                               2.0f);
 //   space_ship->setPosition(Ogre::Vector3(0, 12, 5));
 //   space_ship->setEyePosition(Ogre::Vector3(0, 6, 5));
 //   scene->addChildNode(space_ship);

	auto agent = new HumanAgent("human");
	scene->addChildNode(agent);

	agent->attachTo(alien);
	//camnode = scene->addChildNode(new dt::Node("camnode2"));
	//camnode->setPosition(Ogre::Vector3(0, 5, 15));
	//camnode->addComponent(new dt::CameraComponent("cam2"))->lookAt(Ogre::Vector3(0, 0, 0));;

}

void BattleState::updateStateFrame(double simulation_frame_time) {}

BattleState::BattleState(uint16_t tot_enemy_num, uint16_t tot_crystal_num):
		mQuestionLabel(nullptr),
		mDialogLabel(nullptr),
		mTotalEnemyNum(tot_enemy_num),
		mRemainEnemyNum(tot_enemy_num),
		mTotalCrystalNum(tot_crystal_num),
		mObtainedCrystalNum(0) {
}

//bool BattleState::isVictory() {
//	return false;
//}

void BattleState::win() {
    auto state_mgr = dt::StateManager::get();
    state_mgr->pop(1);

    if (mNextStage != "") {
        state_mgr->setNewState(new BattleState(mNextStage));
    } else {
        state_mgr->setNewState(new MenuState());
    }
}

QString BattleState::getBattleStateName() const {
	return QString("BattleState");
}

dt::GuiLabel* BattleState::getDialogLabel() {
	return mDialogLabel;
}

void BattleState::setDialogLabel(dt::GuiLabel* dialog_label) {
	if (dialog_label) {
		mDialogLabel = dialog_label;
	}
}

uint16_t BattleState::getTotalEnemyNum() const {
	return mTotalEnemyNum;
}

void BattleState::setTotalEnemyNum(uint16_t total_enemy_num) {
	mTotalEnemyNum = total_enemy_num;
}

uint16_t BattleState::getRemainEnemyNum() const {
	return mRemainEnemyNum;
}

void BattleState::setRemainEnemyNum(uint16_t remain_enemy_num) {
	mRemainEnemyNum = remain_enemy_num;
}

uint16_t BattleState::getTotalCrystalNum() const {
	return mTotalCrystalNum;
}

void BattleState::setTotalCrystalNum(uint16_t total_crystal_num) {
	mTotalCrystalNum = total_crystal_num;
}

uint16_t BattleState::getObtainedCrystalNum() const {
	return mObtainedCrystalNum;
}

void BattleState::setObtainedCrystalNum(uint16_t obtained_crystal_num) {
	mObtainedCrystalNum = obtained_crystal_num;
}

dt::GuiLabel* BattleState::getQuestionLabel() {
	return mQuestionLabel;
}

void BattleState::setQuestionLabel(dt::GuiLabel* label) {
	if (label) {
		mQuestionLabel = label;
	}
}

// Slots

void BattleState::__onTriggerText(uint16_t text_id) {
	mQuestionLabel->show();
}

void BattleState::__onHealthChanged(uint16_t pre_health, uint16_t cur_health) {

}

void BattleState::__onAmmoChanged(uint16_t pre_ammo, uint16_t cur_ammo) {

}

void BattleState::__onClipNumChanged(uint16_t pre_num, uint16_t cur_num) {

}

void BattleState::__onGetCrystal() {

}

void BattleState::__onTriggerQA() {
	getQuestionLabel()->show();
}

void BattleState::__onAnswerButtonClick(std::shared_ptr<MyGUI::Widget> sender) {

}

QString BattleState::getNextStage() const {
    return mNextStage;
}

void BattleState::setNextStage(const QString next_stage) {
    mNextStage = next_stage;
}
