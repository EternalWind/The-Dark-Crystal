#include "BattleState.h"
#include "Alien.h"
#include "HumanAgent.h"
#include "Car.h"
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
    auto scene = addScene(SceneLoader::loadScene(mStage + ".scene"));
    scene->addComponent<dt::ScriptComponent>(new dt::ScriptComponent(mStage + ".js", mStage, true));

    auto root_win = dt::GuiManager()
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

int main(int argc, char** argv) {
	//std::cout << "hello" << std::endl;
	//system("pause");
	dt::Game game;
	game.run(new BattleState(), argc, argv);
}
