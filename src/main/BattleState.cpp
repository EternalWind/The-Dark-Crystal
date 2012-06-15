#include "BattleState.h"
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

int BattleState::mStageIndex = 0;

void BattleState::onInitialize() {
	auto scene = addScene(new dt::Scene("battle_state_scene"));
	OgreProcedural::Root::getInstance()->sceneManager = scene->getSceneManager();

	auto light_node = scene->addChildNode(new dt::Node("light_node"));
    light_node->setPosition(Ogre::Vector3(-2000, 2000, 1000));
    light_node->addComponent(new dt::LightComponent("light"));

	auto camnode = scene->addChildNode(new dt::Node("camnode"));
    camnode->setPosition(Ogre::Vector3(0, 5, 10));
    camnode->addComponent(new dt::CameraComponent("cam"))->lookAt(Ogre::Vector3(0, 0, 0));;

	auto test_object = scene->addChildNode(new dt::Node("test_object"));
	test_object->setPosition(Ogre::Vector3(0, 0, -5));
	test_object->addComponent(new dt::MeshComponent("Sinbad.mesh", "", "test_mesh"))->setCastShadows(true);

	dt::GuiRootWindow& window = dt::GuiManager::get()->getRootWindow();

	auto button1 = window.addChildWidget(new dt::GuiButton("b1"));
    button1->setCaption("Health");
    button1->setPosition(10, 600);
    button1->setSize(200, 30);
    //button1->getMyGUIWidget()->eventMouseButtonClick += MyGUI::newDelegate(this, &BattleState::onclick);


}

void BattleState::updateStateFrame(double simulation_frame_time) {
	static double runTime = 0;
	runTime += simulation_frame_time;
	if (runTime > 4) {
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