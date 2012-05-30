#include "StateChangeTest.h"
#include "SceneLoader.h"
#include "StateManager.h"
#include <Scene/StateManager.hpp>
#include <Core/ResourceManager.hpp>

void FirstTestState::onInitialize()
{
    dt::ResourceManager::get()->addDataPath(QDir("data"));
    dt::ResourceManager::get()->addResourceLocation("Materials","FileSystem", true);
    dt::ResourceManager::get()->addResourceLocation("Models","FileSystem", true);
    dt::ResourceManager::get()->addResourceLocation("Scripts","FileSystem", true);
    dt::ResourceManager::get()->addResourceLocation("Others","FileSystem", true);

    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    this->addScene(SceneLoader::loadScene("Startup.scene"));

    //dt::GuiRootWindow& rootWindow = dt::GuiManager::get()->getRootWindow();
    //mNextButton = rootWindow.addChildWidget(new dt::GuiButton("next"));
    //mNextButton->setCaption("Next");
    //mNextButton->setSize(250, 100);
    //mNextButton->setPosition(100, 100);
    //mNextButton->getMyGUIWidget()->eventMouseButtonClick += MyGUI::newDelegate(this, &FirstTestState::onClick);
}

void FirstTestState::onDeinitialize()
{
    //dt::GuiManager::Get()->GetRootWindow().RemoveChild("next");
}

void FirstTestState::onClick(MyGUI::Widget* sender)
{
    //NewStateManager::getInstance().pop();
    //NewStateManager::getInstance().push(SECOND_STATE);
}

void FirstTestState::updateStateFrame(double simulation_frame_time)
{
}

//void SecondTestState::onInitialize()
//{
//
//    dt::Scene::SceneSP scene = addScene(new dt::Scene("scene2"));
//
//    dt::GuiRootWindow& rootWindow = dt::GuiManager::get()->getRootWindow();
//    mReturnButton = rootWindow.addChildWidget(new dt::GuiButton("return"));
//    mReturnButton->setCaption("Return");
//    mReturnButton->setSize(250, 100);
//    mReturnButton->setPosition(100, 100);
//    mReturnButton->getMyGUIWidget()->eventMouseButtonClick += MyGUI::newDelegate(this, &SecondTestState::onClick);
//}
//
//void SecondTestState::onDeinitialize()
//{
//    //dt::GuiManager::Get()->GetRootWindow().RemoveChild("return");
//}
//
//void SecondTestState::onClick(MyGUI::Widget* sender)
//{
//    NewStateManager::getInstance().pop();
//    NewStateManager::getInstance().push(FIRST_STATE);
//}
//
//void SecondTestState::updateStateFrame(double simulation_frame_time)
//{
//}

int main(int argc, char **argv)
{
    dt::Game game;
    game.run(new FirstTestState(), argc, argv);
    system("pause");
}