#include "BattleState.h"
#include <iostream>

#include <Graphics/CameraComponent.hpp>
#include <Graphics/LightComponent.hpp>
#include <Graphics/MeshComponent.hpp>
#include <Physics/PhysicsBodyComponent.hpp>
#include <Core/ResourceManager.hpp>
#include <OgreProcedural.h>
#include <Scene/Game.hpp>

void BattleState::onInitialize() {
	dt::ResourceManager::get()->addDataPath(QDir("data"));
	dt::ResourceManager::get()->addResourceLocation("./models/evilfire.zip", "Zip", true);
	//dt::ResourceManager::get()->addResourceLocation("
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

	dt::Scene::SceneSP scene = addScene(new dt::Scene("battle_state_scene"));
	OgreProcedural::Root::getInstance()->sceneManager = scene->getSceneManager();

	auto light_node = scene->addChildNode(new dt::Node("light_node"));
    light_node->setPosition(Ogre::Vector3(-2000, 2000, 1000));
    light_node->addComponent(new dt::LightComponent("light"));

	//auto camera_node = scene->addChildNode(new dt::Node("camera_node"));
	//camera_node->setPosition(Ogre::Vector3(0, 1.75, 5));
	//camera_node->addComponent(new dt::CameraComponent("camera"))->lookAt(Ogre::Vector3(

	auto test_object = scene->addChildNode(new dt::Node("test_object"));
	test_object->setPosition(Ogre::Vector3(0, 1, -5));
	test_object->addComponent(new dt::MeshComponent("evilfire.mesh", "", "test_mesh"))->setCastShadows(true);
	test_object->addComponent(new dt::PhysicsBodyComponent("test_mesh", "ball_body",
							  dt::PhysicsBodyComponent::BOX))->setMass(1.0f);
}

void BattleState::updateStateFrame(double simulation_frame_time) {

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

QString BattleState::getBattleStateName() {
	return QString("BattleState");
}

dt::GuiLabel* BattleState::getDialogLabel() {
	return mDialogLabel;
}

void BattleState::setDialogDabel(dt::GuiLabel* dialog_label) {

}

// Slots

void BattleState::onTriggerText(uint16_t text_id) {

}

void BattleState::onHealthChanged() {

}

void BattleState::onAmmoChanged() {

}

void BattleState::onClipNumChanged() {

}

void BattleState::onGetCrystal() {

}

void BattleState::onTriggerQA() {

}

void BattleState::onAnswerButtonClick(std::shared_ptr<MyGUI::Widget> sender) {

}

int main(int argc, char** argv) {
	//std::cout << "hello" << std::endl;
	//system("pause");
	dt::Game game;
	game.run(new BattleState(), argc, argv);
}