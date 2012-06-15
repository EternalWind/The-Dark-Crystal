#include "BattleState.h"
#include "Alien.h"
#include "HumanAgent.h"
#include "Car.h"
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

#include <OgreProcedural.h>

int BattleState::mStageIndex = 0;

void BattleState::onInitialize() {

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
	alien->setPosition(Ogre::Vector3(0, 5, -5));
	alien->setEyePosition(Ogre::Vector3(0, 6, -5));
	scene->addChildNode(alien);
/*
	Entity* car = new Car("car",
						"Sinbad.mesh",
						dt::PhysicsBodyComponent::BOX,
						20.0f,
						10,
						20.f,
						1.0f,
						"move.wav",
						"move.wav",
						"move.wav",
						5.0f,
						4.0f,
						10.0f,
						1.0f,
						1.0f);
	
	car->setPosition(Ogre::Vector3(0, 5, 0));
	car->setEyePosition(car->getPosition() + Ogre::Vector3(0, 2, 0));
	scene->addChildNode(car);			*/				

	auto agent = new HumanAgent("human");
	scene->addChildNode(agent);

	agent->attachTo(alien);

	//camnode = scene->addChildNode(new dt::Node("camnode2"));
	//camnode->setPosition(Ogre::Vector3(0, 5, 15));
	//camnode->addComponent(new dt::CameraComponent("cam2"))->lookAt(Ogre::Vector3(0, 0, 0));;
	int a;
}

void BattleState::updateStateFrame(double simulation_frame_time) {
	static double runTime = 0;
	runTime += simulation_frame_time;

	if (runTime > 40) {
		dt::StateManager::get()->pop(1);
	}
}

BattleState::BattleState(uint16_t tot_enemy_num, uint16_t tot_crystal_num):
		mQuestionLabel(nullptr),
		mDialogLabel(nullptr),
		mTotalEnemyNum(tot_enemy_num),
		mRemainEnemyNum(tot_enemy_num),
		mTotalCrystalNum(tot_crystal_num),
		mObtainedCrystalNum(0) {
}

bool BattleState::isVictory() {
	return false;
}

void BattleState::win() {

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

int BattleState::getStageIndex() const {
    return mStageIndex;
}

void BattleState::setStageIndex(const int stage_index) {
    mStageIndex = stage_index;
}

int main(int argc, char** argv) {
	//std::cout << "hello" << std::endl;
	//system("pause");
	dt::Game game;
	game.run(new BattleState(), argc, argv);
}
