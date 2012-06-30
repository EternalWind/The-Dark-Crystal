#include "BattleStateTest.h"
#include "Alien.h"
#include "HumanAgent.h"
#include "Car.h"
#include "MenuState.h"
#include "SceneLoader.h"
#include <iostream>
#include "AIDivideAreaManager.h"
#include "PlayerAIAgent.h"

#include <Graphics/CameraComponent.hpp>
#include <Graphics/LightComponent.hpp>
#include <Graphics/MeshComponent.hpp>
#include <Physics/PhysicsBodyComponent.hpp>
#include <Core/ResourceManager.hpp>
#include <OgreProcedural.h>
#include <Scene/Game.hpp>
#include <Gui/GuiRootWindow.hpp>
#include <Gui/GuiManager.hpp>
#include <Graphics/DisplayManager.hpp>
#include <Scene/StateManager.hpp>
#include <Logic/ScriptComponent.hpp>

#include <OgreProcedural.h>

BattleStateTest::BattleStateTest(const QString stage_name) 
    : mQuestionLabel(nullptr),
      mDialogLabel(nullptr),
      mTotalEnemyNum(0),
      mRemainEnemyNum(0),
      mTotalCrystalNum(0),
      mObtainedCrystalNum(0),
      mStage(stage_name),
      mNextStage("") {}

void BattleStateTest::onInitialize() {
    dt::ResourceManager::get()->addResourceLocation("gui", "FileSystem");
    dt::ResourceManager::get()->addResourceLocation("gui/digits", "FileSystem");
    dt::ResourceManager::get()->addResourceLocation("models/sinbad.zip", "Zip", true);
    dt::ResourceManager::get()->addResourceLocation("models", "FileSystem");
    dt::ResourceManager::get()->addResourceLocation("models/alien.zip", "Zip", true); 
    dt::ResourceManager::get()->addResourceLocation("models/monster.zip", "Zip", true); 

    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    AIDivideAreaManager::get()->loadMapInfo("map.txt");
    
    auto scene = addScene(new dt::Scene("BattleStateTest"));
    OgreProcedural::Root::getInstance()->sceneManager = scene->getSceneManager();

    OgreProcedural::PlaneGenerator().setSizeX(10000).setSizeY(10000).setUTile(100).setVTile(100).realizeMesh("plane");

    Alien* alien = new Alien("alien", "alien.mesh", dt::PhysicsBodyComponent::BOX, 1.0f, "", "", "");
    alien->setEyePosition(Ogre::Vector3(0, 0, 10));
    //this->getScene(scene->getName())->getPhysicsWorld()->setShowDebug(true);
    scene->getPhysicsWorld()->setGravity(Ogre::Vector3::ZERO);
    
    //auto cam_node = scene->addChildNode(new Node("camnode"));
    //auto cam = cam_node->addComponent(new dt::CameraComponent("cam")); 
    //cam_node->setPosition(0, 500, 0);
    //cam->lookAt(0,0,0);
    scene->addChildNode(alien);
    alien->findComponent<dt::PhysicsBodyComponent>("physics_body")->disable();
    //alien->setPosition(0, 100, 0);    
    alien->findComponent<dt::PhysicsBodyComponent>("physics_body")->enable();

   /* HumanAgent* human_agent = new HumanAgent("Player");
    human_agent->attachTo(alien);*/

    PlayerAIAgent * pa = new PlayerAIAgent("rj");
    pa->attachTo(alien);
    
    Alien* alien1 = new Alien("alien1", "alien.mesh", dt::PhysicsBodyComponent::BOX, 1.0f, "", "", "");
    alien1->setEyePosition(Ogre::Vector3(0, 10, -100));
    scene->addChildNode(alien1);
    HumanAgent* human_agent = new HumanAgent("Player");
    human_agent->attachTo(alien1);
    
    //alien->setPosition(33, 50, -27);
    //alien->setScale(0.05);
    auto motion = alien1->findComponent<dt::PhysicsBodyComponent>("physics_body")->getRigidBody()->getMotionState();

    btTransform trans;
    trans.setIdentity();
    trans.setOrigin(btVector3(300, 300, 0));

    alien1->findComponent<dt::PhysicsBodyComponent>("physics_body")->getRigidBody()->setWorldTransform(trans);
    motion->setWorldTransform(trans);

    alien1->findComponent<dt::PhysicsBodyComponent>("physics_body")->disable();
    /*alien->disable();
    alien->enable();*/
    alien1->findComponent<dt::PhysicsBodyComponent>("physics_body")->enable();

    auto plane_node = scene->addChildNode(new dt::Node("plane_node"));
    auto light_node = scene->addChildNode(new dt::Node("light"));

    light_node->addComponent(new dt::LightComponent("l"));

    plane_node->addComponent(new dt::MeshComponent("plane", "PrimitivesTest/Pebbles", "Plane"));
    plane_node->setPosition(0, -100, 0);
    plane_node->addComponent(new dt::PhysicsBodyComponent("Plane", "plane_body", dt::PhysicsBodyComponent::BOX, 0.0f));

    dt::GuiRootWindow& root_win = dt::GuiManager::get()->getRootWindow();

    auto health_img1 = root_win.addChildWidget<dt::GuiImageBox>(new dt::GuiImageBox("health_one"));
    auto health_img2 = root_win.addChildWidget<dt::GuiImageBox>(new dt::GuiImageBox("health_ten"));
    auto health_img3 = root_win.addChildWidget<dt::GuiImageBox>(new dt::GuiImageBox("health_hundred"));
    auto ammo_img1 = root_win.addChildWidget<dt::GuiImageBox>(new dt::GuiImageBox("ammo_one"));
    auto ammo_img2 = root_win.addChildWidget<dt::GuiImageBox>(new dt::GuiImageBox("ammo_ten"));
    auto ammo_img3 = root_win.addChildWidget<dt::GuiImageBox>(new dt::GuiImageBox("ammo_hundred"));
    auto clip_img1 = root_win.addChildWidget<dt::GuiImageBox>(new dt::GuiImageBox("clip_one"));
    auto clip_img2 = root_win.addChildWidget<dt::GuiImageBox>(new dt::GuiImageBox("clip_ten"));
    auto clip_img3 = root_win.addChildWidget<dt::GuiImageBox>(new dt::GuiImageBox("clip_hundred"));
    auto front_sight = root_win.addChildWidget<dt::GuiImageBox>(new dt::GuiImageBox("front_sight"));
    auto answer1 = root_win.addChildWidget<dt::GuiButton>(new dt::GuiButton("answer1"));
    auto answer2 = root_win.addChildWidget<dt::GuiButton>(new dt::GuiButton("answer2"));
    auto answer3 = root_win.addChildWidget<dt::GuiButton>(new dt::GuiButton("answer3"));
    auto answer4 = root_win.addChildWidget<dt::GuiButton>(new dt::GuiButton("answer4"));
    auto question = root_win.addChildWidget<dt::GuiEditBox>(new dt::GuiEditBox("question"));
    auto dialog = root_win.addChildWidget<dt::GuiLabel>(new dt::GuiLabel("dialog"));

    mHealthHUD.push_back(health_img3.get());
    mHealthHUD.push_back(health_img2.get());
    mHealthHUD.push_back(health_img1.get());
    mAmmoHUD.push_back(ammo_img3.get());
    mAmmoHUD.push_back(ammo_img2.get());
    mAmmoHUD.push_back(ammo_img1.get());
    mFrontSight = front_sight.get();
    mClipNumHUD.push_back(clip_img3.get());
    mClipNumHUD.push_back(clip_img2.get());
    mClipNumHUD.push_back(clip_img1.get());
    mAnswerButtons.push_back(answer1.get());
    mAnswerButtons.push_back(answer2.get());
    mAnswerButtons.push_back(answer3.get());
    mAnswerButtons.push_back(answer4.get());
    mQuestionLabel = question.get();
    mDialogLabel = dialog.get();

    for (uint8_t i = 0 ; i < 4 ; ++i) {
        mAnswerButtons[i]->setVisible(false);
    }

    mQuestionLabel->setVisible(false);

    mFrontSight->setImageTexture("FrontSight.png");

    mDialogLabel->getMyGUIWidget()->setAlign(MyGUI::Align::Left);

    MyGUI::EditBox* edit_box = dynamic_cast<MyGUI::EditBox*>(mQuestionLabel->getMyGUIWidget());

    edit_box->setEditMultiLine(true);
    edit_box->setEditWordWrap(true);
    edit_box->setEditStatic(true);

    MyGUI::TextBox* text_box = dynamic_cast<MyGUI::TextBox*>(mDialogLabel->getMyGUIWidget());
    text_box->setTextAlign(MyGUI::Align::Left);

    __onHealthChanged(0,100);
    __onAmmoChanged(0, 60);
    __onClipNumChanged(0, 5);

    __resetGui();

    dt::GuiManager::get()->setMouseCursorVisible(false);
}

void BattleStateTest::updateStateFrame(double simulation_frame_time) {
    static bool flag = false;

    if (!flag) {
        flag = true;
        this->getScene("BattleStateTest")->getPhysicsWorld()->setGravity(Ogre::Vector3(0, -9.8, 0));
    }
}

BattleStateTest::BattleStateTest(uint16_t tot_enemy_num, uint16_t tot_crystal_num):
		mQuestionLabel(nullptr),
		mDialogLabel(nullptr),
		mTotalEnemyNum(tot_enemy_num),
		mRemainEnemyNum(tot_enemy_num),
		mTotalCrystalNum(tot_crystal_num),
		mObtainedCrystalNum(0) {
}

//bool BattleStateTest::isVictory() {
//	return false;
//}

void BattleStateTest::win() {
    auto state_mgr = dt::StateManager::get();
    state_mgr->pop(1);

    if (mNextStage != "") {
        state_mgr->setNewState(new BattleStateTest(mNextStage));
    } else {
        state_mgr->setNewState(new MenuState());
    }
}

QString BattleStateTest::getBattleStateName() const {
	return QString("BattleState");
}

dt::GuiLabel* BattleStateTest::getDialogLabel() {
	return mDialogLabel;
}

void BattleStateTest::setDialogLabel(dt::GuiLabel* dialog_label) {
	if (dialog_label) {
		mDialogLabel = dialog_label;
	}
}

uint16_t BattleStateTest::getTotalEnemyNum() const {
	return mTotalEnemyNum;
}

void BattleStateTest::setTotalEnemyNum(uint16_t total_enemy_num) {
	mTotalEnemyNum = total_enemy_num;
}

uint16_t BattleStateTest::getRemainEnemyNum() const {
	return mRemainEnemyNum;
}

void BattleStateTest::setRemainEnemyNum(uint16_t remain_enemy_num) {
	mRemainEnemyNum = remain_enemy_num;
}

uint16_t BattleStateTest::getTotalCrystalNum() const {
	return mTotalCrystalNum;
}

void BattleStateTest::setTotalCrystalNum(uint16_t total_crystal_num) {
	mTotalCrystalNum = total_crystal_num;
}

uint16_t BattleStateTest::getObtainedCrystalNum() const {
	return mObtainedCrystalNum;
}

void BattleStateTest::setObtainedCrystalNum(uint16_t obtained_crystal_num) {
	mObtainedCrystalNum = obtained_crystal_num;
}

dt::GuiEditBox* BattleStateTest::getQuestionLabel() {
	return mQuestionLabel;
}

void BattleStateTest::setQuestionLabel(dt::GuiEditBox* question) {
	if (question) {
		mQuestionLabel = question;
	}
}

// Slots

void BattleStateTest::__onTriggerText(uint16_t text_id) {
	mQuestionLabel->show();
}

void BattleStateTest::__onHealthChanged(uint16_t pre_health, uint16_t cur_health) {
    __changeDigits(mHealthHUD, cur_health);
}

void BattleStateTest::__onAmmoChanged(uint16_t pre_ammo, uint16_t cur_ammo) {
    __changeDigits(mAmmoHUD, cur_ammo);
}

void BattleStateTest::__onClipNumChanged(uint16_t pre_num, uint16_t cur_num) {
    __changeDigits(mClipNumHUD, cur_num);
}

void BattleStateTest::__onGetCrystal() {

}

void BattleStateTest::__onTriggerQA() {
	getQuestionLabel()->show();
}

void BattleStateTest::__onAnswerButtonClick(std::shared_ptr<MyGUI::Widget> sender) {

}

QString BattleStateTest::getNextStage() const {
    return mNextStage;
}

void BattleStateTest::setNextStage(const QString next_stage) {
    mNextStage = next_stage;
}

QString BattleStateTest::getStage() const {
    return mStage;
}

void BattleStateTest::setStage(const QString stage) {
    mStage = stage;
}

void BattleStateTest::__resetGui() {
    dt::GuiRootWindow& root_win = dt::GuiManager::get()->getRootWindow();
    auto coordination = root_win.getMyGUIWidget()->getAbsoluteCoord();

    mDialogLabel->setCaption("Jym Die Die Die!");

    int gap_h_large = (float)coordination.width / 15.0f;
    int gap_v_large = (float)coordination.height / 15.0f;
    int size_h_large = (float)coordination.width / 10.0f;
    int size_v_large = (float)coordination.height / 10.0f;

    int gap_h_medium = (float)coordination.width / 30.0f;
    int gap_v_medium = (float)coordination.height / 30.0f;
    int size_h_medium = (float)coordination.width / 13.0f;
    int size_v_medium = (float)coordination.height / 13.0f;

    int gap_h_small = (float)coordination.width / 100.0f;
    int gap_v_small = (float)coordination.height / 100.0f;
    int size_h_small = (float)coordination.width / 40.0f;
    int size_v_small = (float)coordination.height / 40.0f;

    mFrontSight->getMyGUIWidget()->setAlign(MyGUI::Align::Center);
    mFrontSight->setSize((int)(gap_h_medium * 1.5f), (int)(gap_h_medium * 1.5f));
    mFrontSight->getMyGUIWidget()->setPosition(coordination.width / 2 - mFrontSight->getMyGUIWidget()->getSize().width / 2,
        coordination.height / 2 - mFrontSight->getMyGUIWidget()->getSize().height /2);

    mQuestionLabel->setSize(4.0f / 13.0f + 0.1f, 1.0f / 3.0f);
    mQuestionLabel->setPosition((int)(coordination.width / 2.0f - mQuestionLabel->getMyGUIWidget()->getAbsoluteRect().width() / 2.0f), 
        (int)(coordination.height / 2.0f - mQuestionLabel->getMyGUIWidget()->getAbsoluteRect().height() / 2.0f));

    for (uint8_t i = 0 ; i < 4 ; ++i) {
        mAnswerButtons[i]->setPosition((int)(coordination.right() / 2.0f - (2 * size_h_medium + 1.5 * gap_h_medium) + i * (size_h_medium + gap_h_medium)), 
            (int)(coordination.height / 2.0f + mQuestionLabel->getMyGUIWidget()->getAbsoluteRect().height() / 2 + gap_v_medium));
        mAnswerButtons[i]->setSize(1.0f / 13.0f, 1.0f / 13.0f);
    }

    for (uint8_t i = 0 ; i < 3 ; ++i) {
        mHealthHUD[i]->setSize(1.0f / 30.0f, 1.0f / 15.0f);
        mAmmoHUD[i]->setSize(1.0f / 30.0f, 1.0f / 15.0f);
        mClipNumHUD[i]->setSize(1.0f / 50.0f, 1.0f / 25.0f);

        mClipNumHUD[i]->setPosition(coordination.width - (3 - i) * (mClipNumHUD[0]->getMyGUIWidget()->getSize().width - gap_h_small * 3 / 8) - gap_h_small,
            coordination.height - 3 * gap_v_medium);
        mAmmoHUD[i]->setPosition(coordination.width - (3 - i) * (mAmmoHUD[0]->getMyGUIWidget()->getSize().width - gap_h_small / 2) - gap_h_small * 3 / 8
            - (coordination.width - mClipNumHUD[0]->getMyGUIWidget()->getPosition().left), coordination.height - 3 * gap_v_medium);
        mHealthHUD[i]->setPosition(i * (mHealthHUD[0]->getMyGUIWidget()->getSize().width - gap_h_small / 2) + gap_h_small / 2, coordination.height - 3 * gap_v_medium);
    }

    mDialogLabel->setSize(300, size_v_small);
    mDialogLabel->setPosition(mHealthHUD[0]->getMyGUIWidget()->getPosition().left, mHealthHUD[0]->getMyGUIWidget()->getPosition().top - mHealthHUD[0]->getMyGUIWidget()->
        getSize().height - gap_v_small / 2);
}

void BattleStateTest::__changeDigits(std::vector<dt::GuiImageBox*>& pics, uint16_t number) {
    uint16_t digit;
    uint16_t factor;

    for (uint8_t i = 0 ; i < 3 ; ++i) {
        factor = pow(10.0, (int)(2 - i));

        digit = number / factor;
        number %= factor;

        pics[i]->setImageTexture(dt::Utils::toString(digit) + ".png");
    }
}